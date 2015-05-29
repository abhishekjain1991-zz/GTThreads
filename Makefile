#### GTThread Library Makefile

CFLAGS  = -Wall -pedantic
LFLAGS  =
CC      = gcc
RM      = /bin/rm -rf
AR      = ar rc
RANLIB  = ranlib

LIBRARY = gtthread.a

LIB_SRC = gtthread.c Dining_Philosophers.c

LIB_OBJ = $(patsubst %.c,%.o,$(LIB_SRC))


TARGET = Dining_Philosophers

# pattern rule for object files
%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

all: $(LIBRARY) $(TARGET)

$(LIBRARY): $(LIB_OBJ)
	$(AR) $(LIBRARY) $(LIB_OBJ)
	$(RANLIB) $(LIBRARY)
 

$(TARGET):$(LIB_SRC)
	$(CC) $(CFLAGS) $(LIB_SRC) -w -o $(TARGET) $(LIBRARY)

clean:
	$(RM) $(LIBRARY) $(LIB_OBJ)
	$(RM) $(TARGET) $(TARGET).o

.PHONY: depend
depend:
	$(CFLAGS) -- $(LIB_SRC)  2>/dev/null
