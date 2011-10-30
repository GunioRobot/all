EXECUTABLE	=	main

CFLAGS		=	-O3
LDFLAGS		=
WARNINGS	=	-Wall -Wextra -Wfatal-errors -Werror -pedantic
DEBUGGING	=
#DEBUGGING	=	-g -pg

SRCDIRS		=	. ./src/ ./src/io/
HDRDIRS		=	./include/
LIBDIRS		=	./lib/

#do not edit below this

SRCEXTS = .c .cpp

ifeq ($(SRCDIRS),)
  SRCDIRS = .
endif

INCDIRS = $(foreach d,$(HDRDIRS),$(addprefix -I, $(d)))
LNKDIRS = $(foreach d,$(LIBDIRS),$(addprefix -L, $(d)))
SOURCES = $(foreach d,$(SRCDIRS),$(wildcard $(addprefix $(d)/*,$(SRCEXTS))))
OBJECTS = $(addsuffix .o, $(basename $(SOURCES)))

CC = g++
COMPILE.c = $(CC) $(WARNINGS) $(CFLAGS) $(INCDIRS) $(LNKDIRS) -c
LINK.c = $(CC) $(CFLAGS) $(LDFLAGS)

.PHONY: all objects clean

# Delete the default suffixes
.SUFFIXES:

all: $(EXECUTABLE)

# Rules for generating object files (.o).
#----------------------------------------
objs:$(OBJECTS)

%.o:%.c
	$(COMPILE.c) $< -o $@

%.o:%.cpp
	$(COMPILE.c) $< -o $@

# Rules for generating the executable.
#-------------------------------------
$(EXECUTABLE):$(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) $(DEBUGGING) -o $@
run:
	./$(EXECUTABLE)

clean:
	$(RM) $(OBJECTS) $(EXECUTABLE)

