#!/usr/bin/env python3

# Stephan Kuschel, 151117

'''
Start this file and open your browser at
localhost:8000
'''

import tornado.web
import tornado.ioloop
import numpy as np
import time

class MainPageHandler(tornado.web.RequestHandler):
    def get(self):
        self.write('<a href="/hello">hello</a><br>')
        self.write('<a href="/prim/65535">Primfaktorenzerlegung</a><br>')



class HalloWeltHandler(tornado.web.RequestHandler):
    def get(self):
        self.write('Hallo Welt')

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


# Let tornado know which handlers to use when
handlers = [
            ('/', MainPageHandler),
            ('/hello', HalloWeltHandler),
            ('/prim/([0-9]+)', PrimfaktorHandler),
           ]

app = tornado.web.Application(handlers)
app.listen(8000)
tornado.ioloop.IOLoop.instance().start()
