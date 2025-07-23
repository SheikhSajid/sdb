Registers offer us one of the best ways to inspect, poke, prod, manipulate, tinker, and generally mess around with the state of a running process.

## ARMv7 Registers

The ARMv7 CPU on iMX6 has 16 32‑bit [**general‑purpose registers (r0–r15)**](https://developer.arm.com/documentation/den0013/d/ARM-Processor-Modes-and-Registers/Registers) for integers, pointers, stack tracking, and passing state around.

**Linux on ARM follows the ARM EABI/AAPCS (the Arm Procedure Call Standard)**, which lays out calling conventions, ELF object formats, dynamic linking rules, etc. There’s a core spec plus ARM-specific supplements.

The AAPCS assigns the x32 GPRs like this:
- **r0–r3**: Caller‑saved; used to pass the first four arguments. r0 (and r1 for 64‑bit values) holds return values.
- **r4–r11**: Callee‑saved general registers.
- **r12 (ip)**: Intra‑procedure scratch register (call-clobbered).
- **r13 (sp)**: Stack pointer.
- **r14 (lr)**: Link register (stores return address).
- **r15 (pc)**: Program counter.

## Interacting with registers using `ptrace`

Ptrace can be used to interact with registers. `PTRACE_GETREGS` and `PTRACE_SETREGS` communicate through the `user_regs` structure, defined in `/usr/include/arm-linux-gnueabihf/sys/user.h`:

```C
struct user_regs
{
  unsigned long int uregs[18]; // why 18 elements if there are only 16 registers? Virtual registers?
};
```

`PTRACE_GETFPREGS` and `PTRACE_SETFPREGS` use the `user_fpregs` from the same header:

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

## Debug Registers

TODO
