LIBRARY		=	libr3d.a

OFLAGS		=	-O3

WARNINGS	=	-W -Wall -Wextra -Wfatal-errors -Werror -Wwrite-strings -Wcast-qual -Wconversion
#-pedantic

SRCDIRS		=	.
HDRDIRS		=	./include/ ../../include/

SRCEXTS = .cpp .c

CFLAGS		=
CPPFLAGS	=	-std=c++0x

#do not edit below this
ifeq ($(SRCDIRS),)
  SRCDIRS = .
endif

SOURCES = $(foreach d,$(SRCDIRS),$(wildcard $(addprefix $(d)/*,$(SRCEXTS))))
INCDIRS = $(foreach d,$(HDRDIRS),$(addprefix -I, $(d)))

OBJECTS = $(addsuffix .o, $(basename $(SOURCES)))

.PHONY: all objects clean
# Delete the default suffixes
.SUFFIXES:

all: $(LIBRARY)

# DEPENDANCY COMPILING RULES
#-------------------------------------
objs:$(OBJECTS)

%.o:%.cpp
	g++ $(WARNINGS) $(OFLAGS) $(CPPFLAGS) $(INCDIRS) -c $< -o $@

%.o:%.c
	gcc $(WARNINGS) $(OFLAGS) $(CFLAGS) $(INCDIRS) -c $< -o $@

$(LIBRARY):$(OBJECTS)
	ar rc $@ $(OBJECTS)
	ranlib $@

# RUN / CLEAN
#-------------------------------------

clean:
	$(RM) $(OBJECTS) $(LIBRARY)
