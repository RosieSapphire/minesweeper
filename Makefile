ifdef VERBOSE
	V :=
else
	V := @
endif

BUILD_DIR := build
PROG_BIN  := $(BUILD_DIR)/minesweeper

WARN_INC   := all \
	      extra \
	      everything
WARN_EXC   := reserved-identifier \
	      reserved-macro-identifier \
	      unsafe-buffer-usage
STD_FLAGS  := -std=c99 -pedantic

CC ?= clang

VENDOR_DIR := vendor
GLFW_DIR := vendor/GLFW
GLAD_DIR := vendor/glad
STB_DIR  := vendor/stb

LNK_FLAGS := -lm
ifdef DEBUG
	ifdef ASAN
		ASAN_FLAGS := -fsanitize=address,leak,null,undefined
	endif
	OPT_FLAGS  := -Og
	DBG_FLAGS  := -ggdb3 -DDEBUG
	LNK_FLAGS  += -L$(GLFW_DIR) -l:libglfw3_d.a \
		      -L$(GLAD_DIR) -l:libglad_d.a \
		      -L$(STB_DIR) -l:libstb_image_d.a
else
	ASAN_FLAGS :=
	OPT_FLAGS  := -O3 -ffast-math -g0
	DBG_FLAGS  := -g0 -DNDEBUG
	LNK_FLAGS  += -L$(GLFW_DIR) -l:libglfw3.a \
		      -L$(GLAD_DIR) -l:libglad.a \
		      -L$(STB_DIR) -l:libstb_image.a
	WARN_EXC   += empty-translation-unit
endif

WARN_FLAGS := $(WARN_INC:%=-W%) $(WARN_EXC:%=-Wno-%)

CC_FLAGS  := -fdiagnostics-color=never \
	     $(WARN_FLAGS) \
	     $(ASAN_FLAGS) \
	     $(STD_FLAGS) \
	     $(OPT_FLAGS) \
	     $(DBG_FLAGS)
INC_FLAGS := $(patsubst %,-I%/include,$(GLFW_DIR) $(GLAD_DIR) $(STB_DIR))

SRC_DIRS := src
C_FILES  := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
O_FILES  := $(C_FILES:%.c=$(BUILD_DIR)/%.o)

.PHONY: all clean todo

all: $(PROG_BIN)

run: $(PROG_BIN)
	./$<

$(PROG_BIN): $(O_FILES)
	@echo "    [LD] $(notdir $@)"
	$(V)$(CC) $(CC_FLAGS) $^ -o $@ $(LNK_FLAGS)
	@echo "Success."

$(BUILD_DIR)/%.o: %.c
	$(V)mkdir -p $(dir $@)
	@echo "    [CC] $(notdir $<)"
	$(V)$(CC) $(CC_FLAGS) $(INC_FLAGS) -o $@ -c $<

clean:
	@echo Clearing previous build.
	$(V)rm -rf $(PROG_BIN) $(O_FILES)

todo:
	@grep -rniE "todo|fixme|removeme|#if 0|#if 1"
