#!/usr/bin/env python2

# Stephan Kuschel, 150603


class AccessCounter(object):

    def __init__(self, name, default=0):
        self.name = '_' + str(name)
        self.getcount = 0
        self.default = 0

    def __get__(self, obj, cls):
        self.getcount += 1
        objgetcount = getattr(obj, '_'+self.name+'_getcount_', 0)
        setattr(obj, '_'+self.name+'_getcount_', objgetcount + 1)
        print 'getcount = {:1} / obj: {:2}'.format(self.getcount, objgetcount)
        return getattr(obj, self.name, self.default)

    def __set__(self, obj, value):
        setattr(obj, self.name, value)


class A(object):
    x = AccessCounter('x')


if __name__ == '__main__':
    a = A()
    b = A()
    b.x = 2
    print ' ---- a.__class.__dict__'
    print a.__class__.__dict__
    print ' ---- a.__dict__'
    print a.__dict__
    print ' ---- b.__class.__dict__'
    print b.__class__.__dict__
    print ' ---- b.__dict__'
    print b.__dict__
    print a.x
    print a.x
    print a.x
    print b.x
    print b.x
    print b.x
