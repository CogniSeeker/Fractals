/*
 * Filename: prgsem_comp_module.c
 * Author:   Oleh Borys
 */

#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#include "../main_module/prg_io_nonblock.h"
#include "../main_module/utils.h"
#include "../main_module/messages.h"
#include "../main_module/computation.h"
#include "../main_module/event_queue.h"

#include "prgsem_comp_module.h"

#ifndef IO_READ_TIMEOUT_MS
#define IO_READ_TIMEOUT_MS 10000
#endif

// The maximum number of messages that can be waiting to be processed
#define BUFFER_SIZE 10

// Shared data
// The buffer and its associated variables
message buffer[BUFFER_SIZE];
int buffer_start = 0;
int buffer_end = 0;
pthread_mutex_t buffer_mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t buffer_count_sem;

bool first_compute = true;

void push_in_queue_buffer(message *msg);

int main(int argc, char *argv[]) {
    int ret = EXIT_SUCCESS;
    const char *fname_pipe_in = argc > 1 ? argv[1] : "/tmp/computational_module.in";
    const char *fname_pipe_out = argc > 2 ? argv[2] : "/tmp/computational_module.out";

    int pipe_in = io_open_read(fname_pipe_in);
    int pipe_out = io_open_write(fname_pipe_out);


    my_assert(pipe_in != -1 && pipe_out != -1, __func__, __LINE__, __FILE__);

    if (pipe_in == -1) {
        fprintf(stderr, "ERROR: Cannot open serial port %s\n", fname_pipe_in);
    }
    if (pipe_out == -1) {
        fprintf(stderr, "ERROR: Cannot open serial port %s\n", fname_pipe_out);
    }

    enum { READ_INPUT_THRD, COMPUTE_THRD, NUM_THREADS };
    const char *thrd_names[] = { "ReadInput", "Compute" };
    void* (*thrd_functions[])(void*) = { read_input_thread, compute_thread };
    pthread_t threads[NUM_THREADS];
    void* thrd_data[NUM_THREADS] = {};
    thrd_data[READ_INPUT_THRD] = &pipe_in;
    thrd_data[COMPUTE_THRD] = &pipe_out;

    // Initialize the count semaphore
    sem_init(&buffer_count_sem, 0, 0);

    for (int i = 0; i < NUM_THREADS; ++i) {
      int r = pthread_create(&threads[i], NULL, thrd_functions[i], thrd_data[i]);
      printf("Create thread '%s' %s\r\n", thrd_names[i], (r == 0 ? "OK" : "FAIL"));
    }

    int *ex;
    for (int i = 0; i < NUM_THREADS; ++i) {
      printf("Call join to the thread %s\r\n", thrd_names[i]);
      int r = pthread_join(threads[i], (void*)&ex);
      printf("Joining the thread %s has been %s\r\n", thrd_names[i], (r == 0 ? "OK" : "FAIL"));
    }

    io_close(pipe_in);
    io_close(pipe_out);

    sem_destroy(&buffer_count_sem);

    return ret; 

}

void* read_input_thread(void* d) {

    int pipe_in = *(int*)d;

    uint8_t msg_buf[sizeof(message)];
    int i = 0;
    int len = 0;
    unsigned char c;

    message msg_startup = { .data.startup.message = {'B', 'O', 'R', 'Y', 'S', 'O', 'L', 'E'}};
    msg_startup.type = MSG_STARTUP;

    push_in_queue_buffer(&msg_startup);

    while(1) {
        c = io_getc(pipe_in);
        // char has been read
        if (i == 0) {
            len = 0;
            if (get_message_size(c, &len)) {
                msg_buf[i++] = c;
            } else {
                fprintf(stderr, "INFO: unknown message type 0x%x\n", c);
                break;
            }
        } else {
            // read remaining bytes of the message
            msg_buf[i++] = c;
        }
        if (len > 0 && i == len) {
            message *msg = my_alloc(sizeof(message));
            if (parse_message_buf(msg_buf, len, msg)) {

                push_in_queue_buffer(msg);

            } else {
                fprintf(stderr, "ERROR: cannot parse message type %d\n", msg_buf[0]);
                free(msg);
            }
            i = len = 0;
        }
    }
    return NULL;
}

void* compute_thread(void *d) {
    int pipe_out = *(int *)d;
    bool quit = false;
    int x = 0, y = 0;
    int iter = 0;

    do {
        // Wait for the count semaphore to be greater than 0
        sem_wait(&buffer_count_sem);

        // Lock the mutex before accessing the buffer
        pthread_mutex_lock(&buffer_mutex);

        // Remove a message from the buffer
        message msg = buffer[buffer_start];
        buffer_start = (buffer_start + 1) % BUFFER_SIZE;
        // message msg_next = buffer[buffer_start];

        // Unlock the mutex
        pthread_mutex_unlock(&buffer_mutex);

        // set number of chunks for each coord to compute
        int x_max = msg.data.compute.n_re;
        int y_max = msg.data.compute.n_im;

        // set start point to process computations
        double chunk_start_re = msg.data.compute.re;
        double chunk_start_im = msg.data.compute.im;

        switch (msg.type) {
        case MSG_GET_VERSION:
            info("MSG_GET_VERSION was accepted");
            msg.type = MSG_VERSION;
            msg.data.version.major = 2;
            msg.data.version.minor = 1;
            msg.data.version.patch = 0;
            break;
        case MSG_SET_COMPUTE:
            info("MSG_SET_COMPUTE was accepted");
            msg.type = MSG_OK;
            break;
        case MSG_COMPUTE:
            if (!get_compute_state()) {
                break;
            }
            if (first_compute) {
                info("Start to process computation");
                first_compute = false;
            }
            msg.type = MSG_COMPUTE_DATA;
            for (y = 0; y < y_max; ++y) {
                for (x = 0; x < x_max; ++x) {

                    // calculate num of iterations for z sequence to diverge
                    iter = compute_julia_set(x, y, chunk_start_re, chunk_start_im);
                    msg.data.compute_data.iter = iter;
                    msg.data.compute_data.i_re = x;
                    msg.data.compute_data.i_im = y;

                    send_message(pipe_out, &msg);
                }
            }
            msg.type = MSG_DONE;
            send_message(pipe_out, &msg);
            if (!is_done()) {
                msg.type = MSG_OK;
            } else {
                msg.type = MSG_DONE;
            }
            info("MSG_COMPUTE_DATA was sent");
            break;
        case MSG_ABORT:
            info("MSG_ABORT was sent");
            msg.type = MSG_ABORT;
            break;
        case MSG_QUIT:
            info("QUIT was accepted - QUIT COMP MODULE");
            quit = true;
            break;
        case MSG_RESET:
            info("SEND RESET");
            msg.type = MSG_RESET;
            send_message(pipe_out, &msg);
            break;
        default:
            break;
        } // end switch

        // Process the message and write the result to pipe_out
        if (msg.type != MSG_QUIT) {
            send_message(pipe_out, &msg);
        }
    } while (!quit);
    return NULL;
}

void send_message(int pipe_out, const message *msg) {
    // Implement sending back the results
    // Fill the msg, convert it to buffer, then write it to pipe_out
    uint8_t msg_buf[sizeof(message)];
    int msg_len;

    my_assert(fill_message_buf(msg, msg_buf, sizeof(message), &msg_len), __func__, __LINE__, __FILE__);
    if (write(pipe_out, msg_buf, msg_len) == msg_len) {
    } else {
        error("send message fail!");
    }
}
void push_in_queue_buffer(message *msg) {

    // Lock the mutex before accessing the buffer
    pthread_mutex_lock(&buffer_mutex);

    // Add the message to the buffer
    buffer[buffer_end] = *msg;
    buffer_end = (buffer_end + 1) % BUFFER_SIZE;

    // Unlock the mutex
    pthread_mutex_unlock(&buffer_mutex);

    // Increment the count semaphore
    sem_post(&buffer_count_sem);
}


/* end of prgsem_comp_module.c */
