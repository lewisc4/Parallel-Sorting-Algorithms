CC=tau_cc.sh
LDC=tau_cc.sh
PROG=quicksort.x
OBJS=quicksort.o
LD_FLAGS= -fopenmp -lstdc++ -lopencv_highgui -lopencv_core
FLAGS= -fopenmp -I/user/include/opencv

CPP=gcc
LDP=gcc
PROGN=quicksortN.x
OBJSN=quicksort.o
LDN_FLAGS= -fopenmp
FLAGSN= -fopenmp
RM=/bin/rm

all: $(PROGN)

$(PROGN):$(OBJSN)
	$(CPP) $^ $(LDN_FLAGS) -o $@

%.o:%.c
	$(LDP) $(FLAGSN) -c $^ -o $@
	
#clean rule
clean:
	$(RM) -rf $(PROGN) $(OBJSN) run*


