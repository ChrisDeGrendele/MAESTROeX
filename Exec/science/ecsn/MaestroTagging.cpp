
#include <Maestro.H>
#include <Maestro_F.H>

using namespace amrex;

void Maestro::RetagArray(const Box& bx, const int lev) {
    // timer for profiling
    BL_PROFILE_VAR("Maestro::RetagArray()", RetagArray);

    Abort("Error: RetagArray should not be called for spherical");
}

void Maestro::TagBoxes(TagBoxArray& tags, const MFIter& mfi, const int lev,
                       const Real time) {
    // timer for profiling
    BL_PROFILE_VAR("Maestro::TagBoxes()", TagBoxes);

    Abort("Error: TagBoxes should not be called for spherical");
}

void Maestro::StateError(TagBoxArray& tags, const MultiFab& state_mf,
                         const MFIter& mfi, const int lev, const Real time) {
    // timer for profiling
    BL_PROFILE_VAR("Maestro::StateError()", StateError);

    // Tag on regions of high temperature

    const Array4<char> tag = tags.array(mfi);
    const Array4<const Real> state = state_mf.array(mfi);

    const Box& tilebox = mfi.tilebox();

    // Tag on regions of high temperature
    ParallelFor(tilebox, [=] AMREX_GPU_DEVICE(int i, int j, int k) {
        if (state(i, j, k, Temp) >= 7.e8 || (i == 0 && j == 0 && k == 0)) {
            tag(i, j, k) = TagBox::SET;
        }
    });
}