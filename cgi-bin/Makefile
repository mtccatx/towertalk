# Project settings

## Compiler
CC :=				gcc
ifdef RELEASE
CFLAGS :=			-O2 \
					-Wall -Werror \
					-MMD -MP \
					-Iinclude
else
CFLAGS :=			-g \
					-Wall -Werror \
					-MMD -MP \
					-Iinclude
endif
LD :=				gcc
ifdef RELEASE
LDFLAGS :=			-lcrypt
else
LDFLAGS :=			-lcrypt
endif

## Project

OBJNAME :=			towertalk.bin
SRC :=				$(wildcard src/*.c)
OBJS =				$(SRC:src/%.c=obj/%.o)

# Targets

.PHONY: all
all: $(OBJNAME)

## Helpers

.PHONY: clean
clean:
	rm -rf obj/
	rm -rf *.d
	rm -rf $(OBJNAME)

obj/%.o: src/%.c
	mkdir -p obj/
	$(CC) -o $@ $(CFLAGS) -c $<

-include $(OBJS:.o=.d)

## Main

$(foreach file,$(SRC),$(eval $(call compile_obj,$(basename $(notdir $(file))))))

$(OBJNAME): $(OBJS)
	$(LD) -o $@ $(OBJS) $(LDFLAGS)
