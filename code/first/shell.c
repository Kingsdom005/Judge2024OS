#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 3

typedef enum { READY, BLOCKED, RUNNING } ProcessState;
typedef enum { LOW, MEDIUM, HIGH } Priority;

typedef struct Process {
    int pid;
    char name[20];
    ProcessState state;
    Priority priority;
    bool waiting_io;
} Process;

typedef struct Resource {
    char name[10];
    int available;
    int total;
} Resource;

Process processes[MAX_PROCESSES];
Resource resources[MAX_RESOURCES];
Process* running_process = NULL; // one kernel

// end define data structures

// 本案例中允许进程同名，如果设置不同名改此函数即可
int create_process(const char* name, Priority priority) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (processes[i].pid == 0) {  // 未使用的进程槽
            processes[i].pid = i + 1;
            strcpy(processes[i].name, name);
            processes[i].state = READY;
            processes[i].priority = priority;
            processes[i].waiting_io = false;
            printf("Process %s created with PID: %d  Priority: %d\n", name, processes[i].pid, priority);
            return processes[i].pid;
        }
    }
    printf("(create_process)Error: Process table full.\n");
    return -1;
}

void destroy_process(int pid) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (processes[i].pid == pid) {
            printf("Destroying process %s (PID: %d)\n", processes[i].name, pid);
            processes[i].pid = 0;
            return;
        }
    }
    printf("(destroy_process)Error: Process with PID %d not found.\n", pid);
}

void activate_process(int pid) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (processes[i].pid == pid) {
            printf("Activating process %s (PID: %d)\n", processes[i].name, pid);
            running_process = &processes[i];
            processes[i].state = RUNNING;
            return;
        }
    }
    printf("(activate_process)Error: Process with PID %d not found.\n", pid);
}

void request_resource(int pid, const char* resource_name) {
    Process* p = NULL;
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (processes[i].pid == pid) {
            p = &processes[i];
            break;
        }
    }
    if (!p) {
        printf("(request_resource_1)Error: Process with PID %d not found.\n", pid);
        return;
    }

    for (int i = 0; i < MAX_RESOURCES; i++) {
        if (strcmp(resources[i].name, resource_name) == 0) {
            if (resources[i].available > 0) {
                resources[i].available--;
                printf("Resource %s allocated to process %s (PID: %d)\n", resource_name, p->name, pid);
            } else {
                printf("Error: Resource %s not available.\n", resource_name);
                p->state = BLOCKED;
            }
            return;
        }
    }
    printf("(request_resource_2)Error: Resource %s not found.\n", resource_name);
}

void release_resource(int pid, const char* resource_name) {
    for (int i = 0; i < MAX_RESOURCES; i++) {
        if (strcmp(resources[i].name, resource_name) == 0) {
            resources[i].available++;
            printf("Resource %s released by process with PID: %d\n", resource_name, pid);
            return;
        }
    }
    printf("(release_resource)Error: Resource %s not found.\n", resource_name);
}

void request_io(int pid) {
    printf("Get request_io pid: %d\n", pid);
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (processes[i].pid == pid) {
            printf("Process %s (PID: %d) requesting I/O\n", processes[i].name, pid);
            processes[i].waiting_io = true;
            processes[i].state = BLOCKED;
            return;
        }
    }
    printf("(request_io)Error: Process with PID %d not found.\n", pid);
}

void io_completion(int pid) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (processes[i].pid == pid) {
            printf("I/O completed for process %s (PID: %d)\n", processes[i].name, pid);
            processes[i].waiting_io = false;
            processes[i].state = READY;
            return;
        }
    }
    printf("(io_completion)Error: Process with PID %d not found.\n", pid);
}

void timeout(int pid) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (processes[i].pid == pid) {
            printf("Process %s (PID: %d) has timed out\n", processes[i].name, pid);
            processes[i].state = READY;
            return;
        }
    }
    printf("(timeout)Error: Process with PID %d not found.\n", pid);
}

void list_process_status() {
    printf("---------------------------------------------\n");
    printf("PID\tName\t\tState\t\tPriority\n");
    printf("---------------------------------------------\n");
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (processes[i].pid != 0) {  // 只打印有效的进程
            printf("%d\t%s\t\t", processes[i].pid, processes[i].name);

            // 打印进程状态
            switch (processes[i].state) {
                case READY:
                    printf("READY\t\t");
                    break;
                case RUNNING:
                    printf("RUNNING\t\t");
                    break;
                case BLOCKED:
                    printf("BLOCKED\t\t");
                    break;
            }

            // 打印优先级
            printf("%d\n", processes[i].priority);
        }
    }
    printf("---------------------------------------------\n");
}

void list_resource_status() {
    printf("------------------------------\n");
    printf("Resource\tTotal\tAvailable\n");
    printf("------------------------------\n");
    for (int i = 0; i < MAX_RESOURCES; i++) {
        printf("%s\t\t%d\t%d\n", resources[i].name, resources[i].total, resources[i].available);
    }
    printf("------------------------------\n");
}


void shell() {
    char command[100];
    while (true) {
        printf("> ");
        fgets(command, 100, stdin);

        if (strncmp(command, "create", 6) == 0) {
            char name[20];
            int priority;
            sscanf(command, "create %s %d", name, &priority);
            create_process(name, (Priority)priority);
        } else if (strncmp(command, "destroy", 7) == 0) {
            int pid;
            sscanf(command, "destroy %d", &pid);
            destroy_process(pid);
        } else if (strncmp(command, "activate", 8) == 0) {
            int pid;
            sscanf(command, "activate %d", &pid);
            activate_process(pid);
        } else if (strncmp(command, "request ", 8) == 0) {
            int pid;
            char resource[10];
            sscanf(command, "request %d %s", &pid, resource);
            request_resource(pid, resource);
        } else if (strncmp(command, "release", 7) == 0) {
            int pid;
            char resource[10];
            sscanf(command, "release %d %s", &pid, resource);
            release_resource(pid, resource);
        } else if (strncmp(command, "request_io", 10) == 0) {
            int pid;
            sscanf(command, "request_io %d", &pid);
            request_io(pid);
        } else if (strncmp(command, "io_complete", 11) == 0) {
            int pid;
            sscanf(command, "io_complete %d", &pid);
            io_completion(pid);
        } else if (strncmp(command, "timeout", 7) == 0) {
            int pid;
            sscanf(command, "timeout %d", &pid);
            timeout(pid);
        } else if (strncmp(command, "list_p", 6) == 0) {
            int pid;
            sscanf(command, "list_process");
            list_process_status(pid);
        } else if (strncmp(command, "list_r", 6) == 0) {
            int pid;
            sscanf(command, "list_resource");
            list_resource_status(pid);
        } else {
            printf("Unknown command\n");
        }
    }
}

void initialize_resources() {
    // 初始化资源 A
    strcpy(resources[0].name, "A");
    resources[0].total = 1;
    resources[0].available = resources[0].total;

    // 初始化资源 B
    strcpy(resources[1].name, "B");
    resources[1].total = 1;
    resources[1].available = resources[1].total;

    // 初始化资源 C
    strcpy(resources[2].name, "C");
    resources[2].total = 1;
    resources[2].available = resources[2].total;

    printf("Resources A、B、C initialized successfully.\n");

}

int main() {    
    
    initialize_resources();  // 初始化资源 A、B、C

    shell();

}