/*
 * Filename: computation.h
 * Author:   Jan Faigl, Oleh Borys
 */

#ifndef COMPUTATION_H
#define COMPUTATION_H

#include <stdio.h>
#include <stdlib.h>

#include "messages.h"

void computation_init(void);
void computation_cleanup(void);

bool is_computing(void);
bool is_done(void);
bool is_abort(void);

void get_grid_size(int *w, int *h);

void abort_comp(void);
void enable_comp(void);
void set_compute_state_false(void);
void set_compute_state_true(void);
bool get_compute_state(void);
void set_not_done(void);
bool set_compute(message *msg);
void set_isnot_computing(void);
bool compute(message *msg);

void update_image(int w, int h, unsigned char *img);
void update_data(const msg_compute_data *compute_data);

double complex_magnitude(double re, double im);
int compute_julia_set(int x, int y, double start_re, double start_im);

void setParameterValue(const char *memberName, double value);

int getParameterValue(const char *memberName);
double getParameterValueDouble(const char *memberName);

void get_info_config(void);

void reset_fractal(void);

#endif

/* end of computation.h */