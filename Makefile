CXX := gcc
CXXFLAGS := -Wall -Wextra -pedantic -std=c17 -march=native -O2
NCURSES_LIBS := -lncurses

TARGET := Snake

SRC := main.c menu.c
OBJ := $(SRC:.c=.o)

all := $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(NCURSES_LIBS)

%.o: %.c
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJ)
