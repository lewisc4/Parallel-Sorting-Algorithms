PROG = bitonic.x
OBJS = bitonic.o
LD_FLAGS = -fopenmp
FLAGS = -fopenmp
CC = gcc



all:$(PROG)

$(PROG):$(OBJS)
	$(CC) $(OBJS) -o $(PROG) $(LD_FLAGS)

%.o:%.c
	$(CC) -c $< $(FLAGS)

clean:
	rm -f $(OBJS) $(PROG)
