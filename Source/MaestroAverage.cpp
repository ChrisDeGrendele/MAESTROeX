
#include <Maestro.H>

using namespace amrex;

// Given a multifab of data (phi), average down to a base state quantity, phibar.
// If we are in plane-parallel, the averaging is at constant height.  
// If we are spherical, then the averaging is done at constant radius.  

void Maestro::Average (const Vector<MultiFab>& phi,
                       Vector<Real>& phibar,
                       int comp)
{

    if (spherical == 0) {

        // planar case

        // phibar is dimensioned to "max_radial_level" so we must mimic that for phisum
        // so we can simply swap thie result with phibar
        Vector<Real> phisum((max_radial_level+1)*nr_fine,0.0);

        // this stores how many cells there are laterally at each level
        Vector<int> ncell(max_radial_level+1);
        for (int lev=0; lev<=finest_level; ++lev) {
            const Box domainBox = geom[0].Domain();

            if (AMREX_SPACEDIM==2) {
                ncell[lev] = domainBox.bigEnd(0)+1;
            }
            else if (AMREX_SPACEDIM==3) {
                ncell[lev] = (domainBox.bigEnd(0)+1)*(domainBox.bigEnd(1)+1);
            }
        }

        // loop is over the existing levels (up to finest_level)
        for (int lev=0; lev<=finest_level; ++lev) {

            // get references to the MultiFabs at level lev
            const MultiFab& phi_mf = phi[lev];

            // Loop over boxes (make sure mfi takes a cell-centered multifab as an argument)
            for ( MFIter mfi(phi_mf); mfi.isValid(); ++mfi )
            {

                // Get the index space of the valid region
                const Box& validBox = mfi.validbox();

                // call fortran subroutine
                // use macros in AMReX_ArrayLim.H to pass in each FAB's data, 
                // lo/hi coordinates (including ghost cells), and/or the # of components
                // We will also pass "validBox", which specifies the "valid" region.
                average(lev, ARLIM_3D(validBox.loVect()), ARLIM_3D(validBox.hiVect()),
                        BL_TO_FORTRAN_N_3D(phi_mf[mfi],comp),
                        phisum.dataPtr());
            }
        }

        // reduction over boxes to get sum
        ParallelDescriptor::ReduceRealSum(phisum.dataPtr(),(max_radial_level+1)*nr_fine);

        // divide phisum by ncell so it stores "phibar"
        divide_phisum_by_ncell(phisum.dataPtr(),ncell.dataPtr());

        // swap pointers so phibar contains the computed average
        std::swap(phisum,phibar);

    }
    else {
        // spherical case
        amrex::Abort("Average does not work for spherical yet.");
    }


}