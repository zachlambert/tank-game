
CC = gcc
CFLAGS = `sdl2-config --cflags` -g -lefence
LFLAGS = `sdl2-config --libs`

OBJ = ./obj
SRC = ./src
INC = ./include
PROG_NAME = main
TARGET = $(PROG_NAME)

LIB_INC =
LIB_FILES =
LIB_NAMES = SDL2_image SDL2_ttf SDL2_mixer m
LIBS = $(addprefix -L, $(LIB_FILES)) $(addprefix -l, $(LIB_NAMES))

INCLUDES = -I$(INC) $(addprefix -I, $(LIB_INC))

HEADERS = $(wildcard $(INC)/*.h) $(wildcard $(INC)/**/*.h)
SOURCES = $(wildcard $(SRC)/*.c) $(wildcard $(SRC)/**/*.c)
OBJECTS = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))

$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@ $(LIBS) $(LFLAGS)

.SECONDEXPANSION:
$(OBJ)/%.o: $(SRC)/%.c $(HEADERS) | $$(dir $$@)
	$(CC) -c $< -o $@ $(INCLUDES) $(CFLAGS)

%/:
	@mkdir -p $@

.PRECIOUS: %/

.PHONY: clean

clean:
	rm -r $(OBJ)
