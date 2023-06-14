#!/usr/bin/env python3

import matplotlib.pyplot as plt
from matplotlib import cm
import numpy as np
from numpy import linalg as LA
import re

def extractN(ns):
  reg = re.compile(r''+ns+'.+')
  file1 = open('info.dat', 'r')
  strings = re.search(reg, file1.read())
  file1.close()
  assert(strings)
  return int(strings.group().split()[1])

##########################
if __name__== "__main__":
##########################
  nx = extractN('nx')
  ny = extractN('ny')
  print(nx, ny)
  totDofs = nx*ny

  fomCoords = np.loadtxt('coordinates.dat', dtype=float)
  x_fom, y_fom = fomCoords[:,1], fomCoords[:,2]
  x_fom = np.reshape(x_fom, (ny,nx))
  y_fom = np.reshape(y_fom, (ny,nx))

  fomTestD = np.fromfile("diffusion_reaction_2d_solution.bin")
  nt = int(np.size(fomTestD)/totDofs)
  print("fomTest: nt = ", nt)
  fomTestD = np.reshape(fomTestD, (nt, totDofs))

  fig, ax = plt.subplots(subplot_kw={"projection": "3d"})
  #fig = plt.figure(1)
  for i in range(nt-1, nt):
    fomS = fomTestD[i,:]
    fomS = np.reshape(fomS, (nx*ny, 1))
    y = fomS[:,0]
    print(np.min(y), np.max(y))
    y1 = np.reshape(y, (ny,nx))

    ax = plt.gca()
    #plt.clf()
    ax.clear()
    surf = ax.plot_surface(x_fom, y_fom, y1, cmap=cm.coolwarm, linewidth=0, antialiased=False)

    # h = plt.contourf(x_fom, y_fom, y1)
    # ax.set_aspect(aspect=1.)
    # plt.colorbar()
    #plt.pause(0.1)
    plt.show()
