EXECS=mpi_hello_world
MPICC?=mpicc.openmpi

all: ${EXECS}

mpi_hello_world: mpi_hello_world.c
	${MPICC} -o exec mpi_hello_world.c

clean:
	rm ${EXECS}
    
