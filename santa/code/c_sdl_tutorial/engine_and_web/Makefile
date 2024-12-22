# Returns all c files nested or not in $(1)
define collect_sources
	$(shell find $(1) -name '*.c')
endef

# Modify these variables to apply your preferences
OBJ_DIR := objects
EXE_NAME := bin

SOURCES := $(call collect_sources, src)
OBJECTS := $(patsubst %.c, $(OBJ_DIR)/%.o, $(SOURCES))

L_FLAGS := `pkg-config --libs sdl2 SDL2_image SDL2_mixer SDL2_ttf` -lm

.PHONY: run clean
.ALL: run

run: $(EXE_NAME)
	@./$(EXE_NAME)

$(EXE_NAME): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXE_NAME) $(L_FLAGS)

$(OBJ_DIR)/%.o: %.c
	@# Making sure that the directory already exists before creating the object
	@# All object files will be placed on a special, isolated directory
	@mkdir -p $(dir $@)

	$(CC) -D NO_AUDIO -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	rm $(EXE_NAME)
