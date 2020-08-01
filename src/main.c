#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_NUMBER 1000
#define ARRAY_PER_PROCESSOR 10

int* init_random_array(int size);
void sort_array(int* array, int size);
void print_array(char* name, int rank, int* array, int size);
void merge_arrays_into_result(int* result, int* subarrays, int size, int subarray_size);

int main(int argc, char** argv) {

    srand(time(NULL));
    MPI_Init(NULL, NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int* input_array;

    if (!world_rank) // mpi_head processor 0
    {
        printf("World size: %d\n", world_size);
        printf("Array per processor size: %d\n", ARRAY_PER_PROCESSOR);
        printf("Input size: %d\n", world_size * ARRAY_PER_PROCESSOR);
        input_array = init_random_array(ARRAY_PER_PROCESSOR * world_size);
        print_array("Input array", world_rank, input_array, ARRAY_PER_PROCESSOR * world_size);
    }
    int* per_proc_array = malloc(sizeof(int) * ARRAY_PER_PROCESSOR);
    
    MPI_Scatter(input_array, ARRAY_PER_PROCESSOR, MPI_INT, per_proc_array,
              ARRAY_PER_PROCESSOR, MPI_INT, 0, MPI_COMM_WORLD);
    print_array("Unsorted subarray", world_rank, per_proc_array, ARRAY_PER_PROCESSOR);

    
    sort_array(per_proc_array, ARRAY_PER_PROCESSOR);
    print_array("Sorted subarray", world_rank, per_proc_array, ARRAY_PER_PROCESSOR);

    int* received_arrays;
    if (!world_rank)
    {
        received_arrays = malloc(sizeof(int) * world_size * ARRAY_PER_PROCESSOR);
    }

    MPI_Gather(per_proc_array, ARRAY_PER_PROCESSOR, MPI_INT, received_arrays, ARRAY_PER_PROCESSOR, MPI_INT, 0, MPI_COMM_WORLD);
    free(per_proc_array);
    
    if (!world_rank) 
    {
        print_array("Received sorted subarrays", 0, received_arrays, world_size * ARRAY_PER_PROCESSOR);
        merge_arrays_into_result(input_array, received_arrays, world_size * ARRAY_PER_PROCESSOR, ARRAY_PER_PROCESSOR);
        free(received_arrays);
        print_array("MERGED final sorted array", 0, input_array, world_size * ARRAY_PER_PROCESSOR);
        free(input_array);
    }

    MPI_Finalize();
    return 0;
}

int* init_random_array(int size)
{
    int* array = malloc(sizeof(int) * size);
    for (int i = 0; i < size; i++)
        array[i] = rand() % MAX_NUMBER;
    return array;
}

void print_array(char* name, int rank, int* array, int size)
{
    printf("%s of size %d from proc %d:\n", name, size, rank);
    for(int i = 0 ; i < size; i++)
        printf("%d ", array[i]);
    printf("\n");

}

void sort_array(int* array, int size)
{
    int swapped = 1;
    while (swapped)
    {
        swapped = 0;
        for (int i = 0; i < size - 1; i++)
        {
            if (array[i+1] < array[i])
            {
                swapped = 1;
                int tmp = array[i];
                array[i] = array[i+1];
                array[i+1] = tmp;
            }
        }
    }
}

void merge_arrays_into_result(int* result, int* subarrays, int size, int subarray_size)
{
    int subarray_count = (size / subarray_size);
    int** subarray = malloc(sizeof(int*) * subarray_count);
    int* merge_cnt = malloc(sizeof(int) * subarray_count);
    for (int i = 0; i < subarray_count; i++)
    {
        subarray[i] = subarrays + i * subarray_size;
        merge_cnt[i] = 0;
    }

    for(int merged_elements = 0; merged_elements < size; merged_elements++)
    {
        int merge_index = 0;
        int num_to_merge = __INT_MAX__;
        for(int i = 0; i < subarray_count; i++)
        {
            if (merge_cnt[i] < subarray_size && subarray[i][merge_cnt[i]] < num_to_merge)
            {
                merge_index = i;
                num_to_merge = subarray[i][merge_cnt[i]];
            }
        }
        merge_cnt[merge_index]++;
        result[merged_elements] = num_to_merge;
    }
}

