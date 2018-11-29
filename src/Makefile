include ./build.config

JSON_INCLUDE_PATH=$(PATH_SRC)/lib/json/include/json-c
JSON_LIB_PATH=$(PATH_SRC)/lib/json/lib/libjson-c.a

MAKE=make
INCLUDES = -I. -I$(JSON_INCLUDE_PATH)
EXTRA_CFLAGS+=-rdynamic
CFLAGS =  $(DEFINES) $(INCLUDES) $(EXTRA_CFLAGS) -g -Wstrict-prototypes -Wall -O2 -pthread 
MAKE_FLAGS  += --no-print-directory

LIB = $(JSON_LIB_PATH)
PROGRAM = ../cmd_agent
OBJ = $(SRC:.c=.o)
SRC = main.c parse.c tcp.c session.c
.SUFFIXES : .c .o

all : $(PROGRAM)

$(PROGRAM) : $(OBJ)
			   $(CC) $(CFLAGS) -o $@ $(OBJ) $(LIB)

clean :
	rm -rf $(PROGRAM) $(OBJ)
