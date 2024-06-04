/*
 * Filename: gui.h
 * Date:     2017/04/15 12:41
 * Author:   Jan Faigl
 */

#ifndef __GUI_H__
#define __GUI_H__

void gui_init(void);
void gui_cleanup(void);

void gui_refresh(void);

void* gui_win_thread(void* d);

void reset_gui(void);
void gui_save_image(void);
// void clearWindow(SDL_Surface* screen, Uint8 r, Uint8 g, Uint8 b);

// set_width_gui(int width);
#endif

/* end of gui.h */