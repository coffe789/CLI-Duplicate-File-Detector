PROJECT = duplicates
HEADERS = $(PROJECT).h
OBJ = duplicates.o

C11	= cc -std=c11
CFLAGS = -Wall -Werror
$(PROJECT) : $(OBJ)
	$(C99) $(CFLAGS) -o $(PROJECT) $(OBJ)

%.o : %.c $(HEADERS)
	$(C11) $(CFLAGS) -c $<

clean:
	rm -f $(PROJECT) $(OBJ)