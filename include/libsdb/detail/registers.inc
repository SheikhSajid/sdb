#ifdef DEFINE_REGISTER
#define GPR_SIZE (sizeof(((struct user_regs *)0)->uregs[0]))

#define GPR_OFFSET(reg) (offsetof(user, regs) + offsetof(user_regs, uregs) + (ARM_##reg) * GPR_SIZE))

#define DEFINE_GPR_32(name, dwarf_id) \
  DEFINE_REGISTER(name, dwarf_id, GPR_SIZE, GPR_OFFSET(name), \
                  register_type::gpr, register_format::uint)

DEFINE_GPR_32(r0,   0),
// DEFINE_GPR_32(r1,   1),
// DEFINE_GPR_32(r2,   2),
// DEFINE_GPR_32(r3,   3),
// DEFINE_GPR_32(r4,   4),
// DEFINE_GPR_32(r5,   5),
// DEFINE_GPR_32(r6,   6),
// DEFINE_GPR_32(r7,   7),
// DEFINE_GPR_32(r8,   8),
// DEFINE_GPR_32(r9,   9),
// DEFINE_GPR_32(r10, 10),
// DEFINE_GPR_32(r11, 11),
// DEFINE_GPR_32(r12, 12),
// DEFINE_GPR_32(sp,   13),
// DEFINE_GPR_32(lr,   14),
// DEFINE_GPR_32(pc,   15),
// DEFINE_GPR_32(cpsr, 16),

#endif
