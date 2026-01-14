/**
 * End to end demo for solving 1D Burgers' equation using reduced-order
 * modeling with Pressio.
 *
 * MATHEMATICAL FORMULATION
 * =====================
 *
 * The one-dimensional Burgers' equation is a classic nonlinear PDE
 * combining advection and diffusion:
 *
 *   ∂u/∂t + u * ∂u/∂x = ν * ∂²u/∂x²                        [1]
 *
 * where:
 *   - u(x, t) is the velocity field (scalar in 1D)
 *   - ν (nu) is the kinematic viscosity parameter
 *   - The domain is periodic: x ∈ [0, L] with u(0,t) = u(L,t)
 *   - Initial condition: u(x, 0) = u₀(x)
 *
 * PHYSICAL INTERPRETATION
 * =======================
 *
 * Burgers' equation models the balance between:
 *
 *   1. CONVECTIVE TRANSPORT (left side): u*∂u/∂x
 *      A fluid parcel carries its momentum forward. Steep gradients
 *      can steepen further (nonlinear shock formation). This term is
 *      NONLINEAR and generates fine-scale structures.
 *
 *   2. VISCOUS DIFFUSION (right side): ν*∂²u/∂x²
 *      Friction smooths velocity gradients, dissipating kinetic
 *      energy. High viscosity => smooth, diffuse solution. Low
 *      viscosity => sharp fronts, potential shock layers.
 *
 * The competing dynamics (steepening vs smoothing) produce rich
 * behavior: traveling waves, shock-like structures, and energy
 * cascade from large to small scales before dissipation.
 *
 * SPATIAL DISCRETIZATION
 * ======================
 *
 * We discretize on a uniform grid x_i = i*Δx, i = 0,...,N-1:
 *
 *   du_i/dt = -1/2 * (u²_{i+1} - u²_{i-1}) / (2Δx)
 *           + ν * (u_{i+1} - 2u_i + u_{i-1}) / Δx²        [2]
 *
 * Using centered finite differences:
 *   - Convective term: -(1/2) * d(u²)/dx via flux form
 *   - Diffusive term: ν * ∂²u/∂x² via standard 3-point stencil
 *   - Periodicity: indices wrap (i+1 mod N, i-1 mod N)
 *
 * This discretization produces a system of N ODEs:
 *   du/dt = f(u, t)                                         [3]
 * which we call the Full-Order Model (FOM).
 *
 * REDUCED-ORDER MODELING WITH PRESSIO
 * ====================================
 *
 * Motivation: The FOM [3] has N equations (thousands/millions
 * in practical 2D/3D). We want to approximate u(t) ≈ ũ(t)
 * using M << N basis vectors, reducing computational cost by
 * orders of magnitude.
 *
 * POD Basis Construction:
 *   1. Simulate the FOM for representative parameters/ICs,
 *      collecting M_snapshots state vectors: {u^(1), u^(2), ..., u^(p)}
 *   2. Compute SVD: U ≈ U_r Σ_r V_r^T
 *      Columns of U_r are the r dominant POD modes (r << N)
 *   3. Basis truncation: keep modes explaining 99.999% of energy
 *
 * Reduced State Representation:
 *   ũ(t) = U_r * a(t) + u_mean                              [4]
 *
 * where a(t) ∈ ℝ^r are reduced coordinates (r coefficients)
 * and u_mean is the snapshot mean.
 *
 * Galerkin Projection (default ROM approach):
 *   Project equation [1] onto the r-dimensional subspace:
 *
 *   da/dt = U_r^T * f(U_r * a + u_mean, t)                  [5]
 *
 * This reduces N equations to r equations. Since r << N,
 * the ROM is dramatically cheaper than the FOM for
 * evaluation and sensitivity analysis.
 *
 * ACCURACY & LIMITATIONS
 * ======================
 *
 * The ROM accuracy depends on:
 *   - Basis quality: POD basis should capture dominant dynamics
 *   - Extrapolation risk: ROM is trained on training parameter
 *     ranges; testing outside these ranges may be inaccurate
 *   - Nonlinear effects: Strong nonlinearity (Burgers convection)
 *     can cause Galerkin ROM errors to accumulate in time
 *   - Viscosity regime: Low-ν (high Reynolds number) regimes
 *     with shock-like structures are harder to capture with
 *     a fixed POD basis (manifold non-linearity)
 *
 * WORKFLOW IN THIS CODE
 * =====================
 *
 * This tutorial demonstrates:
 *   1. Instantiate the FOM (class Burgers1dFom)
 *   2. Run FOM simulations for training parameter values
 *   3. Collect snapshots and compute POD basis
 *   4. Construct Galerkin ROM [5] with Pressio
 *   5. Run ROM on test parameters and compare against FOM
 *
 * Expected Result: ROM should deliver ~100-1000× speedup
 * with minimal error (< 1%) on test cases within training range.
 */

////////////////////////////////////////////////////////////////////////////////

/**
 * Pressio uses macros to enable features like logging and TPLs.
 * Typically, these would be set during configuration, but we will
 * define them explicitly here for demonstration purposes.
 *
 * Importantly, these macros should be defined BEFORE including any Pressio
 * headers.
 */
#define PRESSIO_ENABLE_LOGGING
#define PRESSIO_ENABLE_TPL_EIGEN

/**
 * Due to the hierarchical structure of Pressio, including
 * pressio/rom.hpp will also pull in all necessary dependencies
 * (including Eigen, since we defined the macro above).
 * The logging macros (from pressio-log) are also included here.
 */
#include <pressio/rom.hpp>

/**
 * We'll also need some helper functions along the way.
 */
#include "helpers.h"

/**
 * Pressio supports various linear algebra backends (such as
 * Eigen, Tpetra, and Kokkos) and provides a unified interface
 * with the pressio::ops library for common operations. In this
 * tutorial, we will use Eigen.
 */
using vector_t = Eigen::VectorXd;
using matrix_t = Eigen::MatrixXd;

///////////////////////////////////////////////////////////////////////////////
// Step 1: Define the Full-Order Model (FOM)
///////////////////////////////////////////////////////////////////////////////

/**
 * Pressio requires a specific API to be used for the FOM class. This API can
 * vary depending on the type of problem. For the 1D Burgers' equation,
 * we will use the API for a semi-discrete FOM for time-dependent problems of
 * the form:
 *
 *     d/dt y(t) = f(y, t).
 *
 * The various FOM APIs can be found in pressio/rom/concepts.hpp.
 *
 * According to the semi-discrete API, our FOM class must define the following:
 *
 *     1. Three core types
 *         - time_type, state_type, rhs_type
 *
 *     2. A method to create the RHS vector
 *         - createRhs() -> rhs_type
 *
 *     3. A method to compute the RHS
 *         - rhs(const state_type & u, time_type t, rhs_type & f) -> void
 *
 * And that's it!
 */
class Burgers1dFom
{
    public:
        // API Requirement: Core types
        using time_type  = double;
        using state_type = vector_t;
        using rhs_type   = vector_t;

    private:
        std::size_t N_{ };
        double dx_{ };
        double nu_{ };

    public:
        Burgers1dFom( std::size_t N, double domainLength, double viscosity )
        : N_( N ), dx_( domainLength / ( N ) ), nu_( viscosity )
        {
            assert( N_  >= 3 );
            assert( dx_ > 0.0 );
            assert( nu_ > 0.0 );
        }

        std::size_t N() const { return N_; }
        double dx()     const { return dx_; }
        double nu()     const { return nu_; }

        // API Requirement: createRhs() method
        rhs_type createRhs() const
        {
            return rhs_type::Zero( N_ );
        }

        // API Requirement: rhs(...) method
        void rhs(
            const state_type & u,
            time_type /*t*/,
            rhs_type & f
        ) const
        {
            assert( u.size() == N_ );
            assert( f.size() == N_ );

            // periodic indexing helpers
            auto ip = [ this ]( std::size_t i ){ return ( i + 1 ) % N_; };
            auto im = [ this ]( std::size_t i ){ return ( i + N_ - 1 ) % N_; };

            const double inv2dx = 1.0 / ( 2.0 * dx_ );
            const double invdx2 = 1.0 / ( dx_ * dx_ );

            for ( std::size_t i = 0; i < N_; ++i ) {
                const std::size_t iL = im( i );
                const std::size_t iR = ip( i );

                // convective term: -(1/2 d/dx (u^2)) using centered flux difference
                // d/dx (u^2) ~ (u_{i+1}^2 - u_{i-1}^2) / (2 dx)
                const double dudt_conv =
                    -0.5 * ( ( u[ iR ] * u[ iR ] ) - ( u[ iL ] * u[ iL ] ) ) * inv2dx;

                // viscous term: nu * u_xx using second-order centered FD
                const double dudt_diff =
                    nu_ * ( u[ iR ] - 2.0 * u[ i ] + u[ iL ] ) * invdx2;
                f[ i ] = dudt_conv + dudt_diff;
            }
        }
};

///////////////////////////////////////////////////////////////////////////////
// Step 2: Run the FOM to generate snapshots
////////////////////////////////////////////////////////////////////////////////

/**
 * Building and running the FOM doesn't require any Pressio utilities.
 * For this example, we'll define a simple Forward Euler time integrator
 * to advance the FOM in time and collect the snapshots that will be used
 * to build the ROM with Pressio.
 */
template <typename FomSystem>
matrix_t runFOM( FomSystem& fom, typename FomSystem::time_type startTime, int numSteps, double dt )
{
    // We will store each snapshot vector in a std::vector for now
    std::vector< vector_t > snapshots;

    // Compute the initial condition
    auto u = computeInitialCondition< FomSystem, vector_t >( fom );

    // Time integration loop (using Forward Euler)
    double t = startTime;
    for ( int step = 0; step < numSteps; ++step ) {
        snapshots.push_back( u );   // Store snapshot
        auto f = fom.createRhs();   // Create RHS vector
        fom.rhs( u, t, f );         // Compute RHS
        u += dt * f;                // Update solution
        t += dt;                    // Advance time
    }

    // Convert our vector of snapshot vectors into a matrix
    const std::size_t numSnapshots = snapshots.size();
    matrix_t snapshotMatrix( fom.N(), numSnapshots );
    for ( std::size_t i = 0; i < numSnapshots; ++i ) {
        snapshotMatrix.col( i ) = snapshots[ i ];
    }

    return snapshotMatrix;
}

///////////////////////////////////////////////////////////////////////////////
// Step 3: Build the ROM from the snapshot matrix
///////////////////////////////////////////////////////////////////////////////

/**
 * Now we use the Pressio ecosystem to construct a Galerkin ROM representation
 * with the snapshot matrix. As before, there are various APIs that we can meet
 * to use different types of ROMs. We'll start with a default Galerkin here,
 * but there are other options for features like hyper-reduction.
 */
template <typename FomSystem>
auto buildAndRunROM( const matrix_t& snapshots, FomSystem& fom, typename FomSystem::time_type startTime, int numSteps, double dt )
{
    // Use helper functions (helpers.h) to compute the trial subspace from snapshots
    // and initialize the reduced state by projecting the FOM initial condition.
    // Typically this would be done offline by the app using Pressio.
    auto trialSpace   = snapshots_to_trial_space< FomSystem, matrix_t, vector_t >( snapshots, fom );
    auto reducedState = trial_space_to_reduced_state< FomSystem, vector_t >( trialSpace, fom );

    // Now we can set up the time integration scheme and policy with Pressio
    auto stepScheme = pressio::ode::StepScheme::ForwardEuler;
    auto policy = pressio::ode::steps_fixed_dt(
        startTime,
        pressio::ode::StepCount( numSteps ),
        dt
    );

    // And finally build the default Galerkin ROM
    auto rom = pressio::rom::galerkin::create_unsteady_explicit_problem(
        stepScheme, trialSpace, fom
    );

    // Then we use Pressio to run the ROM time integration
    pressio::ode::advance( rom, reducedState, policy ); /* might need observer */

    // At this point, reducedState contains the ROM solution at final time.
    // We just have to reconstruct the full-order solution from the
    // reduced coordinates via the trial subspace.
    auto romSolution = trialSpace.createFullStateFromReducedState( reducedState );

    return romSolution;
}

///////////////////////////////////////////////////////////////////////////////
// Step 4: Compare FOM and ROM solutions
///////////////////////////////////////////////////////////////////////////////

/**
 * To compare the FOM and ROM solutions, we can compute the relative error
 * between the two solution vectors at the final timestep.
 */
double compareFomAndRom( const matrix_t& fomSolution, const matrix_t& romSolution )
{
    double error = ( fomSolution - romSolution ).norm() / fomSolution.norm();
    return error;
}


// Main driver
int main() {
    // Initialize the logger so we can see Pressio output
    // Change the log level to "debug" for more information
    PRESSIOLOG_INITIALIZE(pressiolog::LogLevel::info);

    using FomSystem = Burgers1dFom;
    using time_t = typename FomSystem::time_type;

    // 1. Create the FOM
    const std::size_t N = 100;          // Number of spatial points
    const double domainLength = 1.0;    // Domain length
    const double viscosity = 0.01;      // Viscosity parameter
    FomSystem fom( N, domainLength, viscosity );
    PRESSIOLOG_INFO( "1. Created FOM with N = {}, dx = {}, nu = {}", fom.N(), fom.dx(), fom.nu() );

    // 2. Run the FOM to generate a snapshot matrix
    const double dt    = 0.001;
    const int numSteps = 1000;
    time_t startTime   = 0.0;
    matrix_t snapshotMatrix = runFOM< FomSystem >( fom, startTime, numSteps, dt );
    PRESSIOLOG_INFO( "2. Generated snapshot matrix with {} snapshots", snapshotMatrix.cols() );

    // 3. Build the ROM from the snapshot matrix and run it
    auto romSolution = buildAndRunROM< FomSystem >( snapshotMatrix, fom, startTime, numSteps, dt );
    PRESSIOLOG_INFO( "3. Built and ran the ROM" );

    // 4. Compare ROM solution against FOM solution (the last snapshot)
    auto fomSolution = snapshotMatrix.col( snapshotMatrix.cols() - 1 );
    auto error = compareFomAndRom( fomSolution, romSolution );
    PRESSIOLOG_INFO( "4. Relative error between FOM and ROM: {}", error );

    // Finalize the logger
    PRESSIOLOG_FINALIZE();
    return 0;
}
