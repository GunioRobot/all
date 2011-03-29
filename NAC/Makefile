EXECUTABLE	=	main

CFLAGS		=
LDFLAGS		=
WARNINGS	=	-Wall -Wextra -Wfatal-errors -Werror -pedantic
DEBUGGING	=
#DEBUGGING	=	-g -pg

SRCDIRS		=	. ./src/
HDRDIRS		=	. ./includes/ ./src/ ../includes/

#do not edit below this

SRCEXTS = .c .cpp
HDREXTS = .h .hpp

ifeq ($(SRCDIRS),)
  SRCDIRS = .
endif

INCDIRS = $(foreach d,$(HDRDIRS),$(addprefix -I, $(d)))
SOURCES = $(foreach d,$(SRCDIRS),$(wildcard $(addprefix $(d)/*,$(SRCEXTS))))
HEADERS = $(foreach d,$(SRCDIRS),$(wildcard $(addprefix $(d)/*,$(HDREXTS))))
OBJECTS = $(addsuffix .o, $(basename $(SOURCES)))

CC = g++
COMPILE.c = $(CC) $(WARNINGS) $(CFLAGS) $(INCDIRS) -c
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

