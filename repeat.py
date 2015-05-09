#!/usr/bin/env python

import sys
from subprocess import Popen


def experiment(N):
	processes = [Popen(["./ga", "config", "%d"%num]) for num in range(1, N + 1)]
	map(lambda p: p.wait(), processes)

if __name__ == "__main__":
	if len(sys.argv) < 2:
		print "Usage: ./ga N"
	else:
		experiment(int(sys.argv[1]))
