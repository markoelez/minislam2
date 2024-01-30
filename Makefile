# Compiler settings
CXX = clang++
CXXFLAGS = -std=c++20 -Wno-deprecated-anon-enum-enum-conversion
LIBS = $(shell pkg-config --cflags --libs opencv4) $(shell pkg-config --cflags --libs sdl2)

# Source files
SOURCES = minislam2/*.cc
HEADERS = $(wildcard minislam2/*.hpp)

# Output binary
OUT = out

# Default target
all: $(OUT)

# Linking the program
$(OUT): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(OUT) $(SOURCES) $(LIBS)

# Running the program
run: $(OUT)
	./$(OUT)

# Cleaning up
clean:
	rm -f $(OUT)

.PHONY: format
format:
	clang-format -i $(SOURCES) $(HEADERS)

.PHONY: all run clean