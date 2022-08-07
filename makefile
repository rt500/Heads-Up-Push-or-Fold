CC = g++

# make platform agnostic
ifeq ($(OS), Windows_NT)
	my_os := Windows
else
	my_os := $(shell uname -s)
endif

ifeq ($(my_os), Windows)
	target = headsup_nash.exe
	delete_cmd = del
endif
ifeq ($(my_os), Linux)
	target = headsup_nash
	delete_cmd = rm -f
endif

# https://stackoverflow.com/questions/2514903/
.PHONY: all
all: $(target)

headsup_nash.o: src/headsup_nash.cpp
	$(CC) -std=c++20 -c -Iinclude/ -o $@ $<

strategy.o: src/strategy.cpp
	$(CC) -std=c++20 -c -Iinclude/  -o $@ $<

$(target): headsup_nash.o strategy.o 
	$(CC) -std=c++20 -Iinclude/ -o $@ $^


.PHONY: clean
clean:
	@$(delete_cmd) *.o
