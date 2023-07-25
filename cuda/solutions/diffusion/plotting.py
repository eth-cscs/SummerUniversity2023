#
# Plotting program, to use to visualize the output of the mini-app
# CSCS - Summer School 2015
# Written by Jean M. Favre
#
# Tested on daint, with python/2.7.6
# Date: Thu May 21 14:11:38 CEST 2015
#

import argparse
import matplotlib

P = argparse.ArgumentParser(description='plot miniapp and diffusion output')
P.add_argument('-s', '--show', action='store_true',
                   help='show result in popup window (requries X-forwarding)')
args = P.parse_args()
if not args.show:
    matplotlib.use('Agg')

import pylab as pl
import numpy as np

# change filename below as appropriate
headerFilename =  "output.bov"
headerfile = open(headerFilename, "r")
header = headerfile.readlines()
headerfile.close()

rawFilename = header[1].split()[1]

res_x, res_y = [int(x) for x in header[2].strip().split()[1:3]]
print('xdim', res_x, ' ydim', res_y)

data = np.fromfile(rawFilename, dtype=np.double, count=res_x*res_y, sep='')

x = np.linspace(0., 1., res_x)
y = np.linspace(0., 1.*res_y/res_x, res_y)
X, Y = np.meshgrid(x, y)

# number of contours we wish to see
V = [-0.01, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.01]
pl.axes().set_aspect('equal')
pl.contourf(X, Y, data.reshape(res_y, res_x), V, alpha=.75, cmap='jet')
pl.contour(X, Y, data.reshape(res_y, res_x), V, colors='black')

pl.savefig("output.png", dpi=72)
if args.show:
    pl.show()
