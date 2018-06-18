CC := g++

SRCDIR := src
BUILDDIR := build
INC := -lmath 
SRCEXT := cpp

CPP_FILES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
O_FILES := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(CPP_FILES:.$(SRCEXT)=.o))

CFLAGS := -g # -Wall

.PHONY: all clean
.DEFAULT: all

all: program

program: $(O_FILES)
		@echo "Link object files ..."
		@echo "$(CC) -o $@ $^"; $(CC) -o $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
		@echo "Compile time ..."
		@echo "$(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) -c -o $@ $<


clean:
		rm -f program
		rm -f $(O_FILES)