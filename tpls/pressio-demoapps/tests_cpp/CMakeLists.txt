
set(MESHSRC ${CMAKE_CURRENT_SOURCE_DIR}/../meshing_scripts)

# note that eigen is inside tpls because the code inside
# pressio-demoapps directly depends on it.
# pressio instead is placed here, inside the tests,
# because the tests rely on pressio for doing time integration
# but the actual demoapps implementation does not depend on it
# so it is better to keep things clean this way.

include_directories(
  # include eigen, pressio-demoapps headers, and pressio
  ${CMAKE_CURRENT_SOURCE_DIR}/../tpls/eigen/eigen3
  ${CMAKE_CURRENT_SOURCE_DIR}/../include
  ${CMAKE_CURRENT_SOURCE_DIR}/pressio/include)

# ---------------------------------------------------------
# various
# ---------------------------------------------------------
add_subdirectory(eigen_mesh_ccu_class)
add_subdirectory(eigen_mesh_ccu_special_cells_list)
add_subdirectory(weno3)
add_subdirectory(weno5)
add_subdirectory(rankine_hugoniot)
add_subdirectory(eigen_rusanov_flux_jacobians_euler)
add_subdirectory(eigen_rusanov_flux_jacobians_swe)

# ---------------------------------------------------------
# test jacobians, compare against FD or complex step
# ---------------------------------------------------------
add_subdirectory(eigen_1d_linear_advection_default_velocity_jacobian_fd)
add_subdirectory(eigen_1d_linear_advection_custom_velocity_jacobian_fd)
add_subdirectory(eigen_1d_diffusion_reaction_jacobian_fd)
add_subdirectory(eigen_1d_euler_jacobian_fd)
add_subdirectory(eigen_2d_diffusion_reaction_jacobian_fd)
add_subdirectory(eigen_2d_gray_scott_jacobian_fd)
add_subdirectory(eigen_2d_euler_jacobian_fd_x_neumann_y_neumann)
add_subdirectory(eigen_2d_euler_jacobian_fd_x_neumann_y_reflective)
add_subdirectory(eigen_2d_euler_jacobian_fd_x_neumann_y_ref_and_dirich)
add_subdirectory(eigen_2d_euler_cross_shock_jacobian_fd)
add_subdirectory(eigen_2d_swe_jacobian_fd_slip_wall)
add_subdirectory(eigen_2d_swe_jacobian_fd_with_sample_mesh)
add_subdirectory(eigen_2d_burgers_periodic_jacobian_fd)
add_subdirectory(eigen_3d_euler_jacobian_fd_xyz_periodic)
add_subdirectory(eigen_3d_euler_jacobian_fd_sedov_symm)

# ---------------------------------------------------------
# 1d problems
# ---------------------------------------------------------
add_subdirectory(eigen_1d_linear_advection_default_velocity_explicit)
add_subdirectory(eigen_1d_linear_advection_default_velocity_implicit)
add_subdirectory(eigen_1d_linear_advection_default_velocity_convergence)
add_subdirectory(eigen_1d_linear_advection_default_velocity_sample_mesh_testA)
add_subdirectory(eigen_1d_linear_advection_default_velocity_sample_mesh_testB)
add_subdirectory(eigen_1d_diffusion_reaction_explicit)
add_subdirectory(eigen_1d_diffusion_reaction_implicit)
add_subdirectory(eigen_1d_diffusion_reaction_sample_mesh_test)
add_subdirectory(eigen_1d_euler_convergence)
add_subdirectory(eigen_1d_euler_sod_explicit)
add_subdirectory(eigen_1d_euler_sod_implicit)
add_subdirectory(eigen_1d_euler_lax_explicit)
add_subdirectory(eigen_1d_euler_shu_osher_explicit)

# ---------------------------------------------------------
# 2d problems
# ---------------------------------------------------------
add_subdirectory(eigen_2d_diffusion_reaction_explicit)
add_subdirectory(eigen_2d_diffusion_reaction_sample_mesh_test)
add_subdirectory(eigen_2d_euler_smooth_explicit)
add_subdirectory(eigen_2d_euler_smooth_implicit)
add_subdirectory(eigen_2d_euler_normal_shock_explicit)
add_subdirectory(eigen_2d_euler_normal_shock_implicit)
add_subdirectory(eigen_2d_euler_riemann_explicit)
add_subdirectory(eigen_2d_euler_sedov_explicit)
add_subdirectory(eigen_2d_euler_sedov_implicit)
add_subdirectory(eigen_2d_euler_sedov_symmetry_explicit)
add_subdirectory(eigen_2d_euler_sedov_symmetry_implicit)
add_subdirectory(eigen_2d_euler_sample_mesh_test1)
add_subdirectory(eigen_2d_euler_double_mach_reflection_explicit)
add_subdirectory(eigen_2d_euler_double_mach_reflection_implicit)
add_subdirectory(eigen_2d_euler_kelvin_helmholtz_explicit)
add_subdirectory(eigen_2d_euler_cross_shock_explicit)
add_subdirectory(eigen_2d_swe_slip_wall_explicit)
add_subdirectory(eigen_2d_swe_sample_mesh_test1)
add_subdirectory(eigen_2d_gray_scott_explicit)
add_subdirectory(eigen_2d_gray_scott_sample_mesh_test)
add_subdirectory(eigen_2d_burgers_periodic_explicit)
add_subdirectory(eigen_2d_burgers_periodic_sample_mesh_test)

# ---------------------------------------------------------
# 3d problems
# ---------------------------------------------------------
add_subdirectory(eigen_3d_euler_smooth)
add_subdirectory(eigen_3d_euler_sedov_symmetry)
