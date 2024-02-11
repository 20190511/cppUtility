CC=g++
CFLAGS=
PROGRAM=scriptReader

START: $(PROGRAM)
	./$(PROGRAM)

$(PROGRAM):PREVINSTALL ospath.h stringExpand.h
	$(CC) -o $(PROGRAM) $(PROGRAM).cpp

PREVINSTALL:
	sudo apt-get install g++
	sudo apt-get install gcc
	sudo apt-get install gdb
clean:
	rm -rf $(PROGRAM) xv6_assemble
