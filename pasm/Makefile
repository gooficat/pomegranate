
run_qemu: run qemu_test

launch: build run

run:
	@$(TARGET) $(ARGS)

qemu_test:
	@qemu-system-x86_64 -drive file=../examples/hello.bin,format=raw

build:
	@$(CC) -o $(TARGET) $(FLAGS) $(SOURCES)

debug:
	@gdb -batch -ex "set args $(ARGS)" -ex "run" $(TARGET) -ex "bt"


CC = clang
FLAGS =-O0 -g
#  -std=c23 -Wall -Wextra -Wpedantic -wno-c23-extensions -Wno-gnu-statement-expression-from-macro-expansion 
SOURCES = src/*.c

TARGET=./pasm
ARGS = ../examples/hello.psm ../examples/hello.bin

