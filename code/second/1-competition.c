#include <stdio.h>
#include <pthread.h>
#include <stdlib.h> 
#include <string.h>

int global_counter = 0;  // 全局计数器

void* increase_counter(void* arg) {
    int counter = *(int*)arg;  // 读取传入的计数次数
    for (int i = 0; i < counter; ++i) {
        global_counter++;  // 竞争条件
    }
    return NULL;
}

int main() {
    FILE *file = fopen("./in.txt", "r");  // 打开文件
    if (file == NULL) {
        printf("Error opening file in.txt(1-competition.c)\n");
        return 1;  // 文件打开失败
    }

    int counter;
    if (fscanf(file, "%d", &counter) != 1) {  // 从文件读取整数
        printf("Failed to read counter from file\n");
        fclose(file);
        return 1;  // 读取失败
    }
    fclose(file);  // 关闭文件

    printf("Variable counter: %d \n", counter);
    // printf("len of counter: %d\n", (int)log10(counter) + 1); // 计算数字的位数

    pthread_t t1, t2;

    // 创建两个线程
    pthread_create(&t1, NULL, increase_counter, &counter);
    pthread_create(&t2, NULL, increase_counter, &counter);

    // 等待两个线程执行完毕
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Final counter value: %d\n", global_counter);  // 输出最终的计数值

    // 将 global_counter 的值写入 out.txt 文件
    FILE *outFile = fopen("out1.txt", "w");  // 打开文件以写入
    if (outFile == NULL) {
        printf("Error opening file out.txt\n");
        return 1;  // 文件打开失败
    }
    fprintf(outFile, "%d", global_counter);  // 写入 global_counter 的值
    fclose(outFile);  // 关闭文件

    return 0;
}
