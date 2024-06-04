/*
 * Filename: computation.c
 * Author:   Jan Faigl, Oleh Borys
 */

#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "gui.h"

#include "computation.h"

struct  {
    double c_re;
    double c_im;
    int n;

    double range_re_min;
    double range_re_max;
    double range_im_min;
    double range_im_max;

    int grid_w;
    int grid_h;
    
    int cur_x;
    int cur_y;

    double d_re;
    double d_im;

    int nbr_chunks;
    int cid;
    double chunk_re;
    double chunk_im;

    uint8_t chunk_n_re;
    uint8_t chunk_n_im;

    uint8_t *grid;
    bool computing;
    bool abort;
    bool done;

} comp = {
    .c_re = -0.4,
    .c_im = 0.6,
    // .c_re = -0.744,
    // .c_im = 0.148,

    .n = 60,
    // .n = 70,
    
    // .range_re_min = -1.4,
    // .range_re_max = 1.4,
    // .range_im_min = -1.4,
    // .range_im_max = 1.4,
    .range_re_min = -1.6,
    .range_re_max = 1.6,
    .range_im_min = -1.1,
    .range_im_max = 1.1,

    .grid = NULL,
    .grid_w = 640,
    .grid_h = 480,
    // .grid_w = 500,
    // .grid_h = 500,

    .chunk_n_re = 64,
    .chunk_n_im = 48,
    // .chunk_n_re = 50,
    // .chunk_n_im = 50,

    .computing = false,
    .abort = false,
    .done = false

};

bool to_compute = true;

void computation_init(void) {
    comp.grid = my_alloc(comp.grid_w * comp.grid_h);
    comp.d_re = (comp.range_re_max - comp.range_re_min) / (1. * comp.grid_w);
    comp.d_im = -(comp.range_im_max - comp.range_im_min) / (1. * comp.grid_h);
    comp.nbr_chunks = (comp.grid_w * comp.grid_h) / (comp.chunk_n_re * comp.chunk_n_im);
}
void computation_cleanup(void) {
    if (comp.grid) {
        free(comp.grid);
    }
    comp.grid = NULL;
}

bool is_computing(void) { return comp.computing; }
bool is_done(void) { return comp.done; }
bool is_abort(void) { return comp.abort; }

void get_grid_size(int *w, int *h) {
    *w = comp.grid_w;
    *h = comp.grid_h;
}

void set_compute_state_false(void) { to_compute = false; }
void set_compute_state_true(void) { to_compute = true; }
bool get_compute_state(void) { return to_compute; }
void set_not_done(void) {comp.done = false; }
void abort_comp(void) { comp.abort = true; }
void enable_comp(void) { comp.abort = false; }
void set_isnot_computing(void) { comp.computing = false; }
bool set_compute(message *msg) {
    my_assert(msg != NULL, __func__, __LINE__, __FILE__);
    bool ret = !is_computing();
    if (ret) {
        msg->type = MSG_SET_COMPUTE;
        msg->data.set_compute.c_re = comp.c_re;
        msg->data.set_compute.c_im = comp.c_im;
        msg->data.set_compute.d_re = comp.d_re;
        msg->data.set_compute.d_im = comp.d_im;
        msg->data.set_compute.n = comp.n;
        msg->data.set_compute.grid_w = comp.grid_w;
        msg->data.set_compute.grid_h = comp.grid_h;
        msg->data.set_compute.nbr_chunks = comp.nbr_chunks;
        comp.done = false;
    }
    return ret;
}
bool compute(message *msg) {
    
    my_assert(msg != NULL, __func__, __LINE__, __FILE__);
    if (!is_computing()) { // first chunk
        comp.cid = 0;
        comp.computing = true;
        comp.cur_x = comp.cur_y = 0; // start computation of the whole image
        comp.chunk_re = comp.range_re_min; // upper-"left" corner
        comp.chunk_im = comp.range_im_max; // "upper"-left corner
        fprintf(stderr, "comp.chunk_re: %f\n", comp.chunk_re);
        fprintf(stderr, "comp.chunk_im: %f\n", comp.chunk_im);
        fprintf(stderr, "comp.chunk_n_re: %d\n", comp.chunk_n_re);
        fprintf(stderr, "comp.chunk_n_im: %d\n\n", comp.chunk_n_im);
        msg->type = MSG_COMPUTE;
    } else { // next chunk
        comp.cid += 1;
        if (comp.cid < comp.nbr_chunks) {
            comp.cur_x += comp.chunk_n_re;
            comp.chunk_re += comp.chunk_n_re * comp.d_re;
            if (comp.cur_x >= comp.grid_w) {
                comp.chunk_re = comp.range_re_min;
                comp.chunk_im += comp.chunk_n_im * comp.d_im;
                comp.cur_x = 0;
                comp.cur_y += comp.chunk_n_im;
            }
            msg->type = MSG_COMPUTE;
        } else { // all has been computed
        }
    }
    if (comp.computing && msg->type == MSG_COMPUTE) {
        msg->data.compute.cid = comp.cid; 
        msg->data.compute.re = comp.chunk_re; 
        msg->data.compute.im = comp.chunk_im; 
        msg->data.compute.n_re = comp.chunk_n_re; 
        msg->data.compute.n_im = comp.chunk_n_im;
    }
    fprintf(stderr, "INFO: compute cid: %d\n", comp.cid);
    return is_computing();
}

void update_image(int w, int h, unsigned char *img) {
    my_assert(img && comp.grid && w == comp.grid_w && h == comp.grid_h, __func__, __LINE__, __FILE__);
    for (int i = 0; i < w * h; ++i) {
        // add one to comp.n to guarantee non-divisibility by zero
        const double t = 1. * comp.grid[i] / (comp.n + 1.0); 
        *(img++) = 9 * (1-t)*t*t*t * 255;
        *(img++) = 15 * (1-t)*(1-t)*t * t * 255;
        *(img++) = 8.5 * (1-t)*(1-t)*(1-t) * t * 255;
    }
}

void update_data(const msg_compute_data *compute_data) {
    my_assert(compute_data != NULL, __func__, __LINE__, __FILE__);
    if (compute_data->cid == comp.cid) {
       
        const int idx = comp.cur_x + compute_data->i_re + (comp.cur_y + compute_data->i_im) * comp.grid_w;
        if (idx >= 0 && idx < (comp.grid_w * comp.grid_h)) {
            comp.grid[idx] = compute_data->iter;
        }
        if ((comp.cid + 1) >= comp.nbr_chunks && (compute_data->i_re + 1) == comp.chunk_n_re && (compute_data->i_im + 1) == comp.chunk_n_im) {
            comp.done = true;
            comp.computing = false;
        }
    } else {
        warn("Received chunk with unexpected chunk id (cid)");
        reset_fractal();
        set_isnot_computing();
        set_not_done();
    }
}

// Function to compute the magnitude of a complex number
double complex_magnitude(double re, double im) {
    double com_mag = re * re + im * im;
    return com_mag;
}

// Function to compute the Julia set
int compute_julia_set(int x, int y, double start_re, double start_im) {
    double real = start_re + ((comp.range_re_max - comp.range_re_min) / (1. * comp.grid_w)) * x;
    double imag = start_im + (-(comp.range_im_max - comp.range_im_min) / (1. * comp.grid_h)) * y;
    int i = 0;
    int n = comp.n;
    while (i < n && complex_magnitude(real, imag) < 4.0) {
        double new_real = real * real - imag * imag;
        double new_imag = 2.0 * real * imag;

        real = new_real + comp.c_re;
        imag = new_imag + comp.c_im;
        i++;
    }
    return i;
}
// get info about current config
void get_info_config(void) {
    fprintf(stderr, "//------------------//\n");
    fprintf(stderr, "* Grid size:\n");
    fprintf(stderr, "       |- width: %d\n", comp.grid_w);
    fprintf(stderr, "       |- height: %d\n", comp.grid_h);
    fprintf(stderr, "* Z number:\n");
    fprintf(stderr, "       |- range_re_min: %f\n", comp.range_re_min);
    fprintf(stderr, "       |- range_re_max: %f\n", comp.range_re_max);
    fprintf(stderr, "       |- range_im_min: %f\n", comp.range_im_min);
    fprintf(stderr, "       |- range_im_max: %f\n", comp.range_im_max);
    fprintf(stderr, "* C constant:\n");
    fprintf(stderr, "       |- c_re: %f\n", comp.c_re);
    fprintf(stderr, "       |- c_im: %f\n", comp.c_im);
    fprintf(stderr, "* Depth n: %d\n", comp.n);
    fprintf(stderr, "//------------------//\n");
}
// set parameter by name
void setParameterValue(const char *memberName, double value) {
    if (strcmp(memberName, "c_re") == 0) {
        comp.c_re = value;
    } else if (strcmp(memberName, "c_im") == 0) {
        comp.c_im = value;
    } else if (strcmp(memberName, "n") == 0) {
        comp.n = (int)value;
    } else if (strcmp(memberName, "range_re_min") == 0) {
        comp.range_re_min = value;
    } else if (strcmp(memberName, "range_re_max") == 0) {
        comp.range_re_max = value;
    } else if (strcmp(memberName, "range_im_min") == 0) {
        comp.range_im_min = value;
    } else if (strcmp(memberName, "range_im_max") == 0) {
        comp.range_im_max = value;
    } else if (strcmp(memberName, "grid_w") == 0) {
        comp.grid_w = (int)value;
        comp.chunk_n_re = (int)value / 10;
    } else if (strcmp(memberName, "grid_h") == 0) {
        comp.grid_h = (int)value;
        comp.chunk_n_im = (int)value / 10;
    }
}
// get value of variable by parameter
int getParameterValue(const char *memberName) {
    if (strcmp(memberName, "n") == 0) {
        return (int)comp.n;
    } else if (strcmp(memberName, "grid_w") == 0) {
        return (int)comp.grid_w;
    } else if (strcmp(memberName, "grid_h") == 0) {
        return (int)comp.grid_h;
    } else if (strcmp(memberName, "cid") == 0) {
        return (int)comp.cid;
    } else if (strcmp(memberName, "chunk_n_re") == 0) {
        return (int)comp.chunk_n_re;
    } else if (strcmp(memberName, "chunk_n_im") == 0) {
        return (int)comp.chunk_n_im;
    }

    // Default return value if memberName is not recognized
    return 0;
}
double getParameterValueDouble(const char *memberName) {
    if (strcmp(memberName, "c_re") == 0) {
        return comp.c_re;
    } else if (strcmp(memberName, "c_im") == 0) {
        return comp.c_im;
    } else if (strcmp(memberName, "range_re_min") == 0) {
        return comp.range_re_min;
    } else if (strcmp(memberName, "range_re_max") == 0) {
        return comp.range_re_max;
    } else if (strcmp(memberName, "range_im_min") == 0) {
        return comp.range_im_min;
    } else if (strcmp(memberName, "range_im_max") == 0) {
        return comp.range_im_max;
    }
    // Default return value if memberName is not recognized
    return 0.0;
}

void reset_fractal(void) {
    comp.cid = 0;
    comp.cur_x = comp.cur_y = 0; // start computation of the whole image
    comp.chunk_re = comp.range_re_min; // upper-"left" corner
    comp.chunk_im = comp.range_im_max; // "upper"-left corner
    for (int i = 0; i < comp.grid_h*comp.grid_w; ++i){
        comp.grid[i] = 0;
    }        
    set_isnot_computing();
    set_not_done();
}

/* end of computation.c */