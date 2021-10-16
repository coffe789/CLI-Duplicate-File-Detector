PROJECT = duplicates
HEADERS = $(PROJECT).h
OBJ = duplicates.o filesizes.o strSHA2.o 

C11	= cc -std=c11
CFLAGS = -Wall -Werror
$(PROJECT) : $(OBJ)
	$(C11) $(CFLAGS) -o $(PROJECT) $(OBJ)

%.o : %.c $(HEADERS)
	$(C11) $(CFLAGS) -c $<

clean:
	rm -f $(PROJECT) $(OBJ)
