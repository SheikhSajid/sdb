#ifdef DEFINE_REGISTER
// Compute the byte‐offset of register #idx in the target’s user struct.
// user.regs is a user_regs_struct, which on ARM is defined roughly as:
//   struct user_regs_struct { unsigned long uregs[18]; };
// so uregs[0] is r0, …, uregs[15] is pc, uregs[16] is cpsr. :contentReference[oaicite:0]{index=0}
#define GPR_OFFSET(idx) \
    (offsetof(user, regs)                           \
     + offsetof(user_regs, uregs)                   \
     + (idx) * sizeof(((user_regs*)0)->uregs[0]))

// Define a full-width (32-bit) register at index ‘idx’ with DWARF regnum ‘dwarf_id’.
#define DEFINE_GPR_32(name, dwarf_id, idx)               \
    DEFINE_REGISTER(                                     \
      name,                                              \
      idx,                                               \
      sizeof(((user_regs*)0)->uregs[0]),                 \
      GPR_OFFSET(idx),                                   \
      register_type::gpr,                                \
      register_format::uint)

DEFINE_GPR_32(r0,   0,  0),
// DEFINE_GPR_32(r1,   1,  1),
// DEFINE_GPR_32(r2,   2,  2),
// DEFINE_GPR_32(r3,   3,  3),
// DEFINE_GPR_32(r4,   4,  4),
// DEFINE_GPR_32(r5,   5,  5),
// DEFINE_GPR_32(r6,   6,  6),
// DEFINE_GPR_32(r7,   7,  7),
// DEFINE_GPR_32(r8,   8,  8),
// DEFINE_GPR_32(r9,   9,  9),
// DEFINE_GPR_32(r10, 10, 10),
// DEFINE_GPR_32(r11, 11, 11),
// DEFINE_GPR_32(r12, 12, 12),
// DEFINE_GPR_32(sp,  13, 13),  // stack pointer
// DEFINE_GPR_32(lr,  14, 14),  // link register
// DEFINE_GPR_32(pc,  15, 15),  // program counter
// DEFINE_GPR_32(cpsr,16, 16),  // current program status register :contentReference[oaicite:1]{index=1}

#endif
