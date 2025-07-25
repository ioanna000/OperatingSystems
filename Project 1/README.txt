NAMES:
	
	Ioanna Gianni 				gioanna@uth.gr    	2704
	Stamatia-Varvara Zafeiri 	zstamat@uth.gr     	2726
	Georgios Kapetanos			kapgeorgios@uth.gr  2737


COMPILATION INSTRUCTIONS:
	
	find_roots_lib:
		* Follow project's instuctions on 3.1 (pages 12-13) to patch the syscall function in the kernel
		* Copy arch/x86/include/generated/uapi/asm/unistd_64.h to /usr/include/x86_64-linux-gnu/asm/
		* In the directory where the file is located, compile using make
		* To use the syscall function via roots.c file run $ ./find_roots_lib 
	scheduler module:
		* In module's directory use make to compile module
		* To insert module in kernel run $ insmod <.ko file>
		* Follor project's instuctions on 2.2.2 (page 8) to enable team00_kyber scheduler
	sysfs module:
		* In module's directory use make to compile module
		* To insert module in kernel run $ insmod <.ko file>
		* Read sysfs file with $ sudo cat /sys/kernel/team00/find_roots

To display message in kernel logs use $ dmesg | tail
