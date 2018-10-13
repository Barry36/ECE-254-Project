#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <signal.h>
#include <sys/stat.h>
#include <math.h>
#include "common.h"

void run_consumer(int id, int buffer_size);

int main(int argc, char *argv[]) {
	int consumer_id;
	int buffer_size;

	if (argc != 3) {
		printf("invalid arguments\n");
		exit(1);
	}

	//parse args
	consumer_id = atoi(argv[1]);
	buffer_size = atoi(argv[2]);

	//start consuming
	run_consumer(consumer_id, buffer_size);

	exit(0) ;
}

void run_consumer(int id, int buffer_size) {
	struct mq_attr attr;
	attr.mq_maxmsg  = buffer_size;
	attr.mq_msgsize = sizeof(int);
	attr.mq_flags   = 0;	// 0 -> a blocking queue

	//open queue
	mqd_t qdes = mq_open(QUEUE_NAME, O_RDWR | O_CREAT, QUEUE_MODE, &attr);
	if (qdes == -1 ) {
		perror("consumer: mq_open() failed");
		exit(1);
	}

	int msg;
	msg = 0;

	//keep receiving until termination signal is received
	while(msg != -1) {
		if (mq_receive(qdes, (char *)&msg, sizeof(int), 0) == -1) {
			perror("consumer: mq_recieve() failed");
		}

		//check and print square root
		int root= sqrt((double)msg);
		if(root == sqrt((double)msg)) {
			printf("%d %d %d\n", id, msg, root);
		}
	}

	//close queue
	if (mq_close(qdes) == -1) {
		perror("consumer: mq_close() failed");
		exit(2);
	}
}