#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>
#include "memory_syscalls.h"
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
	int i;
	int *ptrs[100000];

	time_t t;

	srand((unsigned) time(&t));

	long alloc = our_slob_get_total_alloc_mem();
	long free = our_slob_get_total_free_mem();
	printf("size is: %ld\n", sizeof(long));
	printf("Allocated memory = %ld bytes\nFree memory = %ld bytes\nPercentage = %.2lf\n", our_slob_get_total_alloc_mem(), our_slob_get_total_free_mem(), ((double) free / (double) alloc) * 100);
	for(i = 0; i < 100000; i++) {
		ptrs[i] = (int*) our_kmalloc(((rand() % 50) + 1)* sizeof(int));
	}
	alloc = our_slob_get_total_alloc_mem();
	free = our_slob_get_total_free_mem();
	printf("Allocated memory = %ld bytes\nFree memory = %ld bytes\nPercentage = %.2lf\n", alloc, free, ((double)free / (double)alloc) * 100);
	for(i = 0; i < 10000; i++) {
		our_kfree(ptrs[i]);
	}
        printf("Allocated memory = %ld bytes\nFree memory = %ld bytes\n", our_slob_get_total_alloc_mem(), our_slob_get_total_free_mem());

	return 0;
}
