/*
 * Filename: xwin_sdl.c
 * Date:     2015/06/18 14:37
 * Author:   Jan Faigl
 */

#include <SDL.h>

#include <SDL_image.h>

#include "utils.h"

#include "xwin_sdl.h"

static SDL_Window *win = NULL;

static unsigned char icon_32x32_bits[] = {
   0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x20, 0x00, 0x00, 0x23, 0x00, 0x01, 0x29, 0x00, 0x01, 0x2e, 0x00, 0x02, 0x31, 0x00, 0x02, 0x34, 0x00, 0x02, 0x35, 0x00, 0x02, 0x33, 0x00, 0x02, 0x31, 0x00, 0x01, 0x2d, 0x00, 0x01, 0x29, 0x00, 0x00, 0x23, 0x00, 0x00, 0x20, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21,
   0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x23, 0x00, 0x01, 0x2b, 0x00, 0x02, 0x3b, 0x00, 0x03, 0x41, 0x00, 0x03, 0x43, 0x00, 0x04, 0x46, 0x00, 0x04, 0x49, 0x00, 0x05, 0x4d, 0x00, 0x04, 0x46, 0x00, 0x03, 0x43, 0x00, 0x03, 0x3f, 0x00, 0x03, 0x40, 0x00, 0x03, 0x41, 0x00, 0x03, 0x42, 0x00, 0x03, 0x3c, 0x00, 0x01, 0x2d, 0x00, 0x00, 0x24, 0x00, 0x00, 0x20, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21,
   0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x02, 0x2d, 0x00, 0x03, 0x42, 0x00, 0x03, 0x40, 0x00, 0x03, 0x41, 0x00, 0x07, 0x51, 0x00, 0x08, 0x5d, 0x03, 0x0e, 0x6a, 0x08, 0x20, 0x89, 0x34, 0x3d, 0x66, 0x00, 0x10, 0x7b, 0x00, 0x09, 0x5e, 0x00, 0x08, 0x58, 0x00, 0x04, 0x45, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x41, 0x00, 0x02, 0x38, 0x00, 0x00, 0x24, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21,
   0x00, 0x00, 0x24, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x41, 0x00, 0x08, 0x5a, 0x00, 0x09, 0x5d, 0x00, 0x0f, 0x77, 0x20, 0x44, 0x93, 0x53, 0x2a, 0x0b, 0x69, 0x52, 0x2c, 0x15, 0x26, 0x86, 0x00, 0x0e, 0x74, 0x00, 0x08, 0x5d, 0x00, 0x08, 0x5b, 0x00, 0x07, 0x54, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x37, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21,
   0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x08, 0x5a, 0x00, 0x0c, 0x69, 0x01, 0x0f, 0x77, 0x23, 0x2f, 0x77, 0x2a, 0x5f, 0xa4, 0x63, 0xa6, 0x7a, 0x33, 0x67, 0x8b, 0x09, 0x30, 0xae, 0x01, 0x16, 0x8b, 0x00, 0x0e, 0x75, 0x00, 0x08, 0x5c, 0x00, 0x08, 0x5b, 0x00, 0x08, 0x5a, 0x00, 0x03, 0x41, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x01, 0x29, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21,
   0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x06, 0x4d, 0x00, 0x0d, 0x6e, 0x13, 0x36, 0x98, 0x1b, 0x46, 0xa7, 0x66, 0x7e, 0x6d, 0x4b, 0x1c, 0x07, 0x2f, 0x05, 0x00, 0x74, 0x41, 0x16, 0x3b, 0x6e, 0x8e, 0x47, 0x44, 0x50, 0x24, 0x31, 0x82, 0x00, 0x0f, 0x77, 0x00, 0x09, 0x5f, 0x00, 0x08, 0x5b, 0x00, 0x08, 0x5b, 0x00, 0x04, 0x45, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x02, 0x34, 0x00, 0x00, 0x20, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21,
   0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x41, 0x00, 0x08, 0x5c, 0x00, 0x10, 0x79, 0x4c, 0x59, 0x5e, 0x60, 0x77, 0x62, 0x92, 0x7a, 0x32, 0x24, 0x00, 0x00, 0x1d, 0x00, 0x00, 0x56, 0x13, 0x02, 0x73, 0x7c, 0x4a, 0x46, 0x5d, 0x78, 0x18, 0x3d, 0x9f, 0x01, 0x17, 0x8e, 0x00, 0x0f, 0x76, 0x00, 0x0c, 0x6d, 0x00, 0x09, 0x5c, 0x00, 0x08, 0x5b, 0x00, 0x06, 0x4c, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x41, 0x00, 0x02, 0x38, 0x00, 0x00, 0x20, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21,
   0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x06, 0x4e, 0x00, 0x08, 0x5c, 0x00, 0x0e, 0x72, 0x00, 0x17, 0x95, 0x12, 0x3f, 0xaf, 0x65, 0x61, 0x3a, 0x81, 0x80, 0x3f, 0x9d, 0x79, 0x26, 0x84, 0x5e, 0x27, 0x6d, 0x79, 0x48, 0x1a, 0x58, 0xbf, 0x00, 0x2b, 0xba, 0x00, 0x1a, 0x97, 0x01, 0x14, 0x87, 0x00, 0x0f, 0x78, 0x00, 0x0e, 0x72, 0x00, 0x0c, 0x69, 0x00, 0x08, 0x5b, 0x00, 0x06, 0x52, 0x00, 0x03, 0x3f, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x02, 0x39, 0x00, 0x00, 0x20, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21,
   0x00, 0x03, 0x40, 0x00, 0x03, 0x42, 0x00, 0x08, 0x5d, 0x00, 0x08, 0x5c, 0x00, 0x0e, 0x72, 0x01, 0x14, 0x87, 0x03, 0x2b, 0xb1, 0x30, 0x5f, 0xa3, 0x25, 0x70, 0xb0, 0x6d, 0x84, 0x5b, 0x67, 0xdf, 0xa9, 0x44, 0x60, 0x6f, 0x1b, 0x7e, 0xdc, 0x51, 0x45, 0x49, 0x2d, 0x4c, 0x87, 0x00, 0x21, 0xa5, 0x00, 0x1b, 0x99, 0x0a, 0x20, 0x8c, 0x00, 0x12, 0x83, 0x00, 0x0f, 0x76, 0x00, 0x09, 0x61, 0x00, 0x08, 0x59, 0x00, 0x04, 0x46, 0x00, 0x03, 0x3f, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x02, 0x37, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21,
   0x00, 0x03, 0x3f, 0x00, 0x06, 0x4f, 0x00, 0x08, 0x5b, 0x00, 0x08, 0x5c, 0x00, 0x0f, 0x78, 0x03, 0x22, 0xa0, 0x11, 0x41, 0xb2, 0x25, 0x58, 0xa5, 0x72, 0x6a, 0x2c, 0x5d, 0x6e, 0x50, 0x88, 0xa4, 0x53, 0xbf, 0x95, 0x2b, 0x7c, 0xc3, 0x79, 0x7a, 0x8a, 0x4a, 0x3f, 0x76, 0x99, 0x07, 0x44, 0xcc, 0x2a, 0x59, 0x9c, 0x57, 0x67, 0x61, 0x2a, 0x46, 0x85, 0x1d, 0x3f, 0x98, 0x09, 0x20, 0x88, 0x00, 0x0b, 0x65, 0x00, 0x08, 0x5c, 0x00, 0x06, 0x51, 0x00, 0x03, 0x42, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x02, 0x31, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21,
   0x00, 0x03, 0x42, 0x00, 0x08, 0x5d, 0x00, 0x08, 0x5b, 0x00, 0x0b, 0x62, 0x00, 0x17, 0x8d, 0x5a, 0x50, 0x38, 0x77, 0x3d, 0x0a, 0x5b, 0x85, 0x6b, 0x79, 0x6b, 0x2e, 0x98, 0x4f, 0x12, 0x48, 0x04, 0x00, 0x5e, 0x15, 0x02, 0x53, 0x19, 0x04, 0x76, 0x43, 0x13, 0x9b, 0x86, 0x2e, 0x4b, 0xa0, 0x9e, 0x7e, 0x6e, 0x35, 0x53, 0x13, 0x00, 0x36, 0x0d, 0x02, 0x48, 0x61, 0x70, 0x01, 0x21, 0xa1, 0x00, 0x0f, 0x78, 0x00, 0x0b, 0x64, 0x00, 0x08, 0x5b, 0x00, 0x08, 0x5c, 0x00, 0x05, 0x49, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x01, 0x2b, 0x00, 0x00, 0x21,
   0x00, 0x05, 0x4c, 0x00, 0x08, 0x5b, 0x00, 0x08, 0x5b, 0x00, 0x0e, 0x73, 0x01, 0x14, 0x88, 0x07, 0x39, 0xbc, 0x24, 0x74, 0xbf, 0x64, 0x78, 0x4c, 0x73, 0x1e, 0x02, 0x20, 0x00, 0x00, 0x23, 0x00, 0x00, 0x23, 0x00, 0x00, 0x23, 0x00, 0x00, 0x41, 0x07, 0x01, 0x95, 0x49, 0x0f, 0x84, 0xa9, 0x56, 0x70, 0xac, 0x73, 0x72, 0x8b, 0x5f, 0x42, 0x93, 0xaa, 0x4d, 0x3f, 0x47, 0x01, 0x1a, 0x92, 0x01, 0x13, 0x80, 0x00, 0x0e, 0x73, 0x00, 0x0b, 0x65, 0x00, 0x08, 0x5b, 0x00, 0x08, 0x5b, 0x00, 0x08, 0x5a, 0x00, 0x03, 0x41, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x42, 0x00, 0x00, 0x25,
   0x00, 0x07, 0x54, 0x00, 0x08, 0x5a, 0x00, 0x0c, 0x6b, 0x00, 0x0f, 0x76, 0x01, 0x1b, 0x95, 0x01, 0x31, 0xbd, 0x61, 0x3e, 0x22, 0x8e, 0x8e, 0x42, 0x64, 0x23, 0x05, 0x22, 0x00, 0x00, 0x23, 0x00, 0x00, 0x23, 0x00, 0x00, 0x23, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x82, 0x1d, 0x03, 0x71, 0x6a, 0x2d, 0x5b, 0xa2, 0x8f, 0x32, 0x6f, 0xa2, 0x09, 0x47, 0xcc, 0x04, 0x29, 0xaf, 0x01, 0x1e, 0x9c, 0x01, 0x15, 0x8a, 0x00, 0x0f, 0x74, 0x00, 0x0e, 0x73, 0x00, 0x0c, 0x6a, 0x00, 0x08, 0x5a, 0x00, 0x08, 0x5b, 0x00, 0x08, 0x5b, 0x00, 0x05, 0x4c, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x3b,
   0x00, 0x07, 0x57, 0x00, 0x0a, 0x64, 0x00, 0x0f, 0x75, 0x07, 0x23, 0x96, 0x48, 0x55, 0x63, 0x1f, 0x42, 0x8c, 0x32, 0x7c, 0xa7, 0x9b, 0x63, 0x17, 0x7f, 0x4c, 0x15, 0x2d, 0x00, 0x00, 0x20, 0x00, 0x00, 0x23, 0x00, 0x00, 0x21, 0x00, 0x00, 0x5e, 0x0d, 0x00, 0xcf, 0x74, 0x18, 0x84, 0x83, 0x35, 0x9f, 0xbf, 0x57, 0x66, 0x25, 0x01, 0x42, 0x5f, 0x7c, 0x04, 0x33, 0xbc, 0x04, 0x27, 0xac, 0x00, 0x1d, 0x9f, 0x00, 0x16, 0x8d, 0x01, 0x10, 0x7a, 0x00, 0x0e, 0x72, 0x00, 0x0d, 0x70, 0x00, 0x09, 0x5f, 0x00, 0x08, 0x5a, 0x00, 0x08, 0x5b, 0x00, 0x07, 0x53, 0x00, 0x03, 0x3f, 0x00, 0x03, 0x40,
   0x00, 0x07, 0x55, 0x05, 0x1c, 0x7f, 0x2c, 0x48, 0x84, 0x3b, 0x5a, 0x82, 0x5e, 0x42, 0x1d, 0x81, 0x44, 0x10, 0x6a, 0x97, 0x62, 0x6e, 0xc8, 0x8d, 0x6b, 0x5b, 0x2f, 0xa6, 0x87, 0x2c, 0x71, 0x24, 0x06, 0x34, 0x08, 0x00, 0x92, 0x31, 0x06, 0x6c, 0x47, 0x15, 0xa6, 0x94, 0x37, 0x53, 0x24, 0x0a, 0x4c, 0xbc, 0xb6, 0x34, 0x8e, 0xbc, 0x17, 0x68, 0xd6, 0x0c, 0x54, 0xd6, 0x30, 0x53, 0x86, 0x3d, 0x3a, 0x42, 0x44, 0x51, 0x66, 0x00, 0x21, 0xa7, 0x01, 0x18, 0x90, 0x01, 0x14, 0x86, 0x00, 0x12, 0x84, 0x00, 0x0e, 0x7d, 0x00, 0x0a, 0x62, 0x00, 0x08, 0x5a, 0x00, 0x06, 0x4e, 0x00, 0x03, 0x3f,
   0x00, 0x06, 0x4f, 0x03, 0x13, 0x79, 0x36, 0x37, 0x5d, 0x56, 0x76, 0x70, 0x40, 0x1c, 0x07, 0x5e, 0x2b, 0x09, 0x88, 0x7f, 0x3c, 0x1f, 0x76, 0xc7, 0x5c, 0x60, 0x4e, 0x51, 0x92, 0x90, 0x85, 0x61, 0x25, 0x9a, 0xdd, 0x77, 0x58, 0x68, 0x3c, 0xce, 0xb9, 0x3c, 0x5c, 0x53, 0x29, 0x66, 0xef, 0xbc, 0x59, 0xbf, 0x9f, 0x73, 0x72, 0x46, 0x2d, 0x22, 0x2b, 0x38, 0x8d, 0xad, 0x6a, 0x50, 0x24, 0x5f, 0x4c, 0x28, 0x54, 0x6e, 0x68, 0x07, 0x3d, 0xc6, 0x07, 0x2f, 0xb1, 0x15, 0x32, 0x8e, 0x26, 0x56, 0xa4, 0x4c, 0x5b, 0x64, 0x00, 0x15, 0x92, 0x00, 0x0c, 0x70, 0x00, 0x09, 0x5f, 0x00, 0x04, 0x45,
   0x00, 0x04, 0x44, 0x00, 0x09, 0x5f, 0x00, 0x0c, 0x72, 0x00, 0x15, 0x91, 0x4c, 0x62, 0x6d, 0x35, 0x62, 0x93, 0x1c, 0x32, 0x84, 0x09, 0x34, 0xb3, 0x05, 0x3d, 0xcc, 0x4b, 0x6b, 0x74, 0x61, 0x5f, 0x39, 0x74, 0x5c, 0x29, 0x3b, 0x93, 0xaf, 0x36, 0x22, 0x24, 0x79, 0x6d, 0x39, 0x61, 0xc0, 0x97, 0x63, 0xf0, 0xc0, 0x5f, 0x5a, 0x2e, 0xc7, 0xa2, 0x32, 0x62, 0x6c, 0x3a, 0x8d, 0xd0, 0x74, 0x84, 0x72, 0x33, 0x41, 0x8e, 0x9e, 0x57, 0x53, 0x4d, 0x1a, 0x76, 0xd1, 0x7d, 0x75, 0x38, 0x6c, 0x3b, 0x10, 0x4a, 0x23, 0x0b, 0x57, 0x72, 0x69, 0x2f, 0x34, 0x65, 0x02, 0x14, 0x7c, 0x00, 0x06, 0x50,
   0x00, 0x03, 0x3f, 0x00, 0x06, 0x4e, 0x00, 0x08, 0x5a, 0x00, 0x0a, 0x64, 0x00, 0x10, 0x82, 0x00, 0x13, 0x89, 0x01, 0x15, 0x88, 0x01, 0x19, 0x91, 0x00, 0x21, 0xa6, 0x3b, 0x53, 0x79, 0x3a, 0x2e, 0x35, 0x3b, 0x62, 0x82, 0x0e, 0x58, 0xd6, 0x12, 0x69, 0xde, 0x2a, 0x8e, 0xca, 0x44, 0xbf, 0xc4, 0x5c, 0x3f, 0x1d, 0x9e, 0x98, 0x3e, 0x74, 0x59, 0x1e, 0x86, 0x2f, 0x07, 0x4a, 0x1c, 0x06, 0x8c, 0x3a, 0x0b, 0xa1, 0x8e, 0x32, 0x76, 0x6c, 0x35, 0x6a, 0xc7, 0x93, 0x72, 0x96, 0x59, 0x7d, 0x3f, 0x0f, 0x50, 0x32, 0x16, 0x51, 0x65, 0x6d, 0x3c, 0x4d, 0x72, 0x08, 0x21, 0x84, 0x00, 0x07, 0x57,
   0x00, 0x03, 0x40, 0x00, 0x03, 0x3f, 0x00, 0x07, 0x53, 0x00, 0x08, 0x5b, 0x00, 0x08, 0x5a, 0x00, 0x0a, 0x60, 0x00, 0x0d, 0x70, 0x00, 0x0e, 0x72, 0x01, 0x10, 0x7b, 0x00, 0x16, 0x8f, 0x00, 0x1d, 0xa1, 0x03, 0x28, 0xae, 0x04, 0x33, 0xbc, 0x40, 0x69, 0x95, 0x71, 0x33, 0x05, 0x92, 0xae, 0x51, 0x82, 0x7a, 0x30, 0xd4, 0x73, 0x16, 0x60, 0x0f, 0x01, 0x1f, 0x00, 0x00, 0x23, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x37, 0x02, 0x00, 0x70, 0x45, 0x15, 0x97, 0x60, 0x16, 0x3d, 0x78, 0x94, 0x2e, 0x42, 0x75, 0x4e, 0x5a, 0x60, 0x0b, 0x2b, 0x9b, 0x00, 0x10, 0x78, 0x00, 0x0b, 0x66, 0x00, 0x08, 0x59,
   0x00, 0x03, 0x3b, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x06, 0x4d, 0x00, 0x08, 0x5b, 0x00, 0x08, 0x5b, 0x00, 0x08, 0x5a, 0x00, 0x0c, 0x6b, 0x00, 0x0e, 0x73, 0x00, 0x0f, 0x74, 0x01, 0x15, 0x8a, 0x01, 0x1e, 0x9c, 0x03, 0x28, 0xad, 0x09, 0x46, 0xca, 0x3f, 0x6e, 0x8e, 0x52, 0xab, 0x9c, 0x6d, 0x6f, 0x35, 0x74, 0x16, 0x01, 0x20, 0x00, 0x00, 0x23, 0x00, 0x00, 0x23, 0x00, 0x00, 0x23, 0x00, 0x00, 0x23, 0x00, 0x00, 0x53, 0x16, 0x03, 0x9e, 0x9c, 0x42, 0x69, 0x41, 0x1a, 0x09, 0x35, 0xb3, 0x01, 0x1c, 0x98, 0x01, 0x10, 0x7a, 0x00, 0x0d, 0x6f, 0x00, 0x08, 0x5b, 0x00, 0x07, 0x56,
   0x00, 0x00, 0x25, 0x00, 0x03, 0x42, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x41, 0x00, 0x08, 0x5a, 0x00, 0x08, 0x5b, 0x00, 0x08, 0x5b, 0x00, 0x0b, 0x65, 0x00, 0x0e, 0x73, 0x01, 0x12, 0x7e, 0x01, 0x19, 0x90, 0x40, 0x47, 0x69, 0x30, 0x8b, 0xc3, 0x72, 0x8d, 0x5f, 0x72, 0xa3, 0x72, 0x7b, 0x96, 0x4c, 0x92, 0x54, 0x14, 0x4a, 0x0a, 0x01, 0x23, 0x00, 0x00, 0x23, 0x00, 0x00, 0x23, 0x00, 0x00, 0x21, 0x00, 0x00, 0x7b, 0x24, 0x04, 0x68, 0x6b, 0x39, 0x1f, 0x6d, 0xbf, 0x02, 0x34, 0xc0, 0x01, 0x14, 0x88, 0x00, 0x0e, 0x74, 0x00, 0x08, 0x5c, 0x00, 0x08, 0x5b, 0x00, 0x06, 0x4e,
   0x00, 0x00, 0x21, 0x00, 0x01, 0x2b, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x05, 0x4a, 0x00, 0x08, 0x5c, 0x00, 0x08, 0x5b, 0x00, 0x0b, 0x64, 0x00, 0x0f, 0x77, 0x00, 0x1d, 0x9b, 0x39, 0x5c, 0x8b, 0x42, 0x17, 0x06, 0x50, 0x10, 0x00, 0x7c, 0x59, 0x1d, 0x4c, 0xab, 0xa4, 0x9d, 0x89, 0x2f, 0x70, 0x48, 0x18, 0x5e, 0x19, 0x03, 0x50, 0x0e, 0x01, 0x3e, 0x02, 0x00, 0x86, 0x3b, 0x0c, 0x7b, 0x5a, 0x1f, 0x60, 0x82, 0x5e, 0x72, 0x39, 0x07, 0x53, 0x44, 0x2e, 0x00, 0x19, 0x91, 0x00, 0x0b, 0x67, 0x00, 0x08, 0x5b, 0x00, 0x08, 0x5c, 0x00, 0x04, 0x44,
   0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x02, 0x31, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x42, 0x00, 0x06, 0x52, 0x00, 0x08, 0x5c, 0x00, 0x0b, 0x65, 0x08, 0x1f, 0x89, 0x25, 0x43, 0x90, 0x32, 0x45, 0x78, 0x62, 0x64, 0x4b, 0x35, 0x63, 0x91, 0x0a, 0x49, 0xce, 0x31, 0x71, 0xa8, 0x6d, 0x86, 0x4e, 0x88, 0xc4, 0x6e, 0xc1, 0x97, 0x2d, 0x8e, 0xa5, 0x4d, 0x61, 0x52, 0x31, 0x81, 0x7b, 0x33, 0x2d, 0x5a, 0x93, 0x14, 0x44, 0xb1, 0x07, 0x29, 0xa3, 0x01, 0x11, 0x7d, 0x00, 0x08, 0x5c, 0x00, 0x08, 0x5b, 0x00, 0x07, 0x53, 0x00, 0x03, 0x3f,
   0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x02, 0x37, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x3f, 0x00, 0x04, 0x47, 0x00, 0x08, 0x59, 0x00, 0x09, 0x61, 0x00, 0x0f, 0x77, 0x00, 0x13, 0x88, 0x0c, 0x24, 0x8c, 0x00, 0x1c, 0x9d, 0x00, 0x22, 0xa6, 0x30, 0x4a, 0x83, 0x5d, 0x45, 0x32, 0x23, 0x82, 0xd2, 0x44, 0x57, 0x63, 0x6d, 0xd3, 0x9a, 0x70, 0x97, 0x6b, 0x28, 0x6d, 0xa5, 0x24, 0x60, 0xb9, 0x02, 0x2a, 0xb0, 0x01, 0x15, 0x88, 0x00, 0x0e, 0x73, 0x00, 0x08, 0x5c, 0x00, 0x08, 0x5c, 0x00, 0x04, 0x44, 0x00, 0x03, 0x40,
   0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x20, 0x00, 0x02, 0x39, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x3f, 0x00, 0x06, 0x52, 0x00, 0x08, 0x5b, 0x00, 0x0c, 0x6a, 0x00, 0x0e, 0x72, 0x00, 0x10, 0x79, 0x01, 0x14, 0x87, 0x00, 0x1a, 0x98, 0x00, 0x2a, 0xbb, 0x12, 0x51, 0xc6, 0x68, 0x6b, 0x47, 0x76, 0x64, 0x35, 0x94, 0x75, 0x28, 0x83, 0x86, 0x47, 0x6e, 0x65, 0x35, 0x1a, 0x40, 0xa0, 0x00, 0x16, 0x94, 0x00, 0x0e, 0x73, 0x00, 0x08, 0x5c, 0x00, 0x07, 0x53, 0x00, 0x03, 0x3f, 0x00, 0x03, 0x40,
   0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x20, 0x00, 0x02, 0x38, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x06, 0x4c, 0x00, 0x08, 0x5b, 0x00, 0x09, 0x5d, 0x00, 0x0d, 0x6d, 0x00, 0x0f, 0x76, 0x01, 0x16, 0x8c, 0x0e, 0x35, 0xa8, 0x37, 0x5c, 0x8e, 0x66, 0x80, 0x59, 0x64, 0x20, 0x07, 0x1f, 0x00, 0x00, 0x27, 0x00, 0x00, 0x8c, 0x6f, 0x29, 0x51, 0x7c, 0x7b, 0x46, 0x55, 0x65, 0x00, 0x10, 0x7b, 0x00, 0x08, 0x5c, 0x00, 0x03, 0x42, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40,
   0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x20, 0x00, 0x02, 0x35, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x04, 0x45, 0x00, 0x08, 0x5b, 0x00, 0x08, 0x5b, 0x00, 0x0a, 0x60, 0x00, 0x0f, 0x78, 0x32, 0x37, 0x76, 0x4d, 0x3d, 0x3c, 0x3c, 0x7a, 0x99, 0x77, 0x41, 0x13, 0x2d, 0x04, 0x00, 0x3e, 0x11, 0x02, 0x70, 0x7d, 0x5a, 0x25, 0x4b, 0x9f, 0x1f, 0x40, 0x93, 0x00, 0x0f, 0x77, 0x00, 0x07, 0x53, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40,
   0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x01, 0x2a, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x41, 0x00, 0x08, 0x5a, 0x00, 0x08, 0x5b, 0x00, 0x08, 0x5c, 0x00, 0x0f, 0x75, 0x00, 0x16, 0x8c, 0x0c, 0x31, 0xab, 0x2f, 0x72, 0xa1, 0x62, 0x9d, 0x6f, 0x36, 0x72, 0xa1, 0x2e, 0x32, 0x66, 0x00, 0x10, 0x7c, 0x00, 0x0d, 0x6d, 0x00, 0x08, 0x5a, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40,
   0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x22, 0x00, 0x03, 0x39, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x07, 0x55, 0x00, 0x08, 0x5b, 0x00, 0x08, 0x5d, 0x00, 0x0e, 0x74, 0x18, 0x2a, 0x8a, 0x70, 0x5b, 0x33, 0x4d, 0x15, 0x00, 0x25, 0x4c, 0x93, 0x00, 0x10, 0x7c, 0x00, 0x09, 0x5f, 0x00, 0x08, 0x5b, 0x00, 0x04, 0x42, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x42, 0x00, 0x01, 0x26,
   0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x25, 0x00, 0x02, 0x3a, 0x00, 0x03, 0x41, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x03, 0x40, 0x00, 0x04, 0x45, 0x00, 0x08, 0x59, 0x00, 0x09, 0x5e, 0x00, 0x0f, 0x7a, 0x3d, 0x41, 0x58, 0x0d, 0x26, 0x89, 0x03, 0x11, 0x6f, 0x00, 0x08, 0x5d, 0x00, 0x08, 0x55, 0x00, 0x03, 0x42, 0x00, 0x03, 0x40, 0x00, 0x03, 0x41, 0x00, 0x03, 0x33, 0x00, 0x00, 0x22, 0x00, 0x00, 0x21,
   0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x20, 0x00, 0x00, 0x25, 0x00, 0x01, 0x2f, 0x00, 0x03, 0x3e, 0x00, 0x03, 0x42, 0x00, 0x03, 0x41, 0x00, 0x03, 0x40, 0x00, 0x03, 0x3f, 0x00, 0x04, 0x44, 0x00, 0x04, 0x47, 0x00, 0x06, 0x52, 0x00, 0x05, 0x4d, 0x00, 0x05, 0x47, 0x00, 0x04, 0x44, 0x00, 0x03, 0x41, 0x00, 0x03, 0x40, 0x00, 0x02, 0x2f, 0x00, 0x00, 0x24, 0x00, 0x00, 0x20, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21,
   0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x1f, 0x00, 0x01, 0x24, 0x00, 0x01, 0x2a, 0x00, 0x01, 0x2f, 0x00, 0x02, 0x33, 0x00, 0x02, 0x35, 0x00, 0x02, 0x37, 0x00, 0x02, 0x36, 0x00, 0x02, 0x34, 0x00, 0x01, 0x30, 0x00, 0x01, 0x2b, 0x00, 0x01, 0x25, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21, 0x00, 0x00, 0x21
};

int xwin_init(int w, int h)
{
   int r = SDL_Init(SDL_INIT_VIDEO);
   // my_assert(win == NULL, __func__, __LINE__, __FILE__); // todo: delete it
   win = NULL;
   win = SDL_CreateWindow("PRG Semester Project", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
   my_assert(win != NULL, __func__, __LINE__, __FILE__);
   SDL_SetWindowTitle(win, "PRG SEM");
   SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(icon_32x32_bits, 32, 32, 24, 32*3, 0xff, 0xff00, 0xff0000, 0x0000);
   SDL_SetWindowIcon(win, surface);
   SDL_FreeSurface(surface);
   return r;
}

void xwin_close()
{
   my_assert(win != NULL, __func__, __LINE__, __FILE__);
   SDL_DestroyWindow(win);
   SDL_Quit();
}

void xwin_redraw(int w, int h, unsigned char *img)
{
   my_assert(img && win, __func__, __LINE__, __FILE__);
   SDL_Surface *scr = SDL_GetWindowSurface(win);
   for(int y = 0; y < scr->h; ++y) {
      for(int x = 0; x < scr->w; ++x) {
         const int idx = (y * scr->w + x) * scr->format->BytesPerPixel;
         Uint8 *px = (Uint8*)scr->pixels + idx;
         *(px + scr->format->Rshift / 8) = *(img++);
         *(px + scr->format->Gshift / 8) = *(img++);
         *(px + scr->format->Bshift / 8) = *(img++);
      }
   }
   SDL_UpdateWindowSurface(win);
}

void xwin_poll_events(void) 
{
   SDL_Event event;
   while (SDL_PollEvent(&event));
}

/* end of xwin_sdl.c */
