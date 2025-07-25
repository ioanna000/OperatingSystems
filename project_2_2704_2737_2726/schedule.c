/* schedule.c
 * This file contains the primary logic for the 
 * scheduler.
 */
#include "schedule.h"
#include "macros.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "list.h"

#define NEWTASKSLICE (NS_TO_JIFFIES(100000000))

/* Local Globals
 * rq - This is a pointer to the runqueue that the scheduler uses.
 * current - A pointer to the current running task.
 */
struct runqueue *rq;
struct task_struct *current;

/* External Globals
 * jiffies - A discrete unit of time used for scheduling.
 *			 There are HZ jiffies in a second, (HZ is 
 *			 declared in macros.h), and is usually
 *			 1 or 10 milliseconds.
 */
extern long long jiffies;
extern struct task_struct *idle;

#define CONSIDER_WAIT true

/*-----------------Initilization/Shutdown Code-------------------*/
/* This code is not used by the scheduler, but by the virtual machine
 * to setup and destroy the scheduler cleanly.
 */
 
 /* initscheduler
  * Sets up and allocates memory for the scheduler, as well
  * as sets initial values. This function should also
  * set the initial effective priority for the "seed" task 
  * and enqueu it in the scheduler.
  * INPUT:
  * newrq - A pointer to an allocated rq to assign to your
  *			local rq.
  * seedTask - A pointer to a task to seed the scheduler and start
  * the simulation.
  */

void initschedule(struct runqueue *newrq, struct task_struct *seedTask)
{
	seedTask->next = seedTask->prev = seedTask;
	newrq->head = seedTask;
	newrq->nr_running++;
}


/* killschedule
 * This function should free any memory that 
 * was allocated when setting up the runqueu.
 * It SHOULD NOT free the runqueue itself.
 */

void killschedule()
{
	int i;
	struct task_struct *temp1 = rq->head;
	struct task_struct *temp2 = NULL;


	for(i = 0; i < rq->nr_running; i++){
		temp2 = temp1;

		free(temp2);

		temp1 = temp1->next;
	}

	return;
}


void print_rq () {
	struct task_struct *curr;
	
	printf("Rq: \n");
	curr = rq->head;
	if (curr)
		printf("%p", curr);
	while(curr->next != rq->head) {
		curr = curr->next;
		printf(", %p", curr);
	};
	printf("\n");
}


//Calculate expected burst for each task 
void calculate_exp_Burst(){

	struct task_struct *temp;
	int i;

	temp = rq->head->next;


	for(i = 1; i < rq->nr_running; i++) {

		temp->exp_burst = (temp->burst + 0.5 * temp->exp_burst)/(1+0.5);

	}
}


//Find the lowest expected burst from the queue
double minimumExp_Burst() {
	struct task_struct *temp;
	int i;
	double min_exp_burst = 0;

	calculate_exp_Burst();

	temp = rq->head->next;

	min_exp_burst = temp->exp_burst;

	for(i = 1; i < rq->nr_running; i++) {

		if(min_exp_burst > temp->exp_burst) {
			min_exp_burst = temp->exp_burst;
		}

		temp = temp->next;
	}
	
	return min_exp_burst;
}


//Find the proccess that waits the longest in the ready queue
double maximum_waiting_in_RQ() {
	struct task_struct *temp;
	int i;
	double maximum_waiting_in_RQ;

	temp = rq->head->next;

	maximum_waiting_in_RQ = temp->waiting;
	
	for(i = 1; i < rq->nr_running; i++) {

		if(maximum_waiting_in_RQ < temp->waiting) {
			maximum_waiting_in_RQ = temp->waiting;
		}
		
		temp = temp->next;
	}
	
	return maximum_waiting_in_RQ;
}

struct task_struct* find_next_b() {
	double minimum_exp_burst = minimumExp_Burst();
	struct task_struct *temp;

	//epeidh exw fifo thelw na bgalw auto pou mphke prwta that's why going back to forth
	
	for(temp = rq->head->prev; temp->exp_burst != minimum_exp_burst; temp = temp->prev);

	return temp;
}


//Find next task
//Based on the bool value of is_goodness we find the next task by the exp_Burst or the Goodness value
struct task_struct *find_next_task() {
	int i;
	struct task_struct *temp, *next;
	double min_goodness;

	double minimum_exp_burst = minimumExp_Burst();
	double maximum_waiting_in_rq;

	//calculate the waiting time for each task in the queue 
	//to kanoume edw giati edw einai pou tah epileksoume telika to poio tha einai to next
	//kai epeidh paizoume me sched_clock() kalo tha einai poi xronoi na einai oso pio akriveis
	temp = rq->head->next;
	
	for(i = 1; i < rq->nr_running; i++) {

		temp->waiting = sched_clock() - temp->last_run;
		temp = temp->next;

	}

	maximum_waiting_in_rq = maximum_waiting_in_RQ();
	
	//calculate goodness for each task
	temp = rq->head->next;

	for(i = 1; i < rq->nr_running; i++) {

		//Depemding if waiting in rq is being considered, the goodness is calculated accordingly
		if(CONSIDER_WAIT){

			temp->goodness = ((1 + temp->exp_burst) / (1 + minimum_exp_burst)) * ((1 + maximum_waiting_in_rq) / (1 + temp->waiting));
		}
		else{

			temp->goodness = (1 + temp->exp_burst) / (1 + minimum_exp_burst);
		}

		
		temp = temp->next;
	}


	//FIND TASK THAT HAS THE MINIMUM GOODNESS
	temp = rq->head->next;
	//for now the next task is the first in queue
	next = temp;
	//put a temporary min goodness
	min_goodness = temp->goodness;

	for(i = 1; i < rq->nr_running; i++) {

		if(temp->goodness < min_goodness){
			min_goodness = temp->goodness;
			next = temp;
		}
		temp = temp->next;
	}
	
	return next;


}

/*-------------Scheduler Code Goes Below------------*/
/* This is the beginning of the actual scheduling logic */

/* schedule
 * Gets the next task in the queue
 */
void schedule()
{
	static struct task_struct *nxt = NULL;
	double temp_exp_burst, temp_burst, temp_last_run;
	
//	printf("In schedule\n");
//	print_rq();
	
	current->need_reschedule = 0; /* Always make sure to reset that, in case *
								   * we entered the scheduler because current*
								   * had requested so by setting this flag   */
	
	if (rq->nr_running == 1) {
		context_switch(rq->head);
		
	}
	else {	
		
		//Keep old values in case same process keeps running and scheduler does not give cpu to another
		temp_burst = current->burst;
		temp_exp_burst = current->exp_burst;
		temp_last_run = current->last_run;


		current->burst = sched_clock() - current->start_in_cpu; //new temporal burst value of running process

		current->exp_burst = (current->burst + (0.5 * temp_exp_burst)) / (1 + 0.5); //new expected burst value

		//pame na dwsoume epeksergasth opote edw allazw last run gia th twrinh diergasia
		current->last_run = sched_clock();

		nxt = find_next_task();
		
		
		if(current != nxt) {
			nxt->start_in_cpu = sched_clock(); //time when process started being executed in cpu
			context_switch(nxt);
		}
		else {
			//new process is current process
			current->burst = temp_burst;
			current->exp_burst = temp_exp_burst;
			current->last_run = temp_last_run;

			context_switch(current);
		}
	}
}


/* sched_fork
 * Sets up schedule info for a newly forked task
 */
void sched_fork(struct task_struct *p)
{
	p->time_slice = 100;
	p->burst = 0;
	p->exp_burst = 0;
	p->goodness = 0;
	p->start_in_cpu = 0;
	p->waiting = 0;
	p->last_run = 0;
}

/* scheduler_tick
 * Updates information and priority
 * for the task that is currently running.
 */
void scheduler_tick(struct task_struct *p)
{
	schedule();
}

/* wake_up_new_task
 * Prepares information for a task
 * that is waking up for the first time
 * (being created).
 */
void wake_up_new_task(struct task_struct *p)
{	
	p->next = rq->head->next;
	p->prev = rq->head;
	p->next->prev = p;
	p->prev->next = p;

	//entering run queue so keep the time
	p->last_run = sched_clock();
	
	//addition of new task
	rq->nr_running++;
}

/* activate_task
 * Activates a task that is being woken-up
 * from sleeping.
 */
void activate_task(struct task_struct *p)
{
	p->next = rq->head->next;
	p->prev = rq->head;
	p->next->prev = p;
	p->prev->next = p;

	p->last_run = sched_clock();

	rq->nr_running++;
}

/* deactivate_task
 * Removes a running task from the scheduler to
 * put it to sleep.
 */
void deactivate_task(struct task_struct *p)
{
	p->last_run = sched_clock();

	p->prev->next = p->next;
	p->next->prev = p->prev;
	p->next = p->prev = NULL; /* Make sure to set them to NULL *
							   * next is checked in cpu.c      */

	rq->nr_running--;
}