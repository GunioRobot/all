EXECUTABLE	=	main

OFLAGS		=
LFLAGS		=	-lglw -lglfw -lGL -lXrandr

WARNINGS	=	-W -Wall -Wextra -Wfatal-errors -Werror -Wwrite-strings -Wcast-qual -Wconversion -pedantic
DEBUGGING	=

SRCDIRS		=	.
HDRDIRS		=	../include/ ./include/ ./include2/
LNKDIRS		=	../lib/ ./lib/

SRCEXTS = .cpp .c

CFLAGS		=
CPPFLAGS	=	-std=c++0x

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
	g++ $(WARNINGS) $(OFLAGS) $(CPPFLAGS) $(INCDIRS) -c $< -o $@

%.o:%.c
	gcc $(WARNINGS) $(OFLAGS) $(CFLAGS) $(INCDIRS) -c $< -o $@

$(EXECUTABLE):$(OBJECTS)
	g++ $(OBJECTS) $(LIBDIRS) $(LFLAGS) $(DEBUGGING) -o $@

# RUN / CLEAN
#-------------------------------------

run:
	./$(EXECUTABLE)

clean:
	$(RM) $(OBJECTS) $(EXECUTABLE)
