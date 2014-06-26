EXAMPLE_CPP := $(wildcard example/*.cpp)
EXAMPLE_OUT := $(EXAMPLE_CPP:example/%.cpp=%.bin)

CXXFLAGS := -Wall -std=c++1y -I. -Ilibs/glfw/include
CXX := clang++
LDFLAGS := -Llibs -lglfw3 -lGLEW -lpthread -ljpeg
OS := $(shell uname -s)

ifeq ($(OS), Linux)
	LDFLAGS += -lGL -lGLU -lX11 -lXxf86vm -lXi -lXrandr
endif
ifeq ($(OS), Darwin)
	LDFLAGS += -framework Cocoa -framework OpenGL -framework CoreVideo -framework IOKit
endif

ifeq ($(RELEASE),1)
	CXXFLAGS += -O3
else
	CXXFLAGS += -g
endif

ifeq ($(ASAN), 1)
	CXXFLAGS += -fsanitize=address
	LDFLAGS += -fsanitize=address
endif


.PHONY: clean all clear new fancy test

all: $(EXAMPLE_OUT)

%.bin: example/%.cpp lumina/lumina.a
	$(CXX) $(CXXFLAGS) -o $@ $< lumina/lumina.a $(LDFLAGS)


clean:
	rm -f *.bin