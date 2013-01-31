#!/usr/bin/python
import sys
f = open(sys.argv[1],'r')
lines = f.readlines()
f.close()

print str(sum([int (x) for x in lines]) / len(lines)),'microseconds'
