#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>

long our_kmalloc(size_t size) {
	return(syscall(__NR_user_kmalloc_syscall, size));
}
long our_kfree(void * ptr) {
	return(syscall(__NR_user_kfree_syscall, ptr));
}
long our_slob_get_total_alloc_mem() {
	return(syscall(__NR_slob_get_total_alloc_mem));
}
long our_slob_get_total_free_mem() {
	return(syscall(__NR_slob_get_total_free_mem));
}
