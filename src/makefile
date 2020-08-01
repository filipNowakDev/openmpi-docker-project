EXECS=main
MPICC?=mpicc.openmpi

all: ${EXECS}

main: main.c
	${MPICC} -o main main.c

clean:
	rm ${EXECS}
    
