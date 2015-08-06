#!/usr/bin/env python2

# Stephan Kuschel, 150806

import argparse

parser = argparse.ArgumentParser()
parser.add_argument('xx', help='global argument, mandatory')
parser.add_argument('-o', help='global argument, optional')
subparsers = parser.add_subparsers(help='choose subparser')
subparser1 = subparsers.add_parser('s1', help='subparser1 help')
subparser1.add_argument('1a', help='1a help')
subparser1.add_argument('1b', help='1b help')
subparser2 = subparsers.add_parser('s2', help='subparser2 help')
subparser2.add_argument('2a', help='1a help')
subparser2.add_argument('2b', help='1b help')

args = parser.parse_args()

print 'Arguments parsed as:'
print args
