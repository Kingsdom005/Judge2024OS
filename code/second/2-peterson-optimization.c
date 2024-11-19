// 可以通过添加内存屏障来确保在内存访问顺序上不会出现问题，从而避免竞争条件。
// 使用 GCC 的 __sync_synchronize() 来确保编译器和处理器不会重排内存操作。
// __sync_synchronize() 可以确保在内存屏障前的操作在屏障后执行之前被完全处理。


#include <stdio.h>
#include <pthread.h>
#include <time.h>

volatile int counter = 0;
volatile int flag[2] = {0, 0}; // 标志变量
volatile int turn = 0;         // 表示轮到谁

void* increase_counter(void* arg) {
    int id = *(int*)arg;
    int other = 1 - id;
    
    for (int i = 0; i < 100000; ++i) {
        flag[id] = 1;
        turn = other;
        __sync_synchronize();  // 内存屏障，确保 flag 和 turn 的设置顺序不被重排
        while (flag[other] && turn == other) {
            // 等待对方线程释放锁
            __sync_synchronize();  // 内存屏障，确保循环中的读取不会被优化掉
        }

        // 临界区
        counter++;

        // 退出临界区
        flag[id] = 0;
        __sync_synchronize();  // 内存屏障，确保 flag 清除在临界区之外
    }
    return NULL;
}

int main() {
    clock_t start, finish;
    start = clock();

    pthread_t t1, t2;
    int id1 = 0, id2 = 1;

    pthread_create(&t1, NULL, increase_counter, &id1);
    pthread_create(&t2, NULL, increase_counter, &id2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Final counter value: %d\n", counter);

    finish = clock();
    double Total_time = (double)(finish - start) / CLOCKS_PER_SEC; //单位换算成秒
    printf("%f seconds\n", Total_time);

    return 0;
}
