MRBC = ./mruby/bin/mrbc
CC = gcc
CFLAGS =
LDFLAGS =
INCLUDES = -I ./mruby/include
LIBS = ./mruby/build/host/lib/libmruby.a -lm
OBJS = main.o

TARGET = setting

all: $(TARGET)

$(TARGET): $(OBJS)
	@$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

clean:
	@rm -f $(TARGET) $(OBJS) *.mrb

.c.o:
	@$(CC) $(CFLAGS) $(INCLUDES) -c $<

mrb:
	@$(MRBC) app.rb

.PHONY: mruby
mruby :
	@if [ ! -d "./mruby" ]; then git clone git://github.com/mruby/mruby.git && cd mruby && /usr/bin/env ruby ./minirake && cd ..; fi
