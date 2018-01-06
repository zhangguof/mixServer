#-*- coding:utf-8 -*-
import _engine
import BaseHTTPServer
from cStringIO import StringIO

#BaseHTTPServer.BaseHTTPRequestHandler
class HttpHandle(BaseHTTPServer.BaseHTTPRequestHandler):
	def __init__(self,connect_id,sender):
		print "======="
		self.conn_id = connect_id
		self.send = sender
		self.client_address = sender.client_addr
		self.rbuf = StringIO()
		self.setup()

	def setup(self):
		# self.connection = self.request
		self.rfile = self.rbuf
		self.wfile = self.send
	
	def do_GET(self):
		self.close_connection = 1
		# self.send_error(404)
		# self.send.close()
		# return
		html_code = '''<html>
		<head>
		<title>An Example Page</title>
		</head>
		<body>
		Hello World!!!.
		</body>
		</html>'''
		statu_code = "HTTP/1.1 %s\r\n"%("200 OK")
		reps = {}
		reps["Content-Encoding"] = "utf-8"
		reps["Content-Length"] = len(html_code)
		reps["Content-Type"] = "text/html; charset=UTF-8"
		reps_header = statu_code+"\r\n".join(
			["%s: %s"%(k,v) for k,v in reps.items()]
			) + "\r\n"
		reps_content = reps_header + "\r\n" + html_code
		self.send(reps_content)
		# if self.close_connection:
		# 	self.send.close()

	def handle_line(self,line):
		self.rbuf.write(line+"\r\n")
		if len(line) == 0:
			self.rbuf.write("\r\n")
			self.rbuf.seek(0)
			self.close_connection = 1
			self.handle_one_request()

	def handle_message(self):
		pass


	def handle_close(self):
		if not self.send.closed:
			self.send.close()
		_close_connect(self.conn_id)

		


#c++ interface
#sender = pySender
handles = {}
def new_connect(conn_id,sender):
	print "py new_connect:",conn_id
	print sender
	handles[conn_id] = HttpHandle(conn_id,sender)
	
	# sender("ssssssssssss");


def handle_line(conn_id,line):
	h = handles[conn_id]
	h.handle_line(line)

def handle_message(conn_id,s):
	print "py handle_message:",conn_id
	h = handles[conn_id]
	h.handle_message(s)

def handle_close(conn_id):
	print "py handle_close:",conn_id
	h = handles[conn_id]
	h.handle_close()

def _close_connect(conn_id):
	print "py _close_connect:",conn_id
	del handles[conn_id]

