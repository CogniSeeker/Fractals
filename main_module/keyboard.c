/*
 * Filename: keyboard.c
 * Author:   Jan Faigl, Oleh Borys
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "keyboard.h"
#include "computation.h"
#include "gui.h"
// #include "computation.c"

#include "utils.h"
#include "event_queue.h"

void* keyboard_thread(void* d) {
    call_termios(0);
    fprintf(stderr, "keyboard_thread - start\n");

    char c;
    event ev;
    bool is_set = false;
    char command;
    while (!is_set && ( c = getchar()) != 'q') {
        ev.type = EV_TYPE_NUM;
        switch (c) {
        case 'g':
            // get version
            ev.type = EV_GET_VERSION;
            break;
        case 'q':
            // wait for q
            break;
        case 'a':
            // abort
            ev.type = EV_ABORT;
            break;
        case 's':
            // set compute
            if (is_computing()) {
                warn("New computation parameters requested but it is discarded due to on ongoing computation\n");
                break;
            }
            is_set = true;
            info("CURRENT CONFIG FOR FRACTAL:");
            get_info_config();
            do {
                fprintf(stderr, "\n");
                info("Setting parameters using command - press a command form list below:\n");
                fprintf(stderr, "- 'w': change width of window\n");
                fprintf(stderr, "- 'h': change height of window\n");
                fprintf(stderr, "- 'f': change full size of window\n");
                fprintf(stderr, "- 'c': change c constant\n");
                fprintf(stderr, "- 'n': change fractal depth\n");
                fprintf(stderr, "- 'z': set range of z number\n");
                fprintf(stderr, "- 'i': print out info about current config\n");
                fprintf(stderr, "- 'q': return back\n\n");
                ssize_t read;
                int width, height, depth_n;
                double c_re_part, c_im_part;
                double z_re_min, z_re_max, z_im_min, z_im_max;
                read = scanf("%c", &command);
                if (read == 1) {
                    call_termios(1);
                    switch (command) {
                    case 'w':
                        info("Set width of window: [width]");
                        fprintf(stderr, "Enter only divisible by 10. For. ex: 350\n\n");
                        read = scanf("%d", &width);
                        if (read == 1) {
                            setParameterValue("grid_w", width);
                        } else {
                            warn("Invalid input");
                        }
                        break;
                    case 'h':
                        info("Set height of window: [height]");
                        fprintf(stderr, "Enter only divisible by 10. For. ex: 450\n\n");
                        read = scanf("%d", &height);
                        if (read == 1) {
                            setParameterValue("grid_h", height);
                        } else {
                            warn("Invalid input");
                        }
                        break;
                    case 'f':
                        info("Set full size of window: [width] [height]");
                        fprintf(stderr, "Enter only divisible by 10. For. ex: 350 450\n\n");
                        read = scanf("%d %d", &width, &height);
                        if (read == 2) {
                            setParameterValue("grid_w", width);
                            setParameterValue("grid_h", height);
                        } else {
                            warn("Invalid input");
                        }
                        break;
                    case 'c':
                        info("Set c constant: [c_re_part] [c_im_part]");
                        fprintf(stderr, "For. ex: -0.4 1.5\n\n");
                        read = scanf("%lf %lf", &c_re_part, &c_im_part);
                        if (read == 2) {
                            setParameterValue("c_re", c_re_part);
                            setParameterValue("c_im", c_im_part);
                        } else {
                            warn("Invalid input");
                        }
                        break;
                    case 'n':
                        info("Set fractal depth: [depth]");
                        fprintf(stderr, "For. ex: 60\n\n");
                        read = scanf("%d", &depth_n);
                        if (read == 1) {
                            setParameterValue("n", depth_n);
                        } else {
                            warn("Invalid input");
                        }
                        break;
                    case 'z':
                        info("Set range of z number: [z_re_min] [z_re_max] [z_im_min] [z_im_max]");
                        fprintf(stderr, "For. ex: -1.6 1.2 1.5 0.2\n\n");
                        read = scanf("%lf %lf %lf %lf", &z_re_min, &z_re_max, &z_im_min, &z_im_max);
                        if (read == 4) {
                            setParameterValue("range_re_min", z_re_min);
                            setParameterValue("range_re_max", z_re_max);
                            setParameterValue("range_im_min", z_im_min);
                            setParameterValue("range_im_max", z_im_max);
                        } else {
                            warn("Invalid input");
                        }
                        break;
                    case 'i':
                        info("CURRENT CONFIG FOR FRACTAL:");
                        get_info_config();
                        break;
                    default:
                        break;
                    }
                    // reinitialize window
                    if (command == 'q') {
                        break;
                    }
                    call_termios(0);
                } else {
                    warn("Invalid command, try again");
                }
            } while (1);
            call_termios(0);
            ev.type = EV_SET_COMPUTE;
            is_set = false;
            break;
        case 'c':
            // compute
            if (is_computing() && !is_abort()) {
                warn("WARN: New computation requested but it is discarded due on ongoing computation");
                break;
            }
            ev.type = EV_COMPUTE;
            break;
        case 'r':
            // reset
            ev.type = EV_RESET;
            break;
        default:
            break;
        } // end switch
        if (ev.type != EV_TYPE_NUM) {
            queue_push(ev);
        }
    } // end while
    set_quit();
    ev.type = EV_QUIT;
    queue_push(ev);
    call_termios(1);
    fprintf(stderr, "keyboard_thread - finished\n");
    return NULL;
}

/* end of keyboard.c */