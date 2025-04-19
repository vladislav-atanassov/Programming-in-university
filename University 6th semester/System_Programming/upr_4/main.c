// Standard and system headers
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <sys/ipc.h>
#include <sys/msg.h>

// Constants
#define MAX_CHILDREN 3
#define TEMP_FILE_P3 "/tmp/float_data.txt"
#define LOG_FILE "log_file.txt"

// Message buffer structure for System V message queue
struct msgbuf
{
    long mtype;
    char data[256];
};

// Global variables
int child_pids[MAX_CHILDREN] = {0}; // Stores child process IDs
int watcher_pid = 0;                // PID of the watcher process
int watcher_pipe[2];                // Pipe for child-to-watcher communication
int prompt_pipe[2];                 // Pipe to trigger prompt input from user
int watcher_active = 0;             // Whether watcher is running

int msgq_id;         // ID of the System V message queue
sigset_t block_mask; // Mask used to block signals
struct sigaction sa; // Signal action struct for SIGUSR1

// Prints colored text using ANSI escape codes
void print_colored(const char *message, int fg, int bg)
{
    printf("\033[3%dm\033[4%dm%s\033[0m\n", fg, bg, message);
    fflush(stdout);
}

// Check if all child processes are stopped
int all_children_stopped()
{
    for (int i = 0; i < MAX_CHILDREN; i++)
    {
        if (child_pids[i] != 0)
            return 0;
    }
    return 1;
}

// Stop the watcher process if it's active
void stop_watcher()
{
    if (watcher_active)
    {
        kill(watcher_pid, SIGTERM);
        waitpid(watcher_pid, NULL, 0);
        watcher_active = 0;
        printf("Watcher stopped automatically (PID %d).\n", watcher_pid);
    }
}

// Watcher process that listens for messages, file updates, and pipe input
void start_watcher()
{
    close(watcher_pipe[1]);
    fcntl(watcher_pipe[0], F_SETFL, O_NONBLOCK); // Non-blocking read

    FILE *log = fopen(LOG_FILE, "a"); // Log file for watcher outputs
    if (!log)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    struct msgbuf msg;
    char pipe_buf[256];

    while (1)
    {
        // Check pipe for new messages
        ssize_t n = read(watcher_pipe[0], pipe_buf, sizeof(pipe_buf) - 1);
        if (n > 0)
        {
            pipe_buf[n] = '\0';
            fprintf(log, "Pipe: %s", pipe_buf);
        }

        // Check if float data file has been written to
        FILE *fp = fopen(TEMP_FILE_P3, "r");
        if (fp)
        {
            char line[256];
            if (fgets(line, sizeof(line), fp))
            {
                if (strlen(line) > 0)
                {
                    fprintf(log, "File: %s", line);
                    freopen(TEMP_FILE_P3, "w", fp); // Clear file after reading
                }
            }
            fclose(fp);
        }

        // Check message queue for new messages
        if (msgrcv(msgq_id, &msg, sizeof(msg.data), 1, IPC_NOWAIT) > 0)
        {
            fprintf(log, "MsgQ: %s", msg.data);
        }

        fflush(log);
        usleep(100000); // Sleep to reduce CPU usage
    }
    fclose(log);
    exit(0);
}

// SIGUSR1 handler — used for child-to-parent notifications
void handle_sigusr1(int sig, siginfo_t *si, void *ucontext)
{
    (void)ucontext;
    if (sig == SIGUSR1)
    {
        int index = si->si_value.sival_int;
        write(prompt_pipe[1], &index, sizeof(index)); // Notify main loop
    }
}

// Called by main when user input is needed
void process_input_request(int index)
{
    char buffer[256], input[256];
    const char *prompt_msg = NULL;

    sigprocmask(SIG_BLOCK, &block_mask, NULL); // Block signals during prompt

    // Lock STDOUT to prevent multiple children from printing at once
    struct flock fl = {.l_type = F_WRLCK, .l_whence = SEEK_SET, .l_start = 0, .l_len = 0};
    fcntl(STDOUT_FILENO, F_SETLKW, &fl);

    write(STDOUT_FILENO, "\n", 1);

    // Display the correct prompt based on index
    switch (index)
    {
    case 0:
        prompt_msg = "Process 2 - Enter integer: ";
        break;
    case 1:
        prompt_msg = "Process 3 - Enter float: ";
        break;
    case 2:
        prompt_msg = "Process 4 - Enter string: ";
        break;
    }

    if (prompt_msg)
    {
        write(STDOUT_FILENO, prompt_msg, strlen(prompt_msg));
        ssize_t input_len = read(STDIN_FILENO, input, sizeof(input) - 1);

        if (input_len > 0)
        {
            input[input_len] = '\0'; // Null-terminate the input

            switch (index)
            {
            case 0:
                // Send integer to watcher through pipe
                snprintf(buffer, sizeof(buffer), "2:%d\n", atoi(input));
                write(watcher_pipe[1], buffer, strlen(buffer));
                break;
            case 1:
                // Write float to a temporary file
                FILE *fp = fopen(TEMP_FILE_P3, "w");
                if (fp)
                {
                    fprintf(fp, "3:%.6f\n", atof(input));
                    fclose(fp);
                }
                break;
            case 2:
                // Send string to message queue
                struct msgbuf msg;
                msg.mtype = 1;
                snprintf(msg.data, sizeof(msg.data), "4:%s\n", input);
                msgsnd(msgq_id, &msg, sizeof(msg.data), 0);
                break;
            }
        }
    }

    // Unlock STDOUT and unblock signals
    fl.l_type = F_UNLCK;
    fcntl(STDOUT_FILENO, F_SETLK, &fl);
    sigprocmask(SIG_UNBLOCK, &block_mask, NULL);
}

// Loop function for child processes
void child_loop(int index, int delay_ms)
{
    union sigval value;
    value.sival_int = index;
    sigqueue(getppid(), SIGUSR1, value); // Initial signal to prompt input

    while (1)
    {
        usleep(delay_ms * 1000);             // Sleep before next signal
        sigqueue(getppid(), SIGUSR1, value); // Request input again
    }
}

// Stop child process and update tracking
void stop_process(int index)
{
    if (child_pids[index])
    {
        kill(child_pids[index], SIGTERM);    // Signal from the child to self-terminate
        waitpid(child_pids[index], NULL, 0); // Wait to actually terminate

        printf("Process %d stopped (PID %d).\n", index + 2, child_pids[index]);
        child_pids[index] = 0;

        if (all_children_stopped())
        {
            stop_watcher();
        }
    }
    else
    {
        printf("Process %d not running.\n", index + 2);
    }
}

int main()
{
    // Setup signal handler for SIGUSR1
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = handle_sigusr1;
    sa.sa_flags = SA_SIGINFO | SA_RESTART;
    sigaction(SIGUSR1, &sa, NULL);

    // Setup signal blocking mask for prompts
    sigemptyset(&block_mask);
    sigaddset(&block_mask, SIGUSR1);

    // Create message queue
    msgq_id = msgget(IPC_PRIVATE, 0666 | IPC_CREAT);

    // Create pipes for communication
    pipe(watcher_pipe);
    pipe(prompt_pipe);
    fcntl(prompt_pipe[0], F_SETFL, O_NONBLOCK);

    const char *menu = "\n-- MENU --\n"
                       "1. Start process 2\n"
                       "2. Start process 3\n"
                       "3. Start process 4\n"
                       "4. Stop process 2\n"
                       "5. Stop process 3\n"
                       "6. Stop process 4\n"
                       "7. Exit\nChoice: ";

    write(STDOUT_FILENO, menu, strlen(menu));

    int choice;
    while (1)
    {
        // Use select to wait for either stdin input or pipe activity
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        FD_SET(prompt_pipe[0], &readfds);
        int max_fd = (STDIN_FILENO > prompt_pipe[0]) ? STDIN_FILENO : prompt_pipe[0];

        int ready = select(max_fd + 1, &readfds, NULL, NULL, NULL);
        if (ready == -1)
        {
            if (errno == EINTR)
                continue;
            perror("select");
            exit(EXIT_FAILURE);
        }

        if (FD_ISSET(prompt_pipe[0], &readfds))
        {
            int index;
            while (read(prompt_pipe[0], &index, sizeof(index)) > 0)
            {
                process_input_request(index); // Handle signal from child
            }
            continue;
        }

        if (FD_ISSET(STDIN_FILENO, &readfds))
        {
            char input[128];
            ssize_t n = read(STDIN_FILENO, input, sizeof(input) - 1);
            if (n <= 0)
                continue;
            input[n] = '\0';
            choice = atoi(input);

            if (choice >= 1 && choice <= 3)
            {
                int idx = choice - 1;
                if (child_pids[idx])
                {
                    printf("Process %d already running.\n", idx + 2);
                }
                else // Initial start of the process
                {
                    int fg, bg, delay;
                    printf("Enter text color (0-7): ");
                    scanf("%d", &fg);
                    printf("Enter background color (0-7): ");
                    scanf("%d", &bg);
                    printf("Enter delay in ms: ");
                    scanf("%d", &delay);

                    while (getchar() != '\n') // Clear stdin
                    {
                    }

                    // Create a child process
                    int pid = fork();
                    if (pid == 0)
                    {
                        child_loop(idx, delay);
                    }
                    child_pids[idx] = pid;

                    char msg[100];
                    snprintf(msg, sizeof(msg), "Process %d started (PID %d)", idx + 2, pid);
                    print_colored(msg, fg, bg);

                    if (!watcher_active)
                    {
                        watcher_pid = fork();
                        if (watcher_pid == 0)
                            start_watcher();
                        watcher_active = 1;
                        printf("Watcher started (PID %d)\n", watcher_pid);
                    }
                }
            }
            else if (choice >= 4 && choice <= 6)
            {
                stop_process(choice - 4);
            }
            else if (choice == 7)
            {
                int can_exit = 1;
                for (int i = 0; i < MAX_CHILDREN; i++)
                {
                    if (child_pids[i] != 0)
                    {
                        can_exit = 0;
                        printf("Process %d is still running! Stop it first.\n", i + 2);
                    }
                }

                if (!can_exit)
                {
                    printf("Cannot exit while processes are active.\n");
                    write(STDOUT_FILENO, menu, strlen(menu));
                    continue;
                }

                stop_watcher();
                msgctl(msgq_id, IPC_RMID, NULL); // Clean up message queue
                printf("Program ended.\n");
                break;
            }
            else
            {
                printf("Invalid choice.\n");
            }
            write(STDOUT_FILENO, menu, strlen(menu));
        }
    }
    return 0;
}
