CC=g++
CFLAGS=
PROGRAM=scriptReader
INSTALL_PACKAGE=g++ gcc gdb

START: $(PROGRAM)
	./$(PROGRAM)

$(PROGRAM):PREVINSTALL ospath.h stringExpand.h
	$(CC) -o $(PROGRAM) $(PROGRAM).cpp

xv6: xv6_assemble
	$(CC) -o $< $<.cpp
	./$<

backup: backup PREVINSTALL_BACKUP
	$(CC) -o $@ $@.cpp -lcrypto
	./$@ sha1

PREVINSTALL_BACKUP:
	sudo apt-get install libssl-dev

PREVINSTALL:
	sudo apt-get install $(INSTALL_PACKAGE)
	

clean: fileEraser
	$(CC) -o $< $<.cpp
	./fileEraser
	
