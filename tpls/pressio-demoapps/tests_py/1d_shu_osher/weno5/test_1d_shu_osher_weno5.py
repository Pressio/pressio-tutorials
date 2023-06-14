
import pathlib, sys
file_path = pathlib.Path(__file__).parent.absolute()

import numpy as np
from numpy import linalg as LA
import pressiodemoapps as pda

def test_run():
  meshPath = str(file_path)
  meshObj  = pda.load_cellcentered_uniform_mesh(meshPath)
  scheme   = pda.InviscidFluxReconstruction.Weno5
  appObj   = pda.create_problem(meshObj, pda.Euler1d.ShuOsher, scheme)

  yn = appObj.initialCondition()
  dt = 0.001
  Nsteps = int(1.8/dt)
  pda.advanceSSP3(appObj, yn, dt, Nsteps)
  goldD = np.loadtxt(str(file_path)+"/gold.txt")

  assert(np.allclose(yn.shape, goldD.shape))
  assert(np.isnan(yn).all() == False)
  assert(np.isnan(goldD).all() == False)
  assert(np.allclose(yn, goldD,rtol=1e-9, atol=1e-11))

  # import matplotlib.pyplot as plt
  # x = meshObj.viewX()
  # plt.plot(x, yn[0:-1:3])
  # plt.show()

if __name__ == '__main__':
  test_run()
