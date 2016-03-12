CC=gcc -c
DEPENDENCIES=header.h
OBJECTS= shell.o pipe.o redirect.o

%.o: %.c $(DEPENDENCIES)
	$(CC) -o $@ $<

shell: $(OBJECTS)
	gcc -o $@ $^ 
