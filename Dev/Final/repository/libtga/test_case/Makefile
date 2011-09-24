EXECUTABLE	=	main

CFLAGS		=
LFLAGS		=

WARNINGS	=	-W -Wall -Wextra -Wfatal-errors -Werror -Wwrite-strings -Wcast-qual -Wconversion -pedantic
DEBUGGING	=

SRCDIRS		=	.
HDRDIRS		=	./include/ ../include/
LNKDIRS		=

SRCEXTS = .cpp .c

#do not edit below this
ifeq ($(SRCDIRS),)
  SRCDIRS = .
endif

SOURCES = $(foreach d,$(SRCDIRS),$(wildcard $(addprefix $(d)/*,$(SRCEXTS))))
INCDIRS = $(foreach d,$(HDRDIRS),$(addprefix -I, $(d)))
LIBDIRS = $(foreach d,$(LNKDIRS),$(addprefix -L, $(d)))

OBJECTS = $(addsuffix .o, $(basename $(SOURCES)))

.PHONY: all objects clean
# Delete the default suffixes
.SUFFIXES:

all: $(EXECUTABLE)

# DEPENDANCY COMPILING RULES
#-------------------------------------
objs:$(OBJECTS)

%.o:%.cpp
	g++ $(WARNINGS) $(CFLAGS) $(INCDIRS) $(LIBDIRS) -c $< -o $@

%.o:%.c
	gcc $(WARNINGS) $(CFLAGS) $(INCDIRS) $(LIBDIRS) -c $< -o $@

$(EXECUTABLE):$(OBJECTS)
	g++ $(OBJECTS) $(LFLAGS) $(DEBUGGING) -o $@

# RUN / CLEAN
#-------------------------------------

run:
	./$(EXECUTABLE)

clean:
	$(RM) $(OBJECTS) $(EXECUTABLE)
