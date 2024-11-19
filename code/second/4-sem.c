#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

int buffer_size;  // 缓冲区大小
int produce_count;  // 生产的数量
int *buffer;  // 缓冲区
int count = 0;

FILE *output_file; // 文件指针

pthread_mutex_t mutex;
sem_t empty, full;

void* producer(void* arg) {
    for (int i = 0; i < produce_count; ++i) {
        sem_wait(&empty); // 等待空槽
        pthread_mutex_lock(&mutex);
        buffer[count++] = i;
        fprintf(output_file, "p%d\n", i);
        pthread_mutex_unlock(&mutex);
        sem_post(&full); // 通知缓冲区非空
    }
    return NULL;
}

void* consumer(void* arg) {
    for (int i = 0; i < produce_count; ++i) {
        sem_wait(&full); // 等待缓冲区非空
        pthread_mutex_lock(&mutex);
        int item = buffer[--count];
        fprintf(output_file, "c%d\n", item);
        pthread_mutex_unlock(&mutex);
        sem_post(&empty); // 通知空槽
    }
    return NULL;
}

int main() {
    // 从in.txt读取BUFFER_SIZE和produce_count
    FILE *input_file = fopen("in2.txt", "r");
    if (input_file == NULL) {
        perror("Error opening input file");
        return 1;
    }
    fscanf(input_file, "%d", &buffer_size);  // 读取缓冲区大小
    fscanf(input_file, "%d", &produce_count);  // 读取生产数量
    fclose(input_file);

    // 动态分配缓冲区
    buffer = (int *)malloc(buffer_size * sizeof(int));
    if (buffer == NULL) {
        perror("Error allocating memory for buffer");
        return 1;
    }

    pthread_t prod, cons;
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, buffer_size);
    sem_init(&full, 0, 0);

    // 打开文件以写入
    output_file = fopen("out4.txt", "w");
    if (output_file == NULL) {
        perror("Error opening output file");
        free(buffer); // 释放分配的内存
        return 1;
    }

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    free(buffer);  // 释放分配的内存
    fclose(output_file);  // 关闭文件
    return 0;
}
