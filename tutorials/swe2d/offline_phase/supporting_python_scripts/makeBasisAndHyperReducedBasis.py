#!/usr/bin/env python3

import copy
import scipy.linalg
import numpy as np
import pressiotools.linalg as ptla
import matplotlib.pyplot as plt
np.random.seed(1)
def get_gid_from_ij(i,j):
  return int( (j%ny)*nx + i%nx )

def get_ij_from_gid(gid):
  j = int(gid)/int(nx)
  i = gid%nx
  return int( i), int(j)

if __name__== "__main__":
  nx = 128
  ny = 128
  N_cell = nx*ny
  N_sample = 100
  nBasis = 10

  def pod(S):
    #construct svd obect
    svdO = ptla.Svd()
    dim1,dim2 = np.shape(S)
    if (dim1 <= dim2):
      #compute the thin svd
      svdO.computeThin( ptla.MultiVector(np.asfortranarray(S)))
      U = svdO.viewLeftSingVectorsLocal()
      return U[:,0:nBasis]
    else:
     Kern = np.dot(S.transpose(),S)
     svdO.computeThin( ptla.MultiVector(np.asfortranarray(Kern)))
     sigma = np.sqrt(svdO.viewSingValues())
     u = svdO.viewLeftSingVectorsLocal()
     U = np.dot(S,1./sigma*u)
     return U[:,0:nBasis]   

  print('Building basis!')
  snapshots = np.zeros((0,3*nx*ny))
  for i in range(0,9):
    data = np.fromfile('../solution' + str(i) + '.bin',dtype='float64')
    nt = int(np.size(data)/(nx*ny*3))
    ulocal = np.reshape(data,(nt,3*nx*ny) )
    snapshots = np.append(snapshots,ulocal,axis=0)

  nsnaps = int(np.size(snapshots)/(nx*ny*3))
  snapshots = np.reshape(snapshots,(nsnaps,nx*ny,3))
  PhiA = [None]*3
  for i in range(0,3):
    snapshots_l = np.rollaxis(snapshots[:,:,i],1)
    print('Performing SVD of matrix of size ' + str(np.shape(snapshots_l)))
    PhiA[i] = pod(snapshots_l)

  K1 = np.shape(PhiA[0])[1]
  K2 = np.shape(PhiA[1])[1]
  K3 = np.shape(PhiA[2])[1]
  Phi = np.zeros((3*nx*ny,K1+K2+K3))
  Phi[0::3,0:K1] = PhiA[0]
  Phi[1::3,K1: K1 + K2] = PhiA[1]
  Phi[2::3,K1+K2: K1 + K2 + K3] = PhiA[2]
  np.savetxt('basis.txt',Phi,delimiter=' ')
  print('Done!')

  print('===================')
  print('Making sample mesh!')
  #create random list of cells
  method = 'qsampling'
  sample_mesh = np.zeros(N_sample,dtype='int')
  if method == 'random':
    mesh_inds = np.array(range(0,N_cell),dtype='int')#np.random.randint(N_cell,size=N_sample)
    shuffle(mesh_inds)
    sample_mesh[:] = mesh_inds[0:N_sample]

  if method == 'qsampling':
    snapshots = np.zeros((0,3*nx*ny))
    for i in range(0,9):
      data = np.fromfile('../solution' + str(i) + '.bin',dtype='float64')
      nt = int(np.size(data)/(nx*ny*3))
      ulocal = np.reshape(data,(nt,3*nx*ny) )
      snapshots = np.append(snapshots,ulocal,axis=0)

    nsnaps = int(np.size(snapshots)/(nx*ny*3))
    snapshots = np.reshape(snapshots,(nsnaps,nx*ny,3))
    snapshots_h = np.rollaxis(snapshots[:,:,-1],1)
    U,dum,dum = np.linalg.svd(snapshots_h,full_matrices=False)
    Q,R,P = scipy.linalg.qr(U[:,0:N_sample].transpose(),pivoting=True )
    sample_mesh[:] = P[0:N_sample]

  sample_mesh_plus_stencil = copy.deepcopy(sample_mesh)
  # add stencil to sample mesh
  for gid in sample_mesh:
    i,j = get_ij_from_gid(gid)
    gid1 = get_gid_from_ij(i-1,j)
    gid2 = get_gid_from_ij(i+1,j)
    gid3 = get_gid_from_ij(i,j-1)
    gid4 = get_gid_from_ij(i,j+1)
    sample_mesh_plus_stencil = np.append(sample_mesh_plus_stencil,gid1)
    sample_mesh_plus_stencil = np.append(sample_mesh_plus_stencil,gid2)
    sample_mesh_plus_stencil = np.append(sample_mesh_plus_stencil,gid3)
    sample_mesh_plus_stencil = np.append(sample_mesh_plus_stencil,gid4)

  sample_mesh_plus_stencil = np.unique(sample_mesh_plus_stencil)
  print('Done! Now saving to file')
  np.savetxt('sample_mesh_gids.txt',sample_mesh,fmt='%i')
  np.savetxt('sample_mesh_plus_stencil_gids.txt',sample_mesh_plus_stencil,fmt='%i')

  print("sample mesh is of size " + str(np.size(sample_mesh)))
  print("sample mesh with stencil is of size " + str(np.size(sample_mesh_plus_stencil)))

  ## make basis at sample mesh
  PhiSampleRho  = Phi[0 + 3*sample_mesh_plus_stencil,:]
  PhiSampleRhoU = Phi[1 + 3*sample_mesh_plus_stencil,:]
  PhiSampleRhoE = Phi[2 + 3*sample_mesh_plus_stencil,:]
  PhiSamplePlusStencil = np.zeros(( int(np.size(sample_mesh_plus_stencil)*3), np.shape(Phi)[1]))
  PhiSamplePlusStencil[0::3,:] =  Phi[0 + 3*sample_mesh_plus_stencil,:]
  PhiSamplePlusStencil[1::3,:] =  Phi[1 + 3*sample_mesh_plus_stencil,:]
  PhiSamplePlusStencil[2::3,:] =  Phi[2 + 3*sample_mesh_plus_stencil,:]

  np.savetxt('PhiSamplePlusStencil.txt',PhiSamplePlusStencil)
  rel_indx = np.zeros( int(np.size(sample_mesh)), dtype='int')
  for i in range(0, int(np.size(rel_indx))):
    rel_indx[i] = np.argmin(np.abs( sample_mesh[i] - sample_mesh_plus_stencil) )

  np.savetxt('sample_mesh_relative_indices.txt',rel_indx,fmt='%i')

  info_file_array = np.array([np.shape(Phi)[1],
                              int(np.size(sample_mesh)),
                              int(np.size(sample_mesh_plus_stencil))])
  np.savetxt('info_file.txt',info_file_array,fmt='%i')



  ## create plot for sample mesh (here just re-read in files so this snippet of code can be copied to other locations)
  sm_ids = np.genfromtxt('sample_mesh_gids.txt',dtype='int')
  sm_ids_plot = np.zeros((nx*ny))
  sm_ids_plot[sm_ids] = 1
  sm_ids = np.reshape(sm_ids_plot,(nx,ny))

  smps_ids = np.genfromtxt('sample_mesh_plus_stencil_gids.txt',dtype='int')
  smps_ids_plot = np.zeros((nx*ny))
  smps_ids_plot[smps_ids] = 1
  smps_ids = np.reshape(smps_ids_plot,(nx,ny))


  plt.spy(smps_ids,marker='s',markersize=1.5,color='red')
  plt.spy(sm_ids,marker='s',markersize=1.5,color='black')
  plt.xlabel(r'$x$ index')
  plt.ylabel(r'$y$ index')

  plt.savefig('samplemesh.png', format="png", bbox_inches='tight', dpi=300)



  ### convert final solution file from binary to .txt for docker
  #data_fom = np.fromfile('solution100.bin')
  #np.savetxt('solution100_fom.txt',data_fom)
  #data_fom_nn = np.fromfile('solution4.bin')
  #np.savetxt('solution_nn.txt',data_fom_nn)

