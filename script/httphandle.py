#-*- coding:utf-8 -*-
import _engine


#BaseHTTPServer.BaseHTTPRequestHandler
class HttpHandle:
	def __init__(self,connect_id,sender):
		self.conn_id = connect_id
		self.send = sender

	

	def do_GET(self):
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
		print [reps_header]
		print [reps_content]
		self.send(reps_content)

	def handle_line(self,line):
		if line.find("GET") >= 0:
			print 'do get!!!'
			self.do_GET()

	def handle_message(self):
		pass


	def handle_close(self):
		_close_connect(self.conn_id)


	def send(self,s):
		self.sender(self.conn_id,s)









handles = {}
def new_connect(conn_id,sender):
	# print "py new_connect:",conn_id
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
	# print "py handle_close:",conn_id
	h = handles[conn_id]
	h.handle_close()

def _close_connect(conn_id):
	# print "py close_connect:",conn_id
	del handles[conn_id]

