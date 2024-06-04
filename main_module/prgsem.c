/*
 Filename: prgsem.c

 * Project: Multithreaded Fractal Julia Set Renderer
 * This program calculates and renders the Julia Set fractal using multithreading and two pipes.
 *
 * Directory Structure:
 * - main_module: Manages user input and program control flow.
 * - comp_module: Responsible for the Julia Set calculation.
 * DONT MOVE ANY FILES OR FOLDERS to correctly compile the program
 *
 * Extra functionality:
 * - Parameter Configuration: Set parameters
 *  (`z number`, `c constant`, `n depth`, `width`, `height`)
 *  to influence the output.
 * - Abort Calculation: Ability to abort the calculation process.
 * - Version Inquiry: Command the program to provide its current version.
 * - SDL Communication: Program integrates with SDL for visualization and interaction.
 *
 * Build and Run:
 * Each module has a makefile for easy compilation. Navigate to the respective directory and use `make` command.
 *
 * Copyright: the bigger part of code was taken from tutorial by prof. Faigl. prgsem_comp_module.c was written completely by me.
 * Author: Oleh Borys
*/

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

#include "utils.h"
#include "main.h"
#include "keyboard.h"
#include "prg_io_nonblock.h"
#include "gui.h"

#include "event_queue.h"

#ifndef IO_READ_TIMEOUT_MS
#define IO_READ_TIMEOUT_MS 100
#endif

void* read_pipe_thread(void*);
void print_program_info();

int main(int argc, char *argv[]) {
    int ret = EXIT_SUCCESS;
    const char *fname_pipe_in = argc > 1 ? argv[1] : "/tmp/computational_module.out";
    const char *fname_pipe_out = argc > 2 ? argv[2] : "/tmp/computational_module.in";

    int pipe_in = io_open_read(fname_pipe_in);
    int pipe_out = io_open_write(fname_pipe_out);

    my_assert(pipe_in != -1 && pipe_out != -1, __func__, __LINE__, __FILE__);

    if (pipe_in == -1) {
        fprintf(stderr, "ERROR: Cannot open serial port %s\n", fname_pipe_in);
    }
    if (pipe_out == -1) {
        fprintf(stderr, "ERROR: Cannot open serial port %s\n", fname_pipe_out);
    }

    enum { KEYBOARD_THRD, READ_PIPE_THRD, MAIN_THRD, WIN_THRD, NUM_THREADS };
    const char *thrd_names[] = { "Keyboard", "ReadPipe", "Main", "GuiWin" };
    void* (*thrd_functions[])(void*) = { keyboard_thread, read_pipe_thread, main_thread, gui_win_thread };
    pthread_t threads[NUM_THREADS];
    void* thrd_data[NUM_THREADS] = {};
    thrd_data[READ_PIPE_THRD] = &pipe_in;
    thrd_data[MAIN_THRD] = &pipe_out;

    fprintf(stderr, "//-----------------//\n");
    for (int i = 0; i < NUM_THREADS; ++i) {
      int r = pthread_create(&threads[i], NULL, thrd_functions[i], thrd_data[i]);
      printf("Create thread '%s' %s\r\n", thrd_names[i], (r == 0 ? "OK" : "FAIL"));
    }
    fprintf(stderr, "//-----------------//\n");
    print_program_info();
    int *ex;
    for (int i = 0; i < NUM_THREADS; ++i) {
      printf("Call join to the thread %s\r\n", thrd_names[i]);
      int r = pthread_join(threads[i], (void*)&ex);
      printf("Joining the thread %s has been %s\r\n", thrd_names[i], (r == 0 ? "OK" : "FAIL"));
    }

    io_close(pipe_in);
    io_close(pipe_out);
    return ret;
}
void* read_pipe_thread(void* d) {

    my_assert(d != NULL, __func__, __LINE__, __FILE__);
    int pipe_in = *(int*)d;
    fprintf(stderr, "read_pipe_thread - start\n");
    bool end = false;
    uint8_t msg_buf[sizeof(message)];
    int i = 0;
    int len = 0;

    unsigned char c;
    // discard garbage
    while (io_getc_timeout(pipe_in, IO_READ_TIMEOUT_MS, &c) > 0) {
    }
    
    while (!end) {
        int r = io_getc_timeout(pipe_in, IO_READ_TIMEOUT_MS, &c);
        if (r > 0) {
            // char has been read
            if (i == 0) {
                len = 0;
                if (get_message_size(c, &len)) {
                     msg_buf[i++] = c;
                } else {
                    fprintf(stderr, "ERROR: uknown message type 0x%x\n", c);
                }
            } else {
                // read remaining bytes of the message
                msg_buf[i++] = c;
            }
            if (len > 0 && i == len) {
                message *msg = my_alloc(sizeof(message));
                if (parse_message_buf(msg_buf, len, msg)) {
                    event ev = { .type = EV_PIPE_IN_MESSAGE };
                    ev.data.msg = msg;
                    queue_push(ev);
                } else {
                    fprintf(stderr, "ERROR: cannot parse message type %d\n", msg_buf[0]);
                    free(msg);
                }
                i = len = 0;
            }
        } else if (r == 0) {
            // timeout, do nothing
        } else {
            // handle error, quit
            fprintf(stderr, "ERROR reading from pipe\n");
            set_quit();
            event ev = { .type = EV_QUIT };
            queue_push(ev);
        }

        // quit is already mutexed
        end = is_quit();
    } // end while

    fprintf(stderr, "read_pipe_thread - finished\n"); 
    return NULL;
}
void print_program_info() {
    fprintf(stderr, "Project: Multithreaded Fractal Julia Set Renderer\n");
    fprintf(stderr, "This program calculates and renders the Julia Set fractal using multithreading.\n\n");
    fprintf(stderr, "Directory Structure:\n");
    fprintf(stderr, "- main_module: Manages user input and program control flow.\n");
    fprintf(stderr, "- comp_module: Responsible for the Julia Set calculation.\n\n");
    fprintf(stderr, "Key Features:\n");
    fprintf(stderr, "- Parameter Configuration: Set parameters (z, c, n, width, height) to influence the output.\n");
    fprintf(stderr, "- Abort Calculation: Ability to abort the calculation process.\n");
    fprintf(stderr, "- Version Inquiry: Command the program to provide its current version.\n");
    fprintf(stderr, "- SDL Communication: Program integrates with SDL for visualization and interaction.\n\n");
    fprintf(stderr, "Build and Run:\n");
    fprintf(stderr, "Each module has a makefile for easy compilation. Navigate to the respective directory and use `make` command.\n\n");
    fprintf(stderr, "Contributions and improvements are welcome.\n");
    fprintf(stderr, "Please refer to LICENSE for licensing information.\n\n");

    fprintf(stderr, "Program Controls:\n");
    fprintf(stderr, "- 'c': Compute the fractal.\n");
    fprintf(stderr, "- 's': Set parameters.\n");
    fprintf(stderr, "- 'a': Abort the current computation.\n");
    fprintf(stderr, "- 'g': Get the version of the module.\n");
    fprintf(stderr, "- 'q': Quit the program.\n\n");
    fprintf(stderr, "Extra functionalities: \n");
    fprintf(stderr, "- Set parameters using 's' such as: the window sizes, the z number, the c constant, and the depth n.\n");
    fprintf(stderr, "- Interact with aplication using the same commands in SDL window\n");
    fprintf(stderr, "- Image of fractal is saved in 'frame' directory after every complite calculation of fractal (max two images)\n");
    fprintf(stderr, "//-----------------//\n\n");
}
/* end of prgsem.c */