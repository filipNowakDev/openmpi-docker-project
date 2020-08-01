#!/bin/bash
docker-compose up --scale mpi_node=3 -d
ssh -p 2222 mpiuser@localhost