
import os, subprocess

# everything here MUST be defined

# -----------------------------------------------------------
dimensionality = 2
numDofsPerCell = 3
validParameterNames = ['coriolis', 'pulsemag', 'gravity']

# -----------------------------------------------------------
def inviscid_flux_string_to_stencil_size(stringIn):
  if stringIn == "FirstOrder":
    return 3
  elif stringIn == "Weno3":
    return 5
  elif stringIn == "Weno5":
    return 7
  else:
    sys.exit("Invalid scheme detected {}".format(scheme))
    return None

# -----------------------------------------------------------
def generate_full_mesh(pdaSrcPath, meshPath, dicIn):
  pdaMeshDir = pdaSrcPath + "/meshing_scripts"

  nx, ny = dicIn['meshSize'][0], dicIn['meshSize'][1]

  # figure out the stencil size needed for the mesh
  invFluxRec = dicIn['inviscidFluxReconstruction']
  stencilSize = inviscid_flux_string_to_stencil_size(invFluxRec)

  print('Generating mesh {}'.format(meshPath))
  # call script
  owd = os.getcwd()
  args = ("python3", pdaMeshDir + '/create_full_mesh.py',\
          "-n", str(nx), str(ny),\
          "--outDir", meshPath,\
          "--bounds", "-5.0", "5.0", "-5.0", "5.0",
          "-s", str(stencilSize))

  popen  = subprocess.Popen(args, stdout=subprocess.PIPE); popen.wait()
  output = popen.stdout.read();
