CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = FDEloop
OBJDIR = obj

OBJECTS = $(OBJDIR)/main.o \
          $(OBJDIR)/initial_state.o \
          $(OBJDIR)/fetch.o \
          $(OBJDIR)/decode.o \
		  $(OBJDIR)/resolve_labels.o \
          $(OBJDIR)/instruction_parser.o \
		  $(OBJDIR)/program_build.o \
		  $(OBJDIR)/program_loader.o \
		  $(OBJDIR)/executeALU.o \
		  $(OBJDIR)/executeDATAMOVEMENT.o \
		  $(OBJDIR)/executeLOADSTORE.o \
		  $(OBJDIR)/executeBRANCH.o \
		  $(OBJDIR)/executeHALT.o

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/main.o: FDEloop.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c FDEloop.c -o $@

$(OBJDIR)/initial_state.o: architectureLayer/initial_state.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c architectureLayer/initial_state.c -o $@

$(OBJDIR)/fetch.o: letsPipeline/src/fetch.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c letsPipeline/src/fetch.c -o $@

$(OBJDIR)/decode.o: letsPipeline/src/decode.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c letsPipeline/src/decode.c -o $@

$(OBJDIR)/resolve_labels.o: getProgram/src/resolve_labels.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c getProgram/src/resolve_labels.c -o $@

$(OBJDIR)/instruction_parser.o: getProgram/src/instruction_parser.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c getProgram/src/instruction_parser.c -o $@

$(OBJDIR)/program_build.o: getProgram/src/program_build.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c getProgram/src/program_build.c -o $@

$(OBJDIR)/program_loader.o: getProgram/src/program_loader.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c getProgram/src/program_loader.c -o $@

$(OBJDIR)/executeALU.o: letsPipeline/src/executeALU.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c letsPipeline/src/executeALU.c -o $@

$(OBJDIR)/executeDATAMOVEMENT.o: letsPipeline/src/executeDATAMOVEMENT.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c letsPipeline/src/executeDATAMOVEMENT.c -o $@

$(OBJDIR)/executeLOADSTORE.o: letsPipeline/src/executeLOADSTORE.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c letsPipeline/src/executeLOADSTORE.c -o $@

$(OBJDIR)/executeBRANCH.o: letsPipeline/src/executeBRANCH.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c letsPipeline/src/executeBRANCH.c -o $@

$(OBJDIR)/executeHALT.o: letsPipeline/src/executeHALT.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c letsPipeline/src/executeHALT.c -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET)