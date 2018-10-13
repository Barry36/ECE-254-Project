# Makefile, ECE254 lab5 Linux Concurrency Control
# Yiqing Huang, 2014/07/16

CC = gcc
CFLAGS = -Wall -g #-DDEBUG_1
LD = gcc
LDFLAGS = -g 
LDLIBS = -lrt -lpthread -lm

SRCS = produce.c 
OBJS_PRODUCE = produce.o
OBJS_CONSUMER = consumer.o
OBJS_PRODUCER = producer.o
TARGETS = produce consumer producer

all: ${TARGETS}


produce: $(OBJS_PRODUCE) 
	$(LD) $(LDFLAGS) -o $@.out $^ $(LDLIBS) 
	
consumer: $(OBJS_CONSUMER) 
	$(LD) $(LDFLAGS) -o $@.out $^ $(LDLIBS)

producer: $(OBJS_PRODUCER) 
	$(LD) $(LDFLAGS) -o $@.out $^ $(LDLIBS)

%.o: %.c 
	$(CC) $(CFLAGS) -c $< 

%.d: %.c
	gcc -MM -MF $@ $<

.PHONY: clean
clean:
	rm -f *.d *.o *.out
