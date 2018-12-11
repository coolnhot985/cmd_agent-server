include ./build.config

JSON_INCLUDE_PATH=$(PATH_SRC)/lib/json/include/json-c
JSON_LIB_PATH=$(PATH_SRC)/lib/json/lib/libjson-c.a

MAKE=make
INCLUDES = -I. -I$(JSON_INCLUDE_PATH)
EXTRA_CFLAGS+=-rdynamic
EXTRA_CFLAGS+=-lmysqlclient
CFLAGS =  $(DEFINES) $(INCLUDES) -g -Wstrict-prototypes -Wall -O2 -pthread
MAKE_FLAGS  += --no-print-directory

LIB = $(JSON_LIB_PATH)
PROGRAM = ../cmd_agent
SRC = main.c utils.c session.c parse.c log.c
OBJ = $(SRC:.c=.o)
.SUFFIXES : .c .o

all : $(PROGRAM)

$(PROGRAM) : $(OBJ) 
			   $(CC) $(CFLAGS) -o $@ $(OBJ) $(LIB) $(EXTRA_CFLAGS)

clean :
	rm -rf $(PROGRAM) $(OBJ)
