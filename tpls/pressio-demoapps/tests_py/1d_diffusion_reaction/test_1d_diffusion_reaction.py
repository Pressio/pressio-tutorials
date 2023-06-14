
import pathlib, sys, math
file_path = pathlib.Path(__file__).parent.absolute()

import matplotlib.pyplot as plt
import numpy as np
from numpy import linalg as LA
import pressiodemoapps as pda

def test_default_coefficients():
  meshPath = str(file_path)+"/mesh"
  meshO    = pda.load_cellcentered_uniform_mesh(meshPath)
  probId   = pda.DiffusionReaction1d.ProblemA
  appObj   = pda.create_problem(meshO, probId)

  yn = appObj.initialCondition()
  dt = 0.001
  Nsteps = 1000
  pda.advanceRK4(appObj, yn, dt, Nsteps)
  goldD = np.loadtxt(str(file_path)+"/gold_1.txt")
  assert(np.allclose(yn.shape, goldD.shape))
  assert(np.isnan(yn).all() == False)
  assert(np.isnan(goldD).all() == False)
  assert(np.allclose(yn, goldD,rtol=1e-9, atol=1e-11))

def test_custom_diffusion_and_reaction():
  meshPath = str(file_path)+"/mesh"
  meshO    = pda.load_cellcentered_uniform_mesh(meshPath)
  appObj   = pda.create_diffusion_reaction_1d_problem_A(meshO, 0.01, 0.005)

  yn = appObj.initialCondition()
  dt, Nsteps = 0.001, 1000
  pda.advanceRK4(appObj, yn, dt, Nsteps)
  goldD = np.loadtxt(str(file_path)+"/gold_2.txt")
  assert(np.allclose(yn.shape, goldD.shape))
  assert(np.isnan(yn).all() == False)
  assert(np.isnan(goldD).all() == False)
  assert(np.allclose(yn, goldD,rtol=1e-9, atol=1e-11))


def test_custom_source_diffusion_and_reaction():

  mysource = lambda x, time : np.sin(math.pi*x) *x*x * 4.*np.cos(4.*math.pi*x)

  meshPath = str(file_path)+"/mesh"
  meshO    = pda.load_cellcentered_uniform_mesh(meshPath)
  appObj   = pda.create_diffusion_reaction_1d_problem_A(meshO, mysource, 0.01, 0.005)

  yn = appObj.initialCondition()
  dt, Nsteps = 0.001, 1000
  pda.advanceRK4(appObj, yn, dt, Nsteps)
  goldD = np.loadtxt(str(file_path)+"/gold_2.txt")
  assert(np.allclose(yn.shape, goldD.shape))
  assert(np.isnan(yn).all() == False)
  assert(np.isnan(goldD).all() == False)
  assert(np.allclose(yn, goldD,rtol=1e-9, atol=1e-11))

#----------------------------
if __name__ == '__main__':
#----------------------------
  test_default_coefficients()
  test_custom_diffusion_and_reaction()
  test_custom_source_diffusion_and_reaction()
