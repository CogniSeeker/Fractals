/*
 * Filename: prgsem_comp_module.c
 * Date:     2017/04/15 12:41
 * Author:   Oleh Borys
 */

#ifndef __PRGSEM_COMP_MODULE_H__
#define __PRGSEM_COMP_MODULE_H__

void* read_input_thread(void* d);
void* compute_thread(void *d);

void send_message(int pipe_out, const message *msg);

#endif

/* end of prgsem_comp_module.c */