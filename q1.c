#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <sys/time.h>

#define SIZE 40000000
FILE *fp;

void merge(int arr[], int temp[], int left, int mid, int right) {
    int i, j, k;
    i = left;
    j = mid;
    k = left;
    while (i < mid && j < right) {
        if (arr[i] <= arr[j]) {
            temp[k] = arr[i];
            i++;
        }
        else {
            temp[k] = arr[j];
            j++;
        }
        k++;
    }
    while (i < mid) {
        temp[k] = arr[i];
        i++;
        k++;
    }
    while (j < right) {
        temp[k] = arr[j];
        j++;
        k++;
    }
    for (i = left; i < right; i++) {
        arr[i] = temp[i];
    }
}

void mergeSort(int arr[], int n, int num_threads, double *execution_times) {
    int *temp = malloc(n * sizeof(int));
    int width, i, left;
    double start_time, end_time;
    for (width = 1; width < n; width = 2 * width) {
        #pragma omp parallel for num_threads(num_threads) private(left)
        for (left = 0; left < n; left = left + 2 * width) {
            int mid = left + width;
            int right = left + 2 * width;
            if (mid > n) {
                mid = n;
            }
            if (right > n) {
                right = n;
            }
            merge(arr, temp, left, mid, right);
        }
    }
    free(temp);
}

int main() {
    int *arr = malloc(SIZE * sizeof(int));
    int i, num_threads;
    double execution_times[9];
    struct timeval t1, t2;
    int number_of_threads[]={1,2,4,6,8,10,12,14,16};
    char *datafile="AzeezBhai.csv";
    fp = fopen(datafile, "w");
    fprintf(fp, "Number of threads,Execution time\n");
    
    for(int x=0;x<9;x++)
    {
        num_threads = number_of_threads[x];
        printf("Number of threads: %d\n", num_threads);
        gettimeofday(&t1, NULL);
        for (i = 0; i < SIZE; i++) {
            arr[i] = rand();
        }
        mergeSort(arr, SIZE, num_threads, &execution_times[x]);
        gettimeofday(&t2, NULL);
        double execution_time = (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) * 1e-6;
        fprintf(fp, "%d,%lf\n", num_threads, execution_time);
        printf("Execution time: %lf seconds\n", execution_time);
    }
    free(arr);
    fclose(fp);
    return 0;
}
