// Use this to see if a number has an integer square root
#define EPS 1.E-7

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>
#include "common.h"

double g_time[2];
int spawn_consumer(int id, int buffer_size);
int spawn_producer (int id, int num, int num_p, int buffer_size);
void send_termination_sigs(int num_c);

//TODO: make time go up to the last integer consumption (how?)

int main(int argc, char *argv[]) {
	int num;
	int buffer_size;
	int num_p;
	int num_c;
	struct timeval tv;

	if (argc != 5) {
		printf("Usage: %s <N> <B> <P> <C>\n", argv[0]);
		exit(1);
	}

	num = atoi(argv[1]);			/* number of items to produce */
	buffer_size = atoi(argv[2]); 	/* buffer size                */
	num_p = atoi(argv[3]);  		/* number of producers        */
	num_c = atoi(argv[4]);  		/* number of consumers        */

	int producer_ids[num_p];
	int consumer_ids[num_c];

	//capture start time
	gettimeofday(&tv, NULL);
	g_time[0] = (tv.tv_sec) + tv.tv_usec/1000000.;

	//spawn producers and consumers
	int i;
	for(i = 0; i <  num_p; ++i){
		producer_ids[i] = spawn_producer(i, num, num_p, buffer_size);
	}
	for(i = 0; i < num_c; ++i){
		consumer_ids[i] = spawn_consumer(i, buffer_size);
	}

	//wait for all the producers to finish
	int child_status;
	for(i = 0; i < num_p; ++i) {
		waitpid(producer_ids[i], &child_status, 0 );
	}

	//send the terminination signals to consumers
	send_termination_sigs(num_c);

	//wait for consumers to finish
	for(i = 0; i < num_c; ++i) {
		waitpid(consumer_ids[i], &child_status, 0 );
	}

	//capture end time
 	gettimeofday(&tv, NULL);
 	g_time[1] = (tv.tv_sec) + tv.tv_usec/1000000.;

 	printf("System execution time: %.6lf seconds\n", \
		g_time[1] - g_time[0]);

	//cleanup
	mq_unlink(QUEUE_NAME);

	exit(0);
}

int spawn_consumer(int id, int buffer_size) {
	pid_t child_pid;

	child_pid = fork ();

	if (child_pid != 0) {
		return child_pid;	//this is parent, return pid of child
	} else {
		//create string versions of ints	 
		char id_str[20];
		char buffer_size_str[20];
    		snprintf(id_str, sizeof(id_str), "%d", id);
		snprintf(buffer_size_str, sizeof(buffer_size_str), "%d", buffer_size);

		//create args
		char* args[4];
		args[0] = "./consumer.out";
		args[1] = id_str;
		args[2] = buffer_size_str;
		args[3] = NULL;

		//start consumer
		execvp("./consumer.out", args);
	}
	return -1;
}

int spawn_producer (int id, int num, int num_p, int buffer_size) {
	pid_t child_pid;

	child_pid = fork ();
	
	if (child_pid != 0) {
		return child_pid;	//this is parent, return pid of child
	} else {
		//create string version of ints
		char id_str[20];
		char num_str[20];
		char num_p_str[20];
		char buffer_size_str[20];
    		snprintf(id_str, sizeof(id_str), "%d", id);
    		snprintf(num_str, sizeof(num_str), "%d", num);
    		snprintf(num_p_str, sizeof(num_p_str), "%d", num_p);
		snprintf(buffer_size_str, sizeof(buffer_size_str), "%d", buffer_size);

		//create args
		char* args[8];
		args[0] = "./producer.out";
		args[1] = id_str;
		args[2] = num_str;
		args[3] = num_p_str;
		args[4] = buffer_size_str;
		args[5] = NULL;

		//start producer
		execvp("./producer.out", args);
	}
	return -1;
}

void send_termination_sigs(int num_c) {
	int msg = -1;

	//open queue
	mqd_t qdes = mq_open(QUEUE_NAME, O_RDWR, QUEUE_MODE);
	if (qdes == -1 ) {
		perror("main process: mq_open() failed");
		exit(1);
	}

	//send num_c terminination signals
	int i;
	for(i = 0; i < num_c; ++i) {
		if (mq_send(qdes, (char *)&msg, sizeof(int), 0) == -1) {
			perror("mq_send() failed");
		}
	}

	//close queue
	if (mq_close(qdes) == -1) {
		perror("mq_close() failed");
		exit(2);
	}
}
