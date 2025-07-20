# Implementation

`sdb.cpp` uses the `ptrace` system call to implement basic debugging functionality. Here is how `ptrace` is used:

- **Attaching to an Existing Process:**  
  When the user runs `sdb -p <pid>`, the program calls `ptrace(PTRACE_ATTACH, pid, nullptr, nullptr)` to attach to the running process with the given PID. This allows `sdb` to control and inspect the target process.

- **Tracing a New Process:**  
  When the user runs `sdb <program>`, the debugger forks a child process. In the child, it calls `ptrace(PTRACE_TRACEME, 0, nullptr, nullptr)` before executing the target program. This notifies the kernel that the child should be traced by its parent.

- **Continuing Execution:**  
  When the user enters the `continue` command, `sdb` calls `ptrace(PTRACE_CONT, pid, nullptr, nullptr)` to resume the execution of the stopped process.

- **Waiting for Signals:**  
  After attaching or continuing, `sdb` uses `waitpid` to wait for the traced process to stop on signals (such as breakpoints).

## Relevant Signals for Debuggers

When using `ptrace`, several signals are particularly relevant for debuggers:

- **SIGTRAP**: Sent to the tracee when a breakpoint is hit, after a `PTRACE_ATTACH`, or after certain ptrace events.
- **SIGSTOP**: Used to stop a process; the debugger may send this to pause execution.
- **SIGCONT**: Used to continue a stopped process.
- **SIGSEGV**: Indicates a segmentation fault (invalid memory access).
- **SIGILL**: Indicates an illegal instruction was executed.
- **SIGFPE**: Indicates an erroneous arithmetic operation, such as division by zero.
- **SIGCHLD**: Sent to the parent when a child process stops or terminates.
- **SIGINT**: Sent to interrupt a process (e.g., Ctrl+C in terminal).

A debugger typically monitors these signals to detect events such as breakpoints, exceptions, and process state changes.

## Signals available on the iMX Cube

```
root@debian 0+ ~/sdb master 
 # kill -l
 1) SIGHUP       2) SIGINT       3) SIGQUIT      4) SIGILL       5) SIGTRAP
 6) SIGABRT      7) SIGBUS       8) SIGFPE       9) SIGKILL     10) SIGUSR1
11) SIGSEGV     12) SIGUSR2     13) SIGPIPE     14) SIGALRM     15) SIGTERM
16) SIGSTKFLT   17) SIGCHLD     18) SIGCONT     19) SIGSTOP     20) SIGTSTP
21) SIGTTIN     22) SIGTTOU     23) SIGURG      24) SIGXCPU     25) SIGXFSZ
26) SIGVTALRM   27) SIGPROF     28) SIGWINCH    29) SIGIO       30) SIGPWR
31) SIGSYS      34) SIGRTMIN    35) SIGRTMIN+1  36) SIGRTMIN+2  37) SIGRTMIN+3
38) SIGRTMIN+4  39) SIGRTMIN+5  40) SIGRTMIN+6  41) SIGRTMIN+7  42) SIGRTMIN+8
43) SIGRTMIN+9  44) SIGRTMIN+10 45) SIGRTMIN+11 46) SIGRTMIN+12 47) SIGRTMIN+13
48) SIGRTMIN+14 49) SIGRTMIN+15 50) SIGRTMAX-14 51) SIGRTMAX-13 52) SIGRTMAX-12
53) SIGRTMAX-11 54) SIGRTMAX-10 55) SIGRTMAX-9  56) SIGRTMAX-8  57) SIGRTMAX-7
58) SIGRTMAX-6  59) SIGRTMAX-5  60) SIGRTMAX-4  61) SIGRTMAX-3  62) SIGRTMAX-2
63) SIGRTMAX-1  64) SIGRTMAX
```

## Handling Multiple Threads

When a **tracee** has multiple threads, each thread is represented by a unique **thread ID (TID)** in Linux. The `ptrace` system call operates at the thread level, meaning each thread can be individually traced.

- To trace all threads, **the debugger must attach to each thread separately** using `ptrace(PTRACE_ATTACH, tid, ...)` for every thread ID.
- When a new thread is created in the traced process, the debugger can detect this by monitoring for `SIGTRAP` signals and using the `/proc/<pid>/task/` directory to enumerate thread IDs.
- Each thread can be continued, single-stepped, or inspected independently by issuing `ptrace` commands with the appropriate TID.

**Note:**  
The current implementation of `sdb.cpp` **does not explicitly handle multiple threads**. To support multi-threaded debugging, `sdb` would need to detect and attach to new threads as they are created, and manage events from all threads in the debugged process.
