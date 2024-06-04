/*
 * Filename: main.c
 * Author:   Jan Faigl, Oleh Borys
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "main.h"

#include "utils.h"
#include "messages.h"
#include "event_queue.h"
#include "computation.h"
#include "gui.h"

static void process_pipe_message(event * const ev);

void* main_thread(void* d) {

    my_assert(d != NULL, __func__, __LINE__, __FILE__);
    int pipe_out = *(int*)d;
    message msg;
    uint8_t msg_buf[sizeof(message)];
    int msg_len;
    bool quit = false;
    bool first_compute = true;

    // initialize computation, visualization
    computation_init(); 
    gui_init();
    do {
        event ev = queue_pop();
        msg.type = MSG_NBR;
        switch (ev.type) {
        case EV_QUIT:
            // if (is_computing()) {
            //     warn("Quit requested but the computation is going on");
            //     info("If you want to quit - press 'a' to abort computation, then 'q' to quit");
            //     break;
            // }
            msg.type = MSG_QUIT;
            abort_comp();
            set_quit();
            break;
        case EV_GET_VERSION:
            info("INFO: Get version requested");
            msg.type = MSG_GET_VERSION;
            break;
        case EV_SET_COMPUTE:
            gui_cleanup();
            computation_cleanup();
            computation_init();
            gui_init();
            reset_fractal();
            info ( set_compute(&msg) ? "It was set compute" : "Failed set compute" );
            gui_refresh();
            get_info_config();
            // fprintf(stderr, "Parameters: c: %f %f, d: %f %f, w: %d h: %d nbr_chunks: %d\n", msg.data.set_compute.c_re, msg.data.set_compute.c_im,
            //  msg.data.set_compute.d_re,
            //   msg.data.set_compute.d_im,
            //    msg.data.set_compute.grid_w, msg.data.set_compute.grid_h, msg.data.set_compute.nbr_chunks);
            break;
        case EV_COMPUTE:
            if (first_compute) {
                fprintf(stderr, "INFO: New computation chunk id: for part %d x %d, z: %f %f, %f, %f. c: %f %f\n",
                    getParameterValue("chunk_n_re"),
                    getParameterValue("chunk_n_im"),
                    getParameterValueDouble("range_re_min"),
                    getParameterValueDouble("range_re_max"),
                    getParameterValueDouble("range_im_min"),
                    getParameterValueDouble("range_im_max"),
                    getParameterValueDouble("c_re"),
                    getParameterValueDouble("c_im"));
                first_compute = false;
            }
            if (!is_done()) {
                enable_comp();
                // set_compute_state_true();
                if (!compute(&msg)) {
                    info ("fail compute");
                }
            } else {
                warn("Cannot start new computation because fractal is already computed");
                info("If you want to reset the grid, press [r]");
            }
            break;
        case EV_ABORT:
            if (is_computing()) {
                abort_comp();
                msg.type = MSG_ABORT;
            } else {
                warn("Abort requested but it is not computing");
            }
            break;
        case EV_RESET:
            if (is_computing() || (!is_computing() && is_done())) {
                msg.type = MSG_RESET;
            } else {
                warn("Reset requeted but there is not computation in process");
                break;
            }
            break;
        case EV_PIPE_IN_MESSAGE:
            process_pipe_message(&ev);
            break;
        default:
            break;
        } // switch end
        if (msg.type != MSG_NBR) {
            // fill message in message buffer, then write it to pipe_out
            my_assert(fill_message_buf(&msg, msg_buf, sizeof(message), &msg_len), __func__, __LINE__, __FILE__);
            if (write(pipe_out, msg_buf, msg_len) == msg_len) {
                if (msg.type == MSG_QUIT) {
                    fprintf(stderr, "QUIT was SENT!\n");
                }
            } else {
                error("send message fail!");
            }
        }
        // quit is already mutexed
        quit = is_quit();
    } while (!quit);
    gui_cleanup();
    computation_cleanup();
    return NULL;
}

void process_pipe_message(event * const ev) {
    
    my_assert(ev != NULL && ev->type == EV_PIPE_IN_MESSAGE && ev->data.msg, __func__, __LINE__, __FILE__);
    ev->type = EV_TYPE_NUM;
    const message *msg = ev->data.msg;
    switch (msg->type) {
    case MSG_STARTUP:
        info_module_message("OK");
        fprintf(stderr, "%s", msg->data.startup.message);
        break;
    case MSG_OK:
        info_module_message("OK");
        break;
    case MSG_VERSION:
        fprintf(stderr, "INFO: Module version %d.%d-p%d\n", msg->data.version.major, msg->data.version.minor, msg->data.version.patch);
        break;
    case MSG_COMPUTE_DATA:
    if (!is_abort()) {
        update_data(&(msg->data.compute_data));
        // fprintf(stderr, "COMPUTE_DATA INFO: i_re: %d, i_im: %d, iter: %d\n", msg->data.compute_data.i_re, msg->data.compute_data.i_im, msg->data.compute_data.iter);
        // gui_refresh();
    }
        break;
    case MSG_DONE:
        gui_refresh();
        if (is_done()) {
            info_module_message("COMPUTATION_DONE");
            gui_save_image();
            fprintf(stderr, "Fractal image saved\n");
        } else {
            if (is_abort()) {
                info_module_message("MSG_ABORT");
                break;
            }
            event ev = { .type = EV_COMPUTE };
            queue_push(ev);
        }
        break;
    case MSG_ABORT:
        info_module_message("MSG_ABORT");
        abort_comp();
        break;
    case MSG_ERROR:
        warn("Received ERROR from Module");
        break;
    case MSG_RESET:
        if (!is_abort() && is_computing()) { 
            // todo: fix problem: when fractal is fully computed it calls this condition
            warn("Reset was requested but program is computing value");
            info("Press 'a' to abort computation and then 'r' to reset window");
            break;
        }
        info("RESET confirmed");
        // Clear the window with black color
        reset_fractal();
        gui_refresh();
        break;
    default:
        fprintf(stderr, "Unhandled pipe message type %d\n", msg->type);
        break;
    } // end switch
    free(ev->data.msg);
    ev->data.msg = NULL;
}

/* end of main.c */