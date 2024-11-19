// 使用 peterson方法 解决竞争条件问题
#include <stdio.h>
#include <pthread.h>
#include <time.h>

volatile int gcounter = 0;
volatile int flag[2] = {0, 0}; // 标志变量
volatile int turn = 0;         // 表示轮到谁

typedef struct {
    int id;
    int counter;
} arg_t;

// void* increase_counter(void* arg) {

//     int counter = *(int*)arg.counter;

//     int id = *(int*)arg.id;
//     int other = 1 - id;
    
//     for (int i = 0; i < counter; ++i) {
//         flag[id] = 1;
//         turn = other;
//         while (flag[other] && turn == other);

//         // 临界区
//         gcounter++;

//         // 退出临界区
//         flag[id] = 0;
//     }
//     return NULL;
// }

void* increase_counter(void* arg) {
    // 将 void* 转换为 arg_t* 类型
    arg_t* args = (arg_t*)arg;

    int counter = args->counter; // 获取计数值
    int id = args->id; // 获取线程ID
    int other = 1 - id; // 计算另一个线程的ID
    
    for (int i = 0; i < counter; ++i) {
        flag[id] = 1; // 设置当前线程为准备状态
        turn = other; // 将轮次切换到另一个线程
        
        // 等待另一个线程退出临界区
        while (flag[other] && turn == other);

        // 临界区
        gcounter++; // 更新全局计数器

        // 退出临界区
        flag[id] = 0;
    }
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

    clock_t start, finish;
    start = clock();

    pthread_t t1, t2;

    arg_t arg1 = {0, counter};
    arg_t arg2 = {1, counter};
    // int id1 = 0, id2 = 1;

    pthread_create(&t1, NULL, increase_counter, &arg1);
    pthread_create(&t2, NULL, increase_counter, &arg2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Final counter value: %d\n", gcounter);

    // 将 gcounter 的值写入 out2.txt 文件
    FILE *outFile = fopen("out22.txt", "w");  // 打开文件以写入
    if (outFile == NULL) {
        printf("Error opening file out.txt\n");
        return 1;  // 文件打开失败
    }
    fprintf(outFile, "%d", gcounter); 
    fclose(outFile);  // 关闭文件

    finish = clock();
    double Total_time;
	Total_time = (double)(finish - start) / CLOCKS_PER_SEC; //单位换算成秒
	printf("%f seconds\n", Total_time);

    return 0;
}