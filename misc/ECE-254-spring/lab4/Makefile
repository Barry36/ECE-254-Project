# Makefile, ECE254 lab4 Linux variable size memory management lab 
# Yiqing Huang, 2017/07/10

CC = gcc
CFLAGS = -Wall -g
LD = gcc
LDFLAGS = -g 
LDLIBS = 

SRCS   = mem.c main_test.c excesive_use_test.c
OBJS1   = mem.o main_test.o
OBJS2   = mem.o excesive_use_test.o
TARGETS= main_test.out excesive_use_test.out

all: ${TARGETS}

main_test.out: $(OBJS1) 
	$(LD) -o $@ $^ $(LDLIBS) $(LDFLAGS)

excesive_use_test.out: $(OBJS2) 
	$(LD) -o $@ $^ $(LDLIBS) $(LDFLAGS) 

%.o: %.c 
	$(CC) $(CFLAGS) -c $< 

%.d: %.c
	gcc -MM -MF $@ $<

-include $(SRCS:.c=.d)

.PHONY: clean
clean:
	rm -f *.d *.o $(TARGETS) 
