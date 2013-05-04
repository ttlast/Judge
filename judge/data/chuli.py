####
#           usage : python ./chuli.py -r -m p 1002
#
#          age: eol_convert.py [-h] [-r] [-m {u,p,w,m,d}] [-k] [-f]
#                                filename [filename ...]
#          
#          Convert Line Ending
#          
#          positional arguments:
#            filename        file names
#          
#          optional arguments:
#            -h, --help      show this help message and exit
#            -r              walk through directory
#            -m {u,p,w,m,d}  mode of the line ending
#            -k              keep output file date
#            -f              force conversion of binary files
#####

#!/usr/bin/python
import os
import re
import os.path

def convert_line_endings(temp, mode):
    if mode in ['u', 'p']: #unix, posix
        temp = temp.replace('\r\n', '\n')
        temp = temp.replace('\r', '\n')
    elif mode == 'm':      #mac (before Mac OS 9)
        temp = temp.replace('\r\n', '\r')
        temp = temp.replace('\n', '\r')
    elif mode == 'w':      #windows
        temp = re.sub("\r(?!\n)|(?<!\r)\n", "\r\n", temp)
    return temp

def convert_file(filename, args):
    statinfo = None
    with file(filename, 'rb+') as f:
        data = f.read()
        if '\0' in data and not args.force:  #skip binary  file... ?
            print '%s is a binary file?, skip...' % filename
            return
        newdata = convert_line_endings(data, args.mode)
        if (data != newdata):
            statinfo = os.stat(filename) if args.keepdate else None
            f.seek(0)
            f.write(newdata)
            f.truncate()
    if statinfo:
        os.utime(filename, (statinfo.st_atime, statinfo.st_mtime))
    print filename

def walk_dir(d, args):
    for root, dirs, files in os.walk(d):
        for name in files:
            convert_file(os.path.join(root, name), args)

if __name__ == '__main__':
    import argparse
    import sys
    parser = argparse.ArgumentParser(description='Convert Line Ending')
    parser.add_argument('filename', nargs='+', help='file names')
    parser.add_argument('-r', dest='recursive', action='store_true',
            help='walk through directory')
    parser.add_argument('-m', dest='mode', default='d', choices='upwmd',
            help='mode of the line ending')
    parser.add_argument('-k', dest='keepdate', action='store_true',
            help='keep output file date')
    parser.add_argument('-f', dest='force', action='store_true',
            help='force conversion of binary files')
    args = parser.parse_args()
    if args.mode == 'd':
        args.mode = 'w' if sys.platform == 'win32' else 'p'

    for filename in args.filename:
       if os.path.isdir(filename):
           if args.recursive:
               walk_dir(filename, args)
           else:
                print '%s is a directory, skip...' % filename
       elif os.path.exists(filename):
            convert_file(filename, args)
       else:
           print '%s does not exist' % filename
