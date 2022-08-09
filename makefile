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
all: bin/$(target)


obj/headsup_nash.o: src/headsup_nash.cpp | obj
	$(CC) -std=c++20 -c -Iinclude/ -o $@ $<

obj/strategy.o: src/strategy.cpp | obj
	$(CC) -std=c++20 -c -Iinclude/  -o $@ $<

bin/$(target): obj/headsup_nash.o obj/strategy.o | bin
	$(CC) -std=c++20 -Iinclude/ -o $@ $^

# https://stackoverflow.com/questions/12605051/
obj:
	$(make_dir) $@

bin:
	$(make_dir) $@

.PHONY: clean
clean:
	ifeq ($(my_os), Windows)
		del obj\*.o
	ifeq ($(my_os), Linux)
		rm -f obj/*.o