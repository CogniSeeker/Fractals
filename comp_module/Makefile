CFLAGS+= -Wall -Werror -std=gnu99 -g
LDFLAGS=-pthread

CFLAGS+=$(shell sdl2-config --cflags)
LDFLAGS+=$(shell sdl2-config --libs) -lSDL2_image

BINARIES=prgsem_comp_module

all: $(BINARIES)

PRGSEM_COMP_MODULE_SRCS=prgsem_comp_module.c ../main_module/prg_io_nonblock.c ../main_module/messages.c ../main_module/utils.c ../main_module/computation.c ../main_module/event_queue.c
PRGSEM_COMP_MODULE_OBJS=$(patsubst %.c,%.o,$(PRGSEM_COMP_MODULE_SRCS))

prgsem_comp_module: $(PRGSEM_COMP_MODULE_OBJS)
	$(CC) $(PRGSEM_COMP_MODULE_OBJS) $(LDFLAGS) -o $@

$(PRGSEM_COMP_MODULE_OBJS): %.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(BINARIES) $(PRGSEM_COMP_MODULE_OBJS)
