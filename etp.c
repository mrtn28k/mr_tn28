//a1
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    char buffer[10];
    int fd = open("file.txt", O_RDONLY);
    if (fd < 0) {
        perror("open");
        exit(1);
    }

    off_t offset = lseek(fd, -10, SEEK_END);
    if (offset < 0) {
        perror("lseek");
        exit(1);
    }

    ssize_t nread = read(fd, buffer, 10);
    if (nread < 0) {
        perror("read");
        exit(1);
    }

    printf("Last 10 characters: %.*s\n", nread, buffer);

    close(fd);
    return 0;
}
//1b
#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid;

    pid = fork(); // create a child process

    if (pid < 0) { // error occurred
        fprintf(stderr, "Fork Failed\n");
        return 1;
    }
    else if (pid == 0) { // child process
        printf("Child Process ID: %d\n", getpid());
    }
    else { // parent process
        printf("Parent Process ID: %d\n", getpid());
    }

    return 0;
}
//2a
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *print_welcome(void *arg) {
    printf("Welcome to the program!\n");
    pthread_exit(NULL);
}

void *add_numbers(void *arg) {
    int *args = (int *)arg;
    int num1 = args[0];
    int num2 = args[1];
    int sum = num1 + num2;
    printf("Sum of %d and %d is %d\n", num1, num2, sum);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[2];
    int thread_args[2] = { 10, 20 }; // numbers to add

    // create threads
    int rc1 = pthread_create(&threads[0], NULL, print_welcome, NULL);
    int rc2 = pthread_create(&threads[1], NULL, add_numbers, (void *)thread_args);

    // check for errors
    if (rc1 || rc2) {
        fprintf(stderr, "Error creating threads\n");
        exit(1);
    }

    // wait for threads to finish
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    return 0;
}
//2b
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 2

pthread_t threads[NUM_THREADS];
int counter = 0;
pthread_mutex_t lock;

void *thread_function(void *arg) {
    int i;
    printf("Thread %ld starting...\n", (long)arg);
    for (i = 0; i < 5; i++) {
        pthread_mutex_lock(&lock);
        counter++;
        printf("Thread %ld: counter = %d\n", (long)arg, counter);
        pthread_mutex_unlock(&lock);
    }
    printf("Thread %ld exiting...\n", (long)arg);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int i;
    pthread_mutex_init(&lock, NULL);

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, thread_function, (void *)(intptr_t)i);
    }

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&lock);
    return 0;
}
//3a
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int main() {
    char write_msg[BUFFER_SIZE] = "Hello, world!";
    char read_msg[BUFFER_SIZE];
    int fd[2];
    pid_t pid;

    // create the pipe
    if (pipe(fd) == -1) {
        fprintf(stderr, "Pipe failed");
        return 1;
    }

    // fork a child process
    pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork failed");
        return 1;
    }

    if (pid > 0) {  // parent process
        // close the unused write end of the pipe
        close(fd[WRITE_END]);

        // read from the pipe
        read(fd[READ_END], read_msg, BUFFER_SIZE);
        printf("Parent read: %s\n", read_msg);

        // close the read end of the pipe
        close(fd[READ_END]);
    } else {  // child process
        // close the unused read end of the pipe
        close(fd[READ_END]);

        // write to the pipe
        write(fd[WRITE_END], write_msg, strlen(write_msg)+1);

        // close the write end of the pipe
        close(fd[WRITE_END]);
    }

    return 0;
}
//3b
#include <stdio.h>

#define BUFFER_SIZE 6

int main() {
    FILE *fp;
    char buffer[BUFFER_SIZE];
    long file_size;

    // open the file in read-only mode
    fp = fopen("test.txt", "r");

    if (fp == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    // seek to the end of the file
    fseek(fp, 0L, SEEK_END);

    // get the file size
    file_size = ftell(fp);

    // read the last 5 characters from the file
    fseek(fp, -5L, SEEK_END);
    fread(buffer, 1, BUFFER_SIZE-1, fp);

    // null-terminate the buffer
    buffer[BUFFER_SIZE-1] = '\0';

    // print the last 5 characters
    printf("Last 5 characters: %s\n", buffer);

    // close the file
    fclose(fp);

    return 0;
}
//4a
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

#define BUFFER_SIZE 25

int main() {
    char write_msg[BUFFER_SIZE] = "Hello, world!";
    char read_msg[BUFFER_SIZE];
    int fd;
    pid_t pid;

    // create the named pipe
    mkfifo("/tmp/myfifo", 0666);

    // fork a child process
    pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork failed");
        return 1;
    }

    if (pid > 0) {  // parent process
        // open the named pipe for writing
        fd = open("/tmp/myfifo", O_WRONLY);

        // write to the named pipe
        write(fd, write_msg, strlen(write_msg)+1);

        // close the named pipe
        close(fd);
    } else {  // child process
        // open the named pipe for reading
        fd = open("/tmp/myfifo", O_RDONLY);

        // read from the named pipe
        read(fd, read_msg, BUFFER_SIZE);
        printf("Child read: %s\n", read_msg);

        // close the named pipe
        close(fd);
    }

    return 0;
}
//4b
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main() {
    char *filename = "test.txt";
    char *content[] = {
        "This is the first line.",
        "This is the second line.",
        "This is the third line."
    };
    int fd, n, i;

    // create the file
    fd = open(filename, O_WRONLY | O_CREAT, 0666);

    if (fd < 0) {
        printf("Error creating file.\n");
        return 1;
    }

    // write the content to the file
    for (i = 0; i < 3; i++) {
        n = strlen(content[i]);
        write(fd, content[i], n);
        write(fd, "\n", 1);
    }

    // seek to the 4th character in the file
    lseek(fd, 4, SEEK_SET);

    // write "hello" to the file
    write(fd, "hello", 5);

    // close the file
    close(fd);

    return 0;
}
//5a
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

#define NUM_CHILDREN 2

int main() {
    pid_t pid;
    int i;

    // fork multiple child processes
    for (i = 0; i < NUM_CHILDREN; i++) {
        pid = fork();

        if (pid < 0) {
            fprintf(stderr, "Fork failed");
            return 1;
        } else if (pid == 0) {  // child process
            // read from the file
            int fd = open("counter.txt", O_RDONLY);
            if (fd < 0) {
                fprintf(stderr, "Error opening file");
                return 1;
            }

            int counter;
            read(fd, &counter, sizeof(int));

            // increment the counter
            counter++;

            // write the counter back to the file
            lseek(fd, 0, SEEK_SET);
            write(fd, &counter, sizeof(int));

            close(fd);
            exit(0);
        }
    }

    // wait for all child processes to finish
    for (i = 0; i < NUM_CHILDREN; i++) {
        wait(NULL);
    }

    // read the final value of the counter from the file
    int fd = open("counter.txt", O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "Error opening file");
        return 1;
    }

    int counter;
    read(fd, &counter, sizeof(int));
    printf("Final counter value: %d\n", counter);

    close(fd);

    return 0;
}
//5b
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid1, pid2;
    int num1 = 5, num2 = 10, sum;

    // create first child process
    pid1 = fork();

    if (pid1 < 0) {
        fprintf(stderr, "Fork failed");
        return 1;
    } else if (pid1 == 0) {  // first child process
        // wait for parent process to finish adding the numbers
        wait(NULL);

        // display the sum calculated by the parent process
        printf("Child process 1: Sum of %d and %d is %d\n", num1, num2, sum);

        exit(0);
    } else {  // parent process
        // create second child process
        pid2 = fork();

        if (pid2 < 0) {
            fprintf(stderr, "Fork failed");
            return 1;
        } else if (pid2 == 0) {  // second child process
            // wait for parent process to finish adding the numbers
            wait(NULL);

            // display the sum calculated by the parent process
            printf("Child process 2: Sum of %d and %d is %d\n", num1, num2, sum);

            exit(0);
        } else {  // parent process
            // calculate the sum of the two numbers
            sum = num1 + num2;

            // wait for both child processes to finish
            wait(NULL);
            wait(NULL);
        }
    }

    return 0;
}
//6a
#include <iostream>
#include <pthread.h>
using namespace std;
void *add(void *arg)
{
 int *nums = (int *)arg;
 int result = nums[0] + nums[1];
 return (void *)result;
}
void *subtract(void *arg)
{
 int *nums = (int *)arg;
 int result = nums[0] - nums[1];
 return (void *)result;
}
int main()
{
 int a = 10, b = 5;
 pthread_t tid[2];
 void *thread_result[2];
 pthread_create(&tid[0], NULL, add, &a);
 pthread_create(&tid[1], NULL, subtract, &a);
 pthread_join(tid[0], &thread_result[0]);
 pthread_join(tid[1], &thread_result[1]);
 int c = *(int *)thread_result[0];
 int d = *(int *)thread_result[1];
 cout << "Product of " << c << " and " << d << " is " << c * d << endl;
 return 0;
}
//6b
#include <stdio.h>
int main()
{
 FILE *fp;
 char buffer[1024];
 fp = popen("ls", "r");
 while (fgets(buffer, sizeof(buffer), fp) != NULL)
 {
 printf("%s", buffer);
 }
 pclose(fp);
 return 0;
}
//7a
#include <stdio.h>
#include <pthread.h>
void *print_numbers(void *arg)
{
 for (int i = 1; i <= 10; i++)
 {
 printf("%d ", i);
 }
 printf("\n");
 pthread_exit(NULL);
}
void *check_even_odd(void *arg)
{
 int *num = (int *)arg;
 if (*num % 2 == 0)
 {
 printf("%d is even\n", *num);
 }
 else
 {
 printf("%d is odd\n", *num);
 }
 pthread_exit(NULL);
}
int main()
{
 pthread_t tid[2];
 int num;
 printf("Enter a number: ");
 scanf("%d", &num);
 pthread_create(&tid[0], NULL, print_numbers, NULL);
 pthread_create(&tid[1], NULL, check_even_odd, &num);
 pthread_join(tid[0], NULL);
 pthread_join(tid[1], NULL);
 return 0;
}
//7b
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
int main()
{
 int fd;
 char buffer[8];
 fd = open("file.txt", O_RDONLY);
 if (fd == -1)
 {
 printf("Error opening file\n");
 exit(EXIT_FAILURE);
 }
 lseek(fd, 2, SEEK_SET);
 read(fd, buffer, 8);
 printf("%.*s\n", 8, buffer);
 close(fd);
 return 0;
}
//8a
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
int main()
{
 int fd;
 char buffer[6];
 fd = open("file.txt", O_RDONLY);
 if (fd == -1)
 {
 printf("Error opening file\n");
 exit(EXIT_FAILURE);
 }
 lseek(fd, 9, SEEK_SET);
 read(fd, buffer, 5);
 printf("%.*s\n", 5, buffer);
 close(fd);
 return 0;
}
//8b
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
int count = 0;
sem_t sem;
void *increment(void *arg)
{
 for (int i = 0; i < 1000000; i++)
 {
 sem_wait(&sem);
 count++;
 sem_post(&sem);
 }
 pthread_exit(NULL);
}
int main()
{
 pthread_t tid[2];
 sem_init(&sem, 0, 1);
 pthread_create(&tid[0], NULL, increment, NULL);
 pthread_create(&tid[1], NULL, increment, NULL);
 pthread_join(tid[0], NULL);
 pthread_join(tid[1], NULL);
 printf("Final value of count: %d\n", count);
 sem_destroy(&sem);
 return 0;
}
//9a
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
 pid_t pid1, pid2;
 pid1 = fork();
 if (pid1 == 0)
 {
 printf("Child process P2, PID: %d, PPID: %d\n", getpid(), getppid());
 }
 else if (pid1 > 0)
 {
 pid2 = fork();
 if (pid2 == 0)
 {
 printf("Child process P3, PID: %d, PPID: %d\n", getpid(), getppid());
 }
 else if (pid2 > 0)
 {
 printf("Parent process P1, PID: %d, PPID: %d\n", getpid(), getppid());
 }
 else
 {
 printf("Error creating child process P3\n");
 }
 }
 else
 {
 printf("Error creating child process P2\n");
 }
 return 0;
}
//9b
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
int main()
{
 int fd1, fd2;
 char buffer[1024];
 ssize_t num_read, num_written;
 fd1 = open("input.txt", O_RDONLY);
 fd2 = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
 if (fd1 == -1 || fd2 == -1)
 {
 printf("Error opening file\n");
 exit(EXIT_FAILURE);
 }
 while ((num_read = read(fd1, buffer, 1024)) > 0)
 {
 num_written = write(fd2, buffer, num_read);
 if (num_written != num_read)
 {
 printf("Error writing to file\n");
 exit(EXIT_FAILURE);
 }
 }
 if (num_read == -1)
 {
 printf("Error reading from file\n");
 exit(EXIT_FAILURE);
 }
 close(fd1);
 close(fd2);
 return 0;
}
//10a
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define THREAD_COUNT 10
int counter = 0;
pthread_mutex_t mutex;
void* increment_counter(void* arg)
{
 int i;
 for (i = 0; i < 100000; i++)
 {
 pthread_mutex_lock(&mutex);
 counter++;
 pthread_mutex_unlock(&mutex);
 }
 pthread_exit(NULL);
}
int main()
{
 pthread_t threads[THREAD_COUNT];
 int i;
 if (pthread_mutex_init(&mutex, NULL) != 0)
 {
 printf("Error initializing mutex\n");
 exit(EXIT_FAILURE);
 }
 for (i = 0; i < THREAD_COUNT; i++)
 {
 if (pthread_create(&threads[i], NULL, increment_counter, NULL) != 0)
 {
 printf("Error creating thread\n");
 exit(EXIT_FAILURE);
 }
 }
 for (i = 0; i < THREAD_COUNT; i++)
 {
 if (pthread_join(threads[i], NULL) != 0)
 {
 printf("Error joining thread\n");
 exit(EXIT_FAILURE);
 }
 }
 pthread_mutex_destroy(&mutex);
 printf("Final counter value: %d\n", counter);
 return 0;
}
//10b
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
 pid_t pid1, pid2;
 printf("Process P1, PID: %d, PPID: %d\n", getpid(), getppid());
 pid1 = fork();
 if (pid1 == 0)
 {
 printf("Process P2, PID: %d, PPID: %d\n", getpid(), getppid());
 pid2 = fork();
 if (pid2 == 0)
 {
 printf("Process P3, PID: %d, PPID: %d\n", getpid(), getppid());
 }
 else if (pid2 > 0)
 {
 exit(EXIT_SUCCESS);
 }
 else
 {
 printf("Error creating process P3\n");
 exit(EXIT_FAILURE);
 }
 }
 else if (pid1 > 0)
 {
 wait(NULL);
 exit(EXIT_SUCCESS);
 }
 else
 {
 printf("Error creating process P2\n");
 exit(EXIT_FAILURE);
 }
 return 0;
}
