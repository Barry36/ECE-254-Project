#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <math.h>
#include <sys/stat.h>
#include "common.h"

void run_producer(int id, int num, int num_p, int buffer_size);

int main(int argc, char *argv[]) {
	int producer_id;
	int num;
	int num_p;
	int buffer_size;

	if (argc != 5) {
		printf("producer: invalid arguments\n");
		exit(1);
	}

	//parse args
	producer_id = atoi(argv[1]);
	num = atoi(argv[2]);
	num_p = atoi(argv[3]);
	buffer_size = atoi(argv[4]);

	//start producing
	run_producer(producer_id, num, num_p, buffer_size);

	exit(0);
}

void run_producer(int id, int num, int num_p, int buffer_size) {
	struct mq_attr attr;
	attr.mq_maxmsg  = buffer_size;
	attr.mq_msgsize = sizeof(int);
	attr.mq_flags   = 0;	// 0 -> a blocking queue

	//open queue
	mqd_t qdes = mq_open(QUEUE_NAME, O_RDWR | O_CREAT, QUEUE_MODE, &attr);
	if (qdes == -1 ) {
		perror("producer: mq_open() failed");
		exit(1);
	}

	//produce designated values
	int i;
	for(i = id; i <= num-1; i += num_p){
		int msg = i;

		if (mq_send(qdes, (char *)&msg, sizeof(int), 0) == -1) {
			perror("producer: mq_send() failed");
			i -= num_p;
		}
	}

	//close queue
	if (mq_close(qdes) == -1) {
		perror("producer: mq_close() failed");
		exit(2);
	}
}