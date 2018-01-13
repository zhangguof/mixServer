#-*- coding:utf-8 -*-
import _engine
import BaseHTTPServer
from cStringIO import StringIO
import hashlib
import base64
import time
import struct

html_test_code = '''<html>
<head>
<title>An Example Page</title>
</head>
<body>
Hello World!!!.
</body>
</html>'''

# HTTP/1.1 101 Switching Protocols
# Upgrade: websocket
# Connection: Upgrade
# Sec-WebSocket-Accept: fFBooB7FAkLlXgRSz0BT3v4hq5s=
# Sec-WebSocket-Location: ws://example.com/

weekdayname = ['Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat', 'Sun']

monthname = [None,
             'Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun',
             'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec']

WS_KEY = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

def make_response(code,message,headers):
	res = "%s %s %s\r\n"%("HTTP/1.1",code,message)
	head_str = "\r\n".join(["%s: %s"%(k,v) for k,v in headers])
	return res + head_str + "\r\n"

#https://tools.ietf.org/html/rfc6455
#  0                   1                   2                   3
#  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
# +-+-+-+-+-------+-+-------------+-------------------------------+
# |F|R|R|R| opcode|M| Payload len |    Extended payload length    |
# |I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
# |N|V|V|V|       |S|             |   (if payload len==126/127)   |
# | |1|2|3|       |K|             |                               |
# +-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
# |     Extended payload length continued, if payload len == 127  |
# + - - - - - - - - - - - - - - - +-------------------------------+
# |                               |Masking-key, if MASK set to 1  |
# +-------------------------------+-------------------------------+
# | Masking-key (continued)       |          Payload Data         |
# +-------------------------------- - - - - - - - - - - - - - - - +
# :                     Payload Data continued ...                :
# + - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
# |                     Payload Data continued ...                |
# +---------------------------------------------------------------+

# Octet i of the transformed data ("transformed-octet-i") is the XOR of
#    octet i of the original data ("original-octet-i") with octet at index
#    i modulo 4 of the masking key ("masking-key-octet-j"):

#      j                   = i MOD 4
#    transformed-octet-i = original-octet-i XOR masking-key-octet-j
def ws_unmask(mask_codes,data):
	if not mask_codes:
		return data
	_data = []
	# print 'mask data:',[data]
	for i,d in enumerate(data):
		_data.append(chr(ord(d) ^ mask_codes[i%4]))
	return "".join(_data)

def get_byte_bit(byte,bs,be):
	#0..7 bit
	assert(bs>=0 and be<=7 and bs<=be)
	mask = ((1<<(be-bs+1))-1)
	return (byte >> (7-be)) & mask

def get_bits(byte,bits):
	L = []
	for s,e in bits:
		L.append(get_byte_bit(byte,s,e))


class WSFrame:
	def __init__(self,handle):
		self.header = {}
		self.mask_codes = []
		self.buf = ""
		self.remain_len = 2 #byte going to read
		self.data_len = None
		self.payload = StringIO()
		self.reading_head = True
		self.handle = handle
		self.mode = None
	@staticmethod
	def make_frame_data(msg,mask_codes=None):
		len1 = len(msg)
		len2 = None
		if len1 > 65535:
			len2 = len(msg)
			len1 = 127
		elif len1 > 125:
			len2  = len(msg)
			len1 = 126
		FIN,RSV1,RSV2,RSV3 = 1,0,0,0
		raws = StringIO()
		if mask_codes:
			len1 = 0x80 | len1
		opcode = 0x1
		header = struct.pack(">BB",\
				(0x80&(FIN<<7))  | (0x40&(RSV1<< 6)) | \
				(0x40&(RSV2<<5)) |(0x40&(RSV3<< 4))  | \
				0x0F&opcode,\
				len1
				)
		raws.write(header)
		if len2 is not None:
			if len2>65535:
				raws.write(struct.pack(">Q",len2))
			else:
				raws.write(struct.pack(">H",len2))
		if mask_codes:
			mask = (mask_codes[0]<<24) | (mask_codes[1]<<16) |\
					(mask_codes[2]<<8) | (mask_codes[3])
			raws.write(struct.pack(">I",mask))

		data = ws_unmask(mask_codes,msg)
		raws.write(struct.pack("%ds"%len(data),data))
		raw_data =  raws.getvalue()
		return raw_data


	def on_read(self,s):
		# print "on read====",len(s),len(self.buf)
		if self.reading_head:
			self.buf = self.buf + s
			if len(self.buf)< self.remain_len:
				return
		# print "====self.remain_len:",self.remain_len,len(s),len(self.buf)
		if not self.header:
			mark,_len = struct.unpack(">BB",self.buf[:2])
			self.buf = self.buf[2:]
			# print bin(mark[0]),hex(mark[0])
			FIN =  (mark&0x80) >> 7 
			RSV1 = (mark&0x40) >> 6
			RSV2 = (mark&0x20) >> 5
			RSV3 = (mark&0x10) >> 4
			opcode = mark&0x0F
			# print FIN,RSV1,RSV2,RSV3,opcode
			mask = (_len&0x80) >> 7
			len1 = (_len&0x7F)
			self.header = dict(FIN=FIN,RSV1=RSV1,RSV2=RSV2,RSV3=RSV3,
								opcode=opcode,mask=mask,len1=len1)
			print self.header
			self.remain_len = 0
			if len1==126:
				self.remain_len += 2
			elif len1 == 127:
				self.remain_len += 8
			else:
				self.remain_len += len1
				self.data_len = len1
			if mask:
				self.remain_len += 4
			print "len1:%d,mask:%d,remain_len:%d,opcode:%d"%(len1,mask,self.remain_len,opcode)
			if opcode==0x8:
				print "opcode :%x,handle close"%(opcode)
				self.handle.handle_close()
				return
			if opcode == 0x1:
				self.mode = "text"
			elif opcode == 0x2:
				self.mode = "bin"
			if len(self.buf)>0:
				self.on_read("")

		elif self.data_len is None:
			if self.header['len1'] == 126:
				_len2 = struct.unpack(">H",self.buf[:2])[0]
				self.buf = self.buf[2:]
				self.data_len = _len2
				self.remain_len += (_len2-2)
			elif self.header['len1'] == 127:
				_len2 = struct.unpack(">Q",self.buf[:8])[0]
				self.buf = self.buf[8:]
				self.data_len = _len2
				self.remain_len += (_len2-8)
			print "===read len=====",_len2
			if not self.header['mask']:
				self.reading_head = False
			if len(self.buf) > 0:
				self.on_read("")

		elif self.header['mask'] and not self.mask_codes:
				mask = struct.unpack(">I",self.buf[:4])[0]
				self.reading_head = False
				# print "==== read mask!,",mask,hex(mask)
				# mask  = self.mask
				while mask:
					self.mask_codes.append(mask&0xFF)
					mask = mask >> 8
				self.mask_codes.reverse()
				self.buf = self.buf[4:]
				self.remain_len -= 4
				if len(self.buf) > 0:
					self.on_read("")
		else:
			if self.buf:
				s = self.buf + s
			n = len(s)
			self.payload.write(s)
			self.remain_len -= n
			# print "read data!!!",n
			if self.remain_len == 0:
				self.payload.reset()
				if len(self.mask_codes) > 0:
					data = ws_unmask(self.mask_codes,self.payload.read())
				else:
					data = self.payload.read()
				self.handle.handle_frame(data)


class WSHandle:
	def __init__(self,conn_id,sender):
		self.conn_id = conn_id
		self.send  = sender
		self.cur_frame  = None

	def on_handle(self,headers):
		print "new connect."
		key,host = headers["Sec-WebSocket-Key"],headers["Host"]
		print "key:%s,host:%s"%(key,host)

		self.send_response(key,host)

	def send_response(self,key,host):
		hash_key = base64.b64encode(hashlib.sha1(key+WS_KEY).digest())
		headers = [("Upgrade","websocket"),
				   ("Connection","Upgrade"),
				   ("Sec-WebSocket-Accept",hash_key),
				   ("Content-Length",0),
				   ("Date",self.date_time_string()),
				   ("Sec-WebSocket-Location:","ws://%s"%host)
				  ]
		res = make_response(101,"Switching Protocols",headers) + "\r\n"
		self.send(res)


	def handle_message(self,s):
		if self.cur_frame is None:
			self.cur_frame = WSFrame(self)
		self.cur_frame.on_read(s)

	def send_message(self,s):
		self.send(s)

	def handle_close(self):
		if not self.send.closed:
			self.send.close()
	def handle_frame(self,data):
		self.cur_frame  = None
		print "===handle frame:",len(data)
		fs = WSFrame.make_frame_data("echo:%s"%data)
		self.send_message(fs)



	def date_time_string(self, timestamp=None):
		"""Return the current date and time formatted for a message header."""
		if timestamp is None:
		    timestamp = time.time()
		year, month, day, hh, mm, ss, wd, y, z = time.gmtime(timestamp)
		s = "%s, %02d %3s %4d %02d:%02d:%02d GMT" % (
				weekdayname[wd],
				day, monthname[month], year,
				hh, mm, ss)
		return s


#BaseHTTPServer.BaseHTTPRequestHandler
class HttpHandle(BaseHTTPServer.BaseHTTPRequestHandler):
	def __init__(self,connect_id,sender):
		self.conn_id = connect_id
		self.send = sender
		self.client_address = sender.client_addr
		self.rbuf = StringIO()
		self.tmp_buf = ""
		self.setup()
		

	def setup(self):
		# self.connection = self.request
		self.rfile = self.rbuf
		self.wfile = self.send
	
	def do_GET(self):
		self.close_connection = 1
		print "====handles:",handles

		if self.headers.get("Connection") == "Upgrade":
			self.send.reading_line = False
			ws_handle = WSHandle(self.conn_id,self.send)
			_switch_handle(self.conn_id,ws_handle)
			ws_handle.on_handle(self.headers)
			return

		content = html_test_code
		headers = [("Content-Encoding","utf-8"),
				   ("Content-Type","text/html; charset=UTF-8"),
				   ("Content-Length",len(content))
		]
		res_str = make_response(200,"OK",headers) + "\r\n" + content
		self.send(res_str)

		if self.close_connection:
			self.handle_close()

	def handle_line(self,line):
		self.rbuf.write(line+"\r\n")
		if len(line) == 0:
			self.rbuf.write("\r\n")
			self.rbuf.seek(0)
			self.handle_one_request()

	def handle_message(self,s):
		print "handle message:",s

	def handle_close(self):
		if not self.send.closed:
			self.send.close()

		


#c++ interface
#sender = pySender
handles = {}
def new_connect(conn_id,sender):
	print "py new_connect:",conn_id
	handles[conn_id] = HttpHandle(conn_id,sender)

def _switch_handle(conn_id,handle):
	handles[conn_id] = handle
	# sender("ssssssssssss");

# def handle_stream(conn_id,s):
# 	h = handles[conn_id]
# 	h.handle_stream(s)

def handle_line(conn_id,line):
	h = handles[conn_id]
	h.handle_line(line)

def handle_message(conn_id,s):
	print "py handle_message:",conn_id
	h = handles[conn_id]
	h.handle_message(s)

#call by c++
def handle_close(conn_id):
	print "py handle_close:",conn_id
	h = handles[conn_id]
	h.handle_close()
	del handles[conn_id]

# import dis
# print dis.disassemble(handle_message.func_code)

#call by py!
# def _close_connect(conn_id):
# 	print "py _close_connect:",conn_id
# 	del handles[conn_id]

