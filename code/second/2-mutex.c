// 使用 互斥锁 解决竞争条件问题
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

volatile int gcounter = 0;
pthread_mutex_t lock;

void* increase_counter(void* arg) {
    int counter = *(int*)arg;
    for (int i = 0; i < counter; ++i) {
        pthread_mutex_lock(&lock);
        gcounter++;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main() {

    FILE *file = fopen("./in.txt", "r");  // 打开文件
    if (file == NULL) {
        printf("Error opening file in.txt(2-mutex)\n");
        return 1;  // 文件打开失败
    }

    int counter;
    if (fscanf(file, "%d", &counter) != 1) {  // 从文件读取整数
        printf("Failed to read counter from file\n");
        fclose(file);
        return 1;  // 读取失败
    }
    fclose(file);  // 关闭文件


    clock_t start, finish;
    start = clock();

    pthread_t t1, t2;
    pthread_mutex_init(&lock, NULL);


    // 创建两个线程
    pthread_create(&t1, NULL, increase_counter, &counter);
    pthread_create(&t2, NULL, increase_counter, &counter);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Final counter value: %d\n", gcounter);
    pthread_mutex_destroy(&lock);

    // 将 gcounter 的值写入 out2.txt 文件
    FILE *outFile = fopen("out2.txt", "w");  // 打开文件以写入
    if (outFile == NULL) {
        printf("Error opening file out.txt\n");
        return 1;  // 文件打开失败
    }
    fprintf(outFile, "%d", gcounter); 
    fclose(outFile);  // 关闭文件

    finish = clock();
    double Total_time;
	Total_time = (double)(finish - start) / CLOCKS_PER_SEC; //单位换算成秒
	// printf("%f seconds\n", Total_time);

    return 0;
}