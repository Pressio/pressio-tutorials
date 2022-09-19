#!/usr/bin/env python3

import matplotlib.pyplot as plt
from matplotlib import cm
import numpy as np
import re, os

def extractMeshSize(ns):
  reg = re.compile(r''+ns+'.+')
  file1 = open('./fom_mesh/info.dat', 'r')
  strings = re.search(reg, file1.read())
  file1.close()
  assert(strings)
  return int(strings.group().split()[1])

def plotStateErrorOverTime(romDirs):
  # plot state error over time
  fig = plt.figure(1)
  for dirIt in romDirs:
    # extract how many modes
    modeCount = np.loadtxt(dirIt + "/rom_dofs_count.txt")
    print(modeCount)
    # load time errors
    errors = np.loadtxt(dirIt + "/errors_in_time.txt")
    plt.plot(errors[:,0], '-', label="modeCount = " + str(modeCount))

  plt.ylim((0,0.1))
  plt.legend()

def plotHeight(romDirs):
  nx = extractMeshSize('nx')
  ny = extractMeshSize('ny')
  fomTotDofs = nx*ny*3

  meshCoords = np.loadtxt('./fom_mesh/coordinates.dat', dtype=float)
  x, y = meshCoords[:,1], meshCoords[:,2]
  x = np.reshape(x, (ny,nx))
  y = np.reshape(y, (ny,nx))

  fig = plt.figure(1)
  ax = fig.add_subplot(1, 1, 1, projection='3d')
  fomD = np.fromfile("fom_test_runid_0/state_snapshots.bin")
  nt = int(np.size(fomD)/fomTotDofs)
  print("fomTest: nt = ", nt)
  fomD = np.reshape(fomD, (nt, fomTotDofs))
  h = fomD[-1,0::3]
  bounds = [np.min(h), np.max(h)]
  fomD = np.reshape(h, (ny,nx))
  surf = ax.plot_surface(x, y, fomD, cmap=cm.jet, \
                          vmin=bounds[0], vmax=bounds[1])
  ax.set_title("FOM", fontsize=20)
  ax.set_zlim((0.98, 1.06))
  fig.savefig("FOM.png", format="png", bbox_inches='tight', dpi=300)

  for i in [0,1]:
    fig  = plt.figure(i+2)
    ax   = fig.add_subplot(1, 1, 1, projection='3d')
    K    = np.loadtxt(romDirs[i] + "/rom_dofs_count.txt", dtype=int)
    romD = np.loadtxt(romDirs[i]+"/reconstructed.txt")
    romD = np.reshape(romD[0::3,-1], (ny,nx))
    surf = ax.plot_surface(x, y, romD, cmap=cm.jet, \
                           vmin=bounds[0], vmax=bounds[1])
    ax.set_title("ROM: # modes = " + str(K), fontsize=20)
    ax.set_zlim((0.98, 1.06))
    fig.savefig("ROM_"+str(K)+".png", format="png", bbox_inches='tight', dpi=300)

  plt.show()

if __name__== "__main__":
  # find all rom RUNs
  romDirs = [d for d in os.listdir(".") if "hyperreduced_galerkin" in d]
  assert len(romDirs)==2, "for this demo, should only have two runs"

  plotHeight(romDirs)
