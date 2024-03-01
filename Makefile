CC=g++
CFLAGS=
PROGRAM=scriptReader

START: $(PROGRAM)
	./$(PROGRAM)

$(PROGRAM):PREVINSTALL ospath.h stringExpand.h
	$(CC) -o $(PROGRAM) $(PROGRAM).cpp

xv6: xv6_assemble
	$(CC) -o $< $<.cpp

PREVINSTALL:
	sudo apt-get install g++
	sudo apt-get install gcc
	sudo apt-get install gdb

clean: fileEraser
	$(CC) -o $< $<.cpp
	./fileEraser
	
