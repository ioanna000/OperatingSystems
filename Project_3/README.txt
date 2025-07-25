Gianni Ioanna 2704(gioanna@uth.gr)
Kapetanos Georgios 2737 (kapgeorgios@uth.gr)
Zafeiri Stamatia Varvara 2726(zstamat@uth.gr)

Follow 3.1.1 instructions and enable SLOB allocator before compiling patched kernel, in order to compile successfully the defined syscalls.
Copy /usr/src/linux-5.4.86-dev/arch/x86/include/generated/uapi/asm/unistd_64.h to  /usr/include/x86_64-linux-gnu/asm and compile with "make" the provided userspace app.
