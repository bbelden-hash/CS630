CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = FDEloop
OBJDIR = obj

OBJECTS = $(OBJDIR)/main.o \
          $(OBJDIR)/initial_state.o \
          $(OBJDIR)/fetch.o \
          $(OBJDIR)/decoder.o \
          $(OBJDIR)/program.o \
		  $(OBJDIR)/execute.o

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/main.o: FDEloop.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c FDEloop.c -o $@

$(OBJDIR)/initial_state.o: architectureLayer/initial_state.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c architectureLayer/initial_state.c -o $@

$(OBJDIR)/fetch.o: programmingLayer/_cFiles/fetch.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c programmingLayer/_cFiles/fetch.c -o $@

$(OBJDIR)/decoder.o: programmingLayer/_cFiles/decoder.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c programmingLayer/_cFiles/decoder.c -o $@

$(OBJDIR)/program.o: programmingLayer/_cFiles/program.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c programmingLayer/_cFiles/program.c -o $@

$(OBJDIR)/execute.o: programmingLayer/_cFiles/execute.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c programmingLayer/_cFiles/execute.c -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET)