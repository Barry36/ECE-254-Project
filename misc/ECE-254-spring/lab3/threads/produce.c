// Use this to see if a number has an integer square root
#define EPS 1.E-7

#define QUEUE_NAME "/mebrokas_lseewald_lab3_queue"
#define QUEUE_MODE S_IRUSR | S_IWUSR

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
#include <pthread.h>
#include <semaphore.h>

double g_time[2];

void* run_consumer(void* arg);
void* run_producer(void* args);

pthread_mutex_t mutex;
sem_t spaces;
sem_t items;
int* buffer;

int indexProducer;
int indexConsumer;
int num_received = 0;

int main(int argc, char *argv[]) {
	int num;
	int maxmsg;
	int num_p;
	int num_c;
	int i;
	struct timeval tv;

	if (argc != 5) {
		printf("Usage: %s <N> <B> <P> <C>\n", argv[0]);
		exit(1);
	}

	num = atoi(argv[1]);	/* number of items to produce */
	maxmsg = atoi(argv[2]); /* buffer size                */
	num_p = atoi(argv[3]);  /* number of producers        */
	num_c = atoi(argv[4]);  /* number of consumers        */

	pthread_mutex_init(&mutex, NULL);
	sem_init(&spaces, 0, maxmsg);
	sem_init(&items, 0, 0);
	buffer = (int*)malloc(sizeof(int)*maxmsg);
	indexConsumer = 0;
	indexProducer = 0;

	//capture start time
	gettimeofday(&tv, NULL);
	g_time[0] = (tv.tv_sec) + tv.tv_usec/1000000.;

	pthread_t producers[num_p];
	pthread_t consumers[num_c];

	//spawn producers
	for(i = 0; i < num_p; ++i) {
		int* args = (int*)malloc(sizeof(int));
		args[0] = i;
		args[1] = num;
		args[2] = num_p;
		args[3] = maxmsg;
		pthread_create(&producers[i], NULL, run_producer, (void*)args);
	}

	//spawn consumers
	for(i = 0; i < num_c; ++i) {
		int* args = (int*)malloc(3*sizeof(int));
		args[0] = i;
		args[1] = maxmsg;
		args[2] = num;
		pthread_create(&consumers[i], NULL, run_consumer, (void*)args);
	}

	//kill all the threads for more accurate thread/process comparison
	for(i = 0; i < num_p; ++i) {
		pthread_join(producers[i], NULL);
	}
	for(i = 0; i < num_c; ++i) {
		pthread_join(consumers[i], NULL);
	}

	//clean up
	free(buffer);
	pthread_mutex_destroy(&mutex);
	sem_destroy(&items);
	sem_destroy(&spaces);

	//capture end time
    gettimeofday(&tv, NULL);
    g_time[1] = (tv.tv_sec) + tv.tv_usec/1000000.;

    printf("System execution time: %.6lf seconds\n", \
            g_time[1] - g_time[0]);

	//terminate
	exit(0);
}

void* run_producer(void* args) {
	int id = ((int*)args)[0];
	int num = ((int*)args)[1];
	int num_p = ((int*)args)[2];
	int buffer_size = ((int*)args)[3];
	free(args);

	int i;
	for(i = id; i <= num-1; i += num_p){
		sem_wait(&spaces);									//wait for space in buffer
		pthread_mutex_lock(&mutex);							//start critical section
		buffer[indexProducer] = i;							//add item to buffer
		indexProducer = (indexProducer + 1) % buffer_size;	//increment and mod next space position
		pthread_mutex_unlock(&mutex);						//end critical section
		sem_post(&items);									//signal new item

	}

	//end thread
	pthread_exit(NULL);
}

void* run_consumer(void* args) {
	int id = ((int*)args)[0];
	int buffer_size = ((int*)args)[1];
	int num = ((int*)args)[2];
	free(args);

	int msg;

	while(1){
		sem_wait(&items);					//wait for items
		pthread_mutex_lock(&mutex);			//start critical section

		if(num_received >= num) {			//if all numbers are recieved then consumer should finish
			pthread_mutex_unlock(&mutex);	//exit critical section
			sem_post(&items);				//allow next consumer to get through and finish
			break;							//break from loop
		}
		
		msg = buffer[indexConsumer];						//read item
		indexConsumer = (indexConsumer + 1) % buffer_size;	//increment and mod next item position
		++num_received;										//increment number of received items
		if(num_received == num) {
			sem_post(&items);
		}
		
		pthread_mutex_unlock(&mutex);						//end critical section
		sem_post(&spaces);									//signal new space

		//check for and print integer root
		int root = sqrt((double)msg);
		if(root == sqrt((double)msg)) {
			printf("%d %d %d\n", id, msg, root);
		}
	}

	//end thread (technically will never be called)
	pthread_exit(NULL);
}