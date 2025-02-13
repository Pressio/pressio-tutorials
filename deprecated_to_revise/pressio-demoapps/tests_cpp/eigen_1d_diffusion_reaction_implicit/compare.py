
import numpy as np
import sys, os
import matplotlib.pyplot as plt
from matplotlib import cm

if __name__== "__main__":
  nx=100
  fomTotDofs = nx*1

  D = np.fromfile("1d_diffreac_solution.bin")
  nt = int(np.size(D)/fomTotDofs)
  D = np.reshape(D, (nt, fomTotDofs))
  D = D[-1, :]
  np.savetxt("field.txt", D)
  goldD = np.loadtxt("gold.txt")

  assert(np.allclose(D.shape, goldD.shape))
  assert(np.isnan(D).all() == False)
  assert(np.isnan(goldD).all() == False)
  assert(np.allclose(D, goldD,rtol=1e-9, atol=1e-11))
