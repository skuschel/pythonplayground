#!/usr/bin/env python2

# Stephan Kuschel, 150806

def main():
    '''
    this script expects a single as argument. Argparse will detect,
    if the given string points to a file or if a pipe has been used.
    Try this:
    $ ./parsepipe.py parsepipe.py
    Reads the contents of this file.
    $ echo hallo | ./parsepipe.py -
    Reads 'hallo' as the input.
    '''
    import argparse
    import sys

    parser = argparse.ArgumentParser(description=main.__doc__,
                                     formatter_class=argparse.RawTextHelpFormatter)
    parser.add_argument('infile', type=argparse.FileType('r'), default=sys.stdin, nargs='?')
    args = parser.parse_args()

    print args
    print args.infile.readlines()



if __name__ == '__main__':
    main()
