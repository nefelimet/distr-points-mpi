CC = mpicc
EXEC = mpiexec
CFLAGS = -O0 -lm
INCLUDES = -I.
OBJS = $(SRCS: .c=.o)
SRCS = main.c mpi_distr.c read_bin.c utils.c
SRC = a.out
NPROC = 4
N = 16
D = 4

default: $(SRC)
	@echo $(SRC) has been compiled.

%.o: %.c 
	$(CC) -c -o $@ $< $(CFLAGS) $(INCLUDES)
	
$(SRC): $(OBJS)
	$(CC) -o $(SRC) $(OBJS) $(CFLAGS) $(INCLUDES)
	
run: $(SRC)
	$(EXEC) -n $(NPROC) ./$(SRC) $(NPROC) $(N) $(D)
	
clean:
	rm -f *~ *.o $(SRC)
