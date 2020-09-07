PROGC=dynamic.x
OBJSC=dynamic.o
CC=tau_cc.sh
LDC=tau_cc.sh
LD_FLAGS= -lopencv_core -lopencv_highgui -fopenmp
FLAGS= -I/usr/include/opencv -fopenmp
RM=/bin/rm


#all rule
all: $(PROGC) $(PROGF)

$(PROGC): $(OBJSC)
	$(LDC) $^ $(LD_FLAGS) -o $@

%C.o: %.c
	$(CC) $(FLAGS) -c $^ -o $@


PROG=dynamicN.x
OBJS=dynamic.o
CN=mpicc
LDN=mpicc
LDN_FLAGS= -fopenmp
FLAGSN= -fopenmp

normal: $(PROG)

$(PROG):$(OBJS)
	$(LDN) $^ $(LDN_FLAGS) -o $@

%.o:%.c
	$(CN) $(FLAGSN) -c $^ -o $@


clean:
	$(RM) -rf $(PROGC) $(OBJSC) $(PROG) $(OBJS) run* profile* app*

export:
	export TAU_MAKEFILE=/opt/sw/tau/x86_64/lib/Makefile.tau-papi-mpi-pdt-openmp-opari
print:
	cat run*
