CC := g++

SRCDIR := src
BUILDDIR := build
INC := -lboost_system -lboost_date_time -lboost_thread -lpthread -lm -ltorrent-rasterbar -lncurses -ltinfo
SRCEXT := cpp

CPP_FILES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
O_FILES := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(CPP_FILES:.$(SRCEXT)=.o))

CFLAGS := -g #-std=c++11 # -Wall

.PHONY: all clean
.DEFAULT: all

all: $(BUILDDIR) | program

program: $(O_FILES)
		@echo "Link object files ..."
		@echo "$(CC) -o $@ $^"; $(CC) -o $@ $^ $(INC) 

$(BUILDDIR):
	mkdir -p $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
		@echo "Compile time ..."
		@echo "$(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) -c -o $@ $<


clean:
		rm -f program
		rm -f $(O_FILES)
