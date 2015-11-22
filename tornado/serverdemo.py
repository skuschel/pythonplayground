#!/usr/bin/env python3

# Stephan Kuschel, 151117

'''
Start this file and open your browser at
localhost:8000
'''

import tornado.web
import tornado.ioloop
from tornado import websocket
import numpy as np
import time
import subprocess


class MainPageHandler(tornado.web.RequestHandler):
    def get(self):
        self.write('<a href="/hello">hello</a><br>')
        self.write('<a href="/prim/65535">Primfaktorenzerlegung</a><br>')
        self.write('<a href="/uptime">show uptime</a><br>')
        self.write('<a href="/static/index.html">display static file</a><br>')

GLOBALS = {'hellosockets' :[]}

class HelloClientSocket(websocket.WebSocketHandler):
    def open(self):
        GLOBALS['hellosockets'].append(self)
        print("WebSocket opened. Total sockets open: {:}".format(len(GLOBALS['hellosockets'])))

    def on_close(self):
        GLOBALS['hellosockets'].remove(self)
        print("WebSocket closed. Total sockets open: {:}".format(len(GLOBALS['hellosockets'])))

class HelloPushHandler(tornado.web.RequestHandler):
    def get(self, *args, **kwargs):
        data = self.get_argument('data')
        for socket in GLOBALS['hellosockets']:
            socket.write_message(data)
        self.write('Posted: {:}'.format(data))

class HalloWeltHandler(tornado.web.RequestHandler):
    def get(self):
        page = '''
<html>
<head>
<script src="//ajax.googleapis.com/ajax/libs/jquery/1.8.3/jquery.min.js"></script>
<script language="javascript" type="text/javascript">
$(document).ready(function () {
    var ws = new WebSocket("ws://localhost:8000/hellosocket");
    ws.onmessage = function(event) {
       $('body').append('<div>' + event.data + '</div>');
    }
    $('body').append('<div> goto localhost:8000/hellopost?data=msg to post msg on this page.</div>');
});
</script>
</head>
<body>
Hallo Welt!
</body>
</html>
'''
        self.write(page)


class PrimfaktorHandler(tornado.web.RequestHandler):

    def writeprimfaktoren(self, zahl):
        for i in range(2, int(np.sqrt(zahl))+1):
            if zahl % i == 0:
                self.write(' {:}<br>'.format(i))
                self.writeprimfaktoren(zahl//i)
                return
        # if code continues here, zahl is a prime
        self.write(' {:}<br>'.format(zahl))

    def get(self, zahl):
        template = '''
Die Zahl {:} hat folgende Primfaktoren: <br>
        '''
        self.write(template.format(zahl))
        t0 = time.time()
        self.writeprimfaktoren(int(zahl))
        self.write('calculation took {:3.2f} ms'.format((time.time() - t0)*1000))


class UptimeHandler(tornado.web.RequestHandler):
    def get(self):
        out = subprocess.check_output('uptime')
        self.write(out)


# Let tornado know which handlers to use when
handlers = [
            ('/', MainPageHandler),
            ('/hello', HalloWeltHandler),
            ('/hellosocket', HelloClientSocket),
            ('/hellopush', HelloPushHandler),
            ('/prim/([0-9]+)', PrimfaktorHandler),
            ('/uptime', UptimeHandler),
            ('/static/(.*)', tornado.web.StaticFileHandler, {'path': './static'})
           ]

app = tornado.web.Application(handlers)
app.listen(8000)
tornado.ioloop.IOLoop.instance().start()
