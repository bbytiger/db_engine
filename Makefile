OBJS = \
	main.o \
	storage.o \

LINK_TARGET = db.o

%.o: %.c
	gcc -Wall -g -o $@ -c $< 

all: $(LINK_TARGET)

$(LINK_TARGET): $(OBJS)
	gcc -Wall -g -o $@ $^

clean:
	rm $(OBJS) $(LINK_TARGET) *.db

main.o: storage.h
storage.o: storage.h