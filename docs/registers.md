Registers offer us one of the best ways to inspect, poke, prod, manipulate,
tinker, and generally mess around with the state of a running process.

## ARMv7 Registers

The ARMv7 CPU on iMX6 has 16 32‑bit [general‑purpose registers (r0–r15)](https://developer.arm.com/documentation/den0013/d/ARM-Processor-Modes-and-Registers/Registers)
for integers, pointers, stack tracking, and passing state around.

**Linux on ARM follows the ARM EABI/AAPCS (the Arm Procedure Call Standard)**,
which lays out calling conventions, ELF object formats, dynamic linking rules,
etc. There’s a core spec plus ARM-specific supplements.

The AAPCS assigns the x32 GPRs like this:
- **r0–r3**   : Caller‑saved; used to pass the first four arguments. r0 (and r1
                for 64‑bit values) holds return values.
- **r4–r11**  : Callee‑saved general registers.
- **r12 (ip)**: Intra‑procedure scratch register (call-clobbered).
- **r13 (sp)**: Stack pointer.
- **r14 (lr)**: Link register (stores return address).
- **r15 (pc)**: Program counter.

## Interacting with registers using `ptrace`

Ptrace can be used to interact with registers. `PTRACE_GETREGS` and `PTRACE_SETREGS`
communicate through the `user_regs` structure, defined in `/usr/include/arm-linux-gnueabihf/sys/user.h`:

```C
struct user_regs
{
  unsigned long int uregs[18]; // why 18 elements if there are only 16 registers? Virtual registers?
};
```

Unfortunately, for ARMv7 `user-regs` is defined as a single array instead
individual struct fields like in x86. We can get an idea of the individual
registers from `/usr/include/arm-linux-gnueabihf/asm/ptrace.h`.

```C
/*
 * This struct defines the way the registers are stored on the
 * stack during a system call.  Note that sizeof(struct pt_regs)
 * has to be a multiple of 8.
 */
struct pt_regs {
	long uregs[18];
};

#define ARM_cpsr	uregs[16]
#define ARM_pc		uregs[15]
#define ARM_lr		uregs[14]
#define ARM_sp		uregs[13]
#define ARM_ip		uregs[12]
#define ARM_fp		uregs[11]
#define ARM_r10		uregs[10]
#define ARM_r9		uregs[9]
#define ARM_r8		uregs[8]
#define ARM_r7		uregs[7]
#define ARM_r6		uregs[6]
#define ARM_r5		uregs[5]
#define ARM_r4		uregs[4]
#define ARM_r3		uregs[3]
#define ARM_r2		uregs[2]
#define ARM_r1		uregs[1]
#define ARM_r0		uregs[0]
#define ARM_ORIG_r0	uregs[17]
```
I am not sure if these definitions are meant for userspace use. There are quite
a few `ptrace.h` files throughout the `/usr/include` directory. Again, I do not
know why we need so many.

```bash
root@debian 0+ /usr/include  
$ find /usr/include -name "ptrace.h"
/usr/include/kernel-4.9.150-imx6-sr/linux/ptrace.h
/usr/include/kernel-4.9.150-imx6-sr/asm/ptrace.h
/usr/include/linux/ptrace.h
/usr/include/arm-linux-gnueabihf/sys/ptrace.h
/usr/include/arm-linux-gnueabihf/asm/ptrace.h
```

`PTRACE_GETFPREGS` and `PTRACE_SETFPREGS` use the `user_fpregs` from the same
header:

```C
struct user_fpregs
{
  struct fp_reg
  {
    unsigned int sign1:1;
    unsigned int unused:15;
    unsigned int sign2:1;
    unsigned int exponent:14;
    unsigned int j:1;
    unsigned int mantissa1:31;
    unsigned int mantissa0:32;
  } fpregs[8];
  unsigned int fpsr:32;
  unsigned int fpcr:32;
  unsigned char ftype[8];
  unsigned int init_flag;
};
```

## Describing Registers

The code in header `include/libsdb/register_info.hpp` captures several features
of each register: a unique enumerator to identify it in the code; its name; the
DWARF register number it is assigned in the [ARM ABI](https://github.com/ARM-software/abi-aa/blob/main/aadwarf32/aadwarf32.rst).

DWARF registers refer to the register numbering and naming scheme defined by the
**DWARF debugging standard**.

## Debug Registers

TODO
