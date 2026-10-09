/* The polar crate kinds' constructors, for include/vehicle.hpp, with no
 * include guard: vehicle.hpp includes them as inline functions
 * (POLAR_CRATE_CTOR `inline`), which CreateActor (actor_factory.cpp)
 * expands; polar_crates.cpp includes them at its end with
 * POLAR_CRATE_CTOR empty, for the ROM's out-of-line copies
 * (CreatePolarTimeCrate, ...; none has a caller). Each is PolarCrate's
 * constructor (InitPolarCrate), then the kind's vtable. */

POLAR_CRATE_CTOR PolarTimeCrate::PolarTimeCrate(const struct anim_table_record *rec, s32 x, s32 y,
                                                s32 z)
    : PolarCrate(rec, x, y, z)
{
}

POLAR_CRATE_CTOR PolarQuestionCrate::PolarQuestionCrate(const struct anim_table_record *rec, s32 x,
                                                        s32 y, s32 z)
    : PolarCrate(rec, x, y, z)
{
}

POLAR_CRATE_CTOR PolarAkuAkuCrate::PolarAkuAkuCrate(const struct anim_table_record *rec, s32 x,
                                                    s32 y, s32 z)
    : PolarCrate(rec, x, y, z)
{
}

POLAR_CRATE_CTOR PolarNitroCrate::PolarNitroCrate(const struct anim_table_record *rec, s32 x, s32 y,
                                                  s32 z)
    : PolarCrate(rec, x, y, z)
{
}

POLAR_CRATE_CTOR PolarLifeCrate::PolarLifeCrate(const struct anim_table_record *rec, s32 x, s32 y,
                                                s32 z, struct actor_spawn *spawn)
    : PolarCrate(rec, x, y, z)
{
    this->spawn = spawn;
}

POLAR_CRATE_CTOR PolarFourWumpaCrate::PolarFourWumpaCrate(const struct anim_table_record *rec,
                                                          s32 x, s32 y, s32 z)
    : PolarCrate(rec, x, y, z)
{
}

POLAR_CRATE_CTOR PolarBasicCrate::PolarBasicCrate(const struct anim_table_record *rec, s32 x, s32 y,
                                                  s32 z)
    : PolarCrate(rec, x, y, z)
{
}
