#!/usr/bin/env python3

import matplotlib.pyplot as plt
from matplotlib import cm
import numpy as np
from plotsetting import edit_figure_for_web

axis_font = {'size':20,'family':'serif'}

if __name__== "__main__":
  ## Post processing file. Makes plots of solution h at x,y=1.25
  nx = 64
  ny = 64
  fom_final_time = 10.
  rom_final_time = 10.
  K = 30 #10 bases * 3 dofs
  def index_mapper(i,j):
    return (j%ny)*nx + i%nx

  ##data goes in the following order
  # h_{11}^1, u_{11}^1, v_{11}^1, h_{12}^1, ... , h_{11}^2

  # load nearest neighbors solution
  data_fom = np.fromfile('../fom/solution0.bin')
  nt_fom = int(np.size(data_fom)/(nx*ny*3))
  u_fom = np.reshape(data_fom,(nt_fom,3*nx*ny) )
  u_fom = np.reshape(u_fom,(nt_fom,nx,ny,3))
  t_fom = np.linspace(0,fom_final_time,nt_fom)

  data_rom = np.fromfile('solution.bin')
  yRef = np.fromfile('state_ref.bin')
  basis = np.genfromtxt('basis.txt')
  nt_rom = int(np.size(data_rom)/(K))
  u_rom = np.reshape(data_rom,(nt_rom,K) )
  u_rom = np.einsum('ij,nj->ni',basis,u_rom) + yRef
  u_rom = np.reshape(u_rom,(nt_rom,nx,ny,3))
  t_rom = np.linspace(0,rom_final_time,nt_rom)

  fig, ax = plt.subplots()
  ax.plot(t_fom, u_fom[:,int(nx/4),int(ny/4), 0],'or',mfc='None',label='Truth')
  ax.plot(t_rom, u_rom[:,int(nx/4),int(ny/4), 0],lw=1.8,color='y',label='hr-LSPG')
  ax.set_xlabel(r'$t$',**axis_font)
  ax.set_ylabel(r'$h(1.25,1.25,t)$',**axis_font)
  leg = plt.legend(loc=1, fancybox=True, framealpha=0)
  edit_figure_for_web(ax, leg)
  plt.tight_layout()
  fig.savefig('result.png', format="png", bbox_inches='tight', dpi=250, transparent=True)
  plt.close("all")
