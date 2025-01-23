// Lab-1: Implementation of Process (Creation of Process, Child Process, and Parent Process)

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int pid, status;
    printf("Parent Process: PID = %d\n", getpid());
    pid = fork();

    if (pid > 0) { // Parent process
        printf("Parent: Waiting for child process to terminate...\n");
        wait(&status);
        printf("Parent: Child process terminated with exit code %d\n", WEXITSTATUS(status));
    } else if (pid == 0) { // Child process
        printf("Child Process: PID = %d, Parent PID = %d\n", getpid(), getppid());
        exit(42); // Child exits with a specific code
    } else { // Error
        perror("Fork failed");
    }

    return 0;
}
// Lab-2: Implementation of System Call (open)
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main() {
    int fd = open("foo.txt", O_RDONLY | O_CREAT, 0644);

    if (fd == -1) {
        perror("Error opening file");
        printf("Error Number: %d\n", errno);
    } else {
        printf("File descriptor = %d\n", fd);
        close(fd);
    }

    return 0;
}

// Lab-3: Inter-Process Communication (Using Pipes)
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    int pipefds[2];
    char writemessage1[] = "PU";
    char writemessage2[] = "PURBANCHAL-UNIVERSITY";
    char readmessage[50];

    if (pipe(pipefds) == -1) {
        perror("Unable to create pipe");
        return 1;
    }

    write(pipefds[1], writemessage1, strlen(writemessage1) + 1);
    read(pipefds[0], readmessage, sizeof(readmessage));
    printf("Message 1: %s\n", readmessage);

    write(pipefds[1], writemessage2, strlen(writemessage2) + 1);
    read(pipefds[0], readmessage, sizeof(readmessage));
    printf("Message 2: %s\n", readmessage);

    return 0;
}




// Lab-4: Dining Philosopher’s Problem

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t chopsticks[5];

void *philosopher(void *num) {
    int id = *(int *)num;

    printf("Philosopher %d is thinking.\n", id);
    sem_wait(&chopsticks[id]);
    sem_wait(&chopsticks[(id + 1) % 5]);

    printf("Philosopher %d is eating.\n", id);
    sleep(1); // Simulate eating
    printf("Philosopher %d finished eating.\n", id);

    sem_post(&chopsticks[(id + 1) % 5]);
    sem_post(&chopsticks[id]);

    return NULL;
}

int main() {
    pthread_t threads[5];
    int ids[5];

    for (int i = 0; i < 5; i++) {
        sem_init(&chopsticks[i], 0, 1);
        ids[i] = i;
    }

    for (int i = 0; i < 5; i++)
        pthread_create(&threads[i], NULL, philosopher, &ids[i]);

    for (int i = 0; i < 5; i++)
        pthread_join(threads[i], NULL);

    return 0;
}





// Lab-5: Process Scheduling (Shortest Job First)


#include <stdio.h>

int main() {
    int n, bt[100], wt[100] = {0}, tat[100];
    float avg_wt = 0, avg_tat = 0;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    printf("Enter Burst Time for each process:\n");
    for (int i = 0; i < n; i++) {
        printf("P%d: ", i + 1);
        scanf("%d", &bt[i]);
    }

    // Sort processes by burst time
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (bt[j] > bt[j + 1]) {
                int temp = bt[j];
                bt[j] = bt[j + 1];
                bt[j + 1] = temp;
            }
        }
    }

    for (int i = 1; i < n; i++) {
        wt[i] = wt[i - 1] + bt[i - 1];
        avg_wt += wt[i];
    }

    printf("Process\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < n; i++) {
        tat[i] = bt[i] + wt[i];
        avg_tat += tat[i];
        printf("P%d\t%d\t\t%d\t\t%d\n", i + 1, bt[i], wt[i], tat[i]);
    }

    avg_wt /= n;
    avg_tat /= n;

    printf("Average Waiting Time: %.2f\n", avg_wt);
    printf("Average Turnaround Time: %.2f\n", avg_tat);

    return 0;
}
//Lab-6: Banker’s Algorithm
#include <stdio.h>

int main() {
    int n = 5, m = 3;
    int alloc[5][3] = {{0, 1, 0}, {2, 0, 0}, {3, 0, 2}, {2, 1, 1}, {0, 0, 2}};
    int max[5][3] = {{7, 5, 3}, {3, 2, 2}, {9, 0, 2}, {2, 2, 2}, {4, 3, 3}};
    int avail[3] = {3, 3, 2};
    int f[5] = {0}, safe_seq[5], index = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (max[i][j] - alloc[i][j] > avail[j]) {
                printf("System is not in a safe state.\n");
                return 1;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        if (!f[i]) {
            int flag = 1;
            for (int j = 0; j < m; j++) {
                if (max[i][j] - alloc[i][j] > avail[j]) {
                    flag = 0;
                    break;
                }
            }
            if (flag) {
                for (int j = 0; j < m; j++)
                    avail[j] += alloc[i][j];
                safe_seq[index++] = i;
                f[i] = 1;
                i = -1;
            }
        }
    }

    printf("System is in a safe state. Safe sequence is: ");
    for (int i = 0; i < n; i++)
        printf("P%d ", safe_seq[i]);
    printf("\n");

    return 0;
}







