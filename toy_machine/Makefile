CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = main
OBJDIR = obj

OBJECTS = $(OBJDIR)/main.o \
          $(OBJDIR)/fetch.o \
          $(OBJDIR)/decode.o \
		  $(OBJDIR)/execute.o \
          $(OBJDIR)/mem.o \
		  $(OBJDIR)/writeback.o \
		  $(OBJDIR)/instruction_parser.o \
		  $(OBJDIR)/program_loader.o \
		  $(OBJDIR)/branch_predictors.o \
		  $(OBJDIR)/utils.o \
		  $(OBJDIR)/label.o

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/main.o: main.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c main.c -o $@

$(OBJDIR)/fetch.o: pipeline/stages/fetch.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c pipeline/stages/fetch.c -o $@

$(OBJDIR)/decode.o: pipeline/stages/decode.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c pipeline/stages/decode.c -o $@

$(OBJDIR)/execute.o: pipeline/stages/execute.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c pipeline/stages/execute.c -o $@

$(OBJDIR)/mem.o: pipeline/stages/mem.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c pipeline/stages/mem.c -o $@

$(OBJDIR)/writeback.o: pipeline/stages/wb.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c pipeline/stages/wb.c -o $@

$(OBJDIR)/instruction_parser.o: program/get_program/instruction_parser.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c program/get_program/instruction_parser.c -o $@

$(OBJDIR)/program_loader.o: program/get_program/program_loader.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c program/get_program/program_loader.c -o $@

$(OBJDIR)/branch_predictors.o: architectureLayer/branch_predictor/branch_predictors.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c architectureLayer/branch_predictor/branch_predictors.c -o $@

$(OBJDIR)/utils.o: pipeline/stages/utils.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c pipeline/stages/utils.c -o $@

$(OBJDIR)/label.o: architectureLayer/labels/label.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c architectureLayer/labels/label.c -o $@


clean:
	rm -rf $(OBJDIR) $(TARGET)