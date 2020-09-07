PROG=oddEvenParallel.x
OBJS=oddEvenParallel.o
LD_FLAGS= -fopenmp
FLAGS = -fopenmp
CC=tau_cc.sh
RM= /bin/rm

all:$(PROG)

$(PROG):$(OBJS)
	$(CC) $(OBJS) $(FLAGS) -o $(PROG)

%.o:%.c
	$(CC) -c $(FLAGS) $<

clean:
	rm -f $(OBJS) $(PROG)
