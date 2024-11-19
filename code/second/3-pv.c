#include <stdio.h>
#include <pthread.h>

int gcounter = 0; // 共享变量

int semaphore = 1; // 信号量初始值
pthread_mutex_t mutex;
pthread_cond_t cond;

void P() {
    pthread_mutex_lock(&mutex);
    while (semaphore == 0) {
        pthread_cond_wait(&cond, &mutex);
    }
    semaphore--;
    pthread_mutex_unlock(&mutex);
}

void V() {
    pthread_mutex_lock(&mutex);
    semaphore++;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}

void* thread_func(void* arg) {
    P();
    // printf("Critical section\n"); // 临界区代码
    int counter = *(int*)arg;
    for (int i = 0; i < counter; ++i) {
        gcounter++;  // 竞争条件
    }
    V();
    return NULL;
}

int main() {

    FILE *file = fopen("in.txt", "r");  // 打开文件
    if (file == NULL) {
        printf("Error opening file in.txt\n");
        return 1;  // 文件打开失败
    }

    int counter;
    if (fscanf(file, "%d", &counter) != 1) {  // 从文件读取整数
        printf("Failed to read counter from file\n");
        fclose(file);
        return 1;  // 读取失败
    }
    fclose(file);  // 关闭文件

    pthread_t t1, t2;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&t1, NULL, thread_func, &counter);
    pthread_create(&t2, NULL, thread_func, &counter);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    printf("Counter: %d\n", gcounter);

    // 将 gcounter 的值写入 out.txt 文件
    FILE *outFile = fopen("out3.txt", "w");  // 打开文件以写入
    if (outFile == NULL) {
        printf("Error opening file out.txt\n");
        return 1;  // 文件打开失败
    }
    fprintf(outFile, "%d", gcounter); 
    fclose(outFile);  // 关闭文件
    
    return 0;
}