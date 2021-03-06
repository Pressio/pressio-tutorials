#!/usr/bin/env python3

import matplotlib.pyplot as plt
from matplotlib import cm
import numpy as np
axis_font = {'size':20,'family':'serif'}
#close("all")

if __name__== "__main__":
  ## Post processing file. Makes plots of solution h at x,y=1.25
  nx = 64
  ny = nx
  fom_final_time = 10.
  animate = True
  def index_mapper(i,j):
    return (j%ny)*nx + i%nx

  ##data goes in the following order
  # h_{11}^1, u_{11}^1, v_{11}^1, h_{12}^1, ... , h_{11}^2

  data_fom = np.fromfile('solution0.bin')
  nt_fom = int(np.size(data_fom)/(nx*ny*3))
  u_fom = np.reshape(data_fom,(nt_fom,3*nx*ny) )
  u_fom = np.reshape(u_fom,(nt_fom,nx,ny,3))
  t_fom = np.linspace(0,fom_final_time,nt_fom)

  fig, ax = plt.subplots()
  plt.plot(t_fom, u_fom[:,int(nx/4),int(ny/4), 0],label='Truth')
  plt.xlabel(r'$t$',**axis_font)
  plt.ylabel(r'$h(1.25,1.25,t)$',**axis_font)
  ax.legend(loc=1)
  plt.tight_layout()
  fig.savefig('result.png', format="png", bbox_inches='tight', dpi=300)
  plt.close("all")

  if animate:
    x = np.linspace(0,5,nx)
    y = np.linspace(0,5,ny)
    x,y = np.meshgrid(x,y)
    fig = plt.figure(1)

    for i in range(0,np.shape(u_fom)[0]):
      plt.clf()
      ax = fig.gca(projection='3d')
      ax.plot_surface(x,y,u_fom[i,:,:,0],cmap=cm.jet)
      ax.set_zlim(np.amin(u_fom[...,0]),np.amax(u_fom[...,0]))
      plt.pause(0.001)
