#!/usr/bin/env python2

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
import os


class MyFormHandler(tornado.web.RequestHandler):
    def get(self):
        self.write('<html><body><form action="/" method="post">'
                   '<input type="text" name="message">'
                   '<input type="submit" value="Submit">'
                   '</form></body></html>')

    def post(self):
        self.set_header("Content-Type", "text/plain")
        self.write("You wrote " + self.get_argument("message"))


handlers = [
            ('/', MyFormHandler)
           ]


def main():
    app = tornado.web.Application(handlers)
    app.listen(8000)
    tornado.ioloop.IOLoop.instance().start()

if __name__=='__main__':
    main()

