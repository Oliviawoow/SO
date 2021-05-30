/*
 * Loader Implementation
 *
 * 2018, Operating Systems
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include "exec_parser.h"
#include "utils.h"

#define PAGE_SIZE 4096

static so_exec_t *exec;
static struct sigaction old_action;
char *p;
int how[3] = { PROT_NONE, PROT_READ, PROT_WRITE };
int file_descriptor;

/* furat din lab 6 ex 5 */
static void segv_handler(int signum, siginfo_t *info, void *context)
{
	char *addr;
	int i = 0;
	int nr_seg;
	int found = 0;

	/* Calling the old signal handler by default */
	/* old_action.sa_sigaction(signum, info, context); */

	/* check if the signal is SIGSEGV */
	if (signum != SIGSEGV) {
		old_action.sa_sigaction(signum, info, context);
		return;
	}

	/* obtain from siginfo_t the memory location
	 * which caused the page fault
	 */
	addr = (char *)info->si_addr;

	nr_seg = exec->segments_no;
	while (i < nr_seg) {
		int diff;
		uintptr_t start;
		int size_seg;
		int page_index;
		int * data;
		int page_pos_seg;
		int offset;
		int permisions;

		data = exec->segments[i].data;
		start = exec->segments[i].vaddr;
		size_seg = exec->segments[i].mem_size;

		diff = (uintptr_t)addr - start;

		if (diff < size_seg) {
			found++;
			page_index = diff / PAGE_SIZE;
			page_pos_seg = page_index * PAGE_SIZE;

			/* page not mapped = 0 */
			if (data[page_index] == 0) {
				/* unspecified zone */
				if (page_pos_seg >
				exec->segments[i].file_size) {
					p = mmap((void *) start + page_index *
					PAGE_SIZE, PAGE_SIZE,
					exec->segments[i].perm,
					MAP_FIXED | MAP_ANONYMOUS |
					MAP_PRIVATE, -1, 0);
					DIE(p == (char *)-1, "mmap");
				} else {
					offset = exec->segments[i].offset +
					PAGE_SIZE * page_index;

					permisions = exec->segments[i].perm;

					p = mmap((void *) start + page_index *
					PAGE_SIZE, PAGE_SIZE,
					permisions, MAP_FIXED |
					MAP_PRIVATE, file_descriptor,
					offset);
					DIE(p == (char *)-1, "mmap");
				}
				data[page_index] = 1;
				return;
				/* page mapped = 1 */
			} else if (data[page_index] == 1) {
				old_action.sa_sigaction(signum, info, context);
				return;
			}
		}
		i++;
	}
	/* page not found */
	if (found == 0) {
		old_action.sa_sigaction(signum, info, context);
		return;
	}
}

/* furat din lab 6 ex 5 */
int so_init_loader(void)
{
	/* initialize on-demand loader */
	struct sigaction action;
	int rc;

	action.sa_sigaction = segv_handler;
	sigemptyset(&action.sa_mask);
	sigaddset(&action.sa_mask, SIGSEGV);
	action.sa_flags = SA_SIGINFO;

	rc = sigaction(SIGSEGV, &action, &old_action);
	DIE(rc == -1, "sigaction");
	return 0;
}

int so_execute(char *path, char *argv[])
{
	int i;
	int no_page;

	file_descriptor = open(path, O_RDONLY);
	DIE(file_descriptor < 0, "Error");

	exec = so_parse_exec(path);
	if (!exec)
		return -1;

	for (i = 0; i < exec->segments_no; ++i) {
		no_page = exec->segments[i].mem_size / PAGE_SIZE + 1;
		so_seg_t *segment = &(exec->segments[i]);

		segment->data = (int *)malloc(no_page * sizeof(int));
		memset(segment->data, 0, no_page);
	}

	so_start_exec(exec, argv);

	return -1;
}
