/*
 * Thread test code.
 */
#include <types.h>
#include <lib.h>
#include <thread.h>
#include <synch.h>
#include <test.h>
#include <current.h>

#define NTHREADS  8

static struct semaphore *tsem = NULL;

static
void
init_sem(void)
{
	if (tsem==NULL) {
		tsem = sem_create("tsem", 0);
		if (tsem == NULL) {
			panic("threadtest: sem_create failed\n");
		}
	}
}


/*
 * This test has each of NTHREADS threads print a
 * distinct digit to stdout.
 */

static
void print_digits(void * junk, unsigned long num) { 
	 (void)junk;
	 int ch = '0' + num;
	 volatile int i;
    for (i = 0; i < 200; i++);
	 putch(ch);
	 V(tsem);
}


static
void
_runthreads()
{
	char name[16];
	int i, result;

	for (i=0; i<NTHREADS; i++) {
		snprintf(name, sizeof(name), "threadtest%d", i);
		result = thread_fork(name, NULL,
	              print_digits,			   
				     NULL, i);
		if (result) {
			panic("threadtest: thread_fork failed %s)\n", 
			      strerror(result));
		}
}

	for (i=0; i<NTHREADS; i++) {
		P(tsem);
	}
}


int
threadtest_fun(int nargs, char **args)
{
	(void)nargs;
	(void)args;

	init_sem();
	kprintf("Starting the fun thread test...\n");
   _runthreads();	
	kprintf("\nCustom thread test done.\n");

	return 0;
}


