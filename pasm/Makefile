
launch: build run


debug:
	@gdb -batch -ex "set args $(ARGS)" -ex "run" $(TARGET) -ex "bt"


CC = clang
FLAGS = -Wall -O0 -g

SOURCES = src/*.c

TARGET=./pasm
ARGS = ../examples/hello.psm ../examples/hello.bin
build:
	@$(CC) -o $(TARGET) $(FLAGS) $(SOURCES)

run:
	@$(TARGET) $(ARGS)
