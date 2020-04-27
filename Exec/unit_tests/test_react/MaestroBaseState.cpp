#include <Maestro.H>
#include <Maestro_F.H>

using namespace amrex;

void 
Maestro::InitBaseState(BaseState<Real>& rho0, BaseState<Real>& rhoh0, 
                       BaseState<Real>& p0, 
                       const int lev)
{
    // timer for profiling
    BL_PROFILE_VAR("Maestro::InitBaseState()", InitBaseState); 

    const int max_lev = base_geom.max_radial_level + 1;
    
    for (auto i = 0; i < base_geom.nr_fine; ++i) {
        for (auto n = 0; n < Nscal; ++n) {
            s0_init[lev+max_lev*(i+base_geom.nr_fine*n)] = 0.0;
        }
        rho0(lev,i) = 0.0;
        rhoh0(lev,i) = 0.0;
        tempbar(lev,i) = 0.0;
        tempbar_init(lev,i) = 0.0;
        p0(lev,i) = 0.0;
        p0_init(lev,i) = 0.0;
    }

    // initialize any inlet BC parameters
    SetInletBCs();
}
