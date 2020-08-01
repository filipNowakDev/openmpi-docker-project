# Open MPI docker cluster

Simple docker cluster for testing OpenMPI applications.

## Building image

To build the openmpi enabled docker image you should run `./build_image.sh`

## Running cluster

To run the 4 node cluster just run `./start_cluster.sh`.
This command will start 4 docker instances and ssh into the main one.
Password is `mpiuser`.

## Running program
To compile sources in docker's bash you should do `cd src` and `sudo make`.
To run freshly compiled executable you can `./run.sh` - this will execute the mpi program on all 4 nodes.

## Shutdown

To exit the container press Ctrl-D.
After that in host shell run `./shutdown-container.sh`. This will shutdown and remove cluster's containers.

## Example

Example program I created demostrates distributed bubble/merge alogrithm mixture which is cool and shows how to use scatter/gather MPI functions.