CXX := clang
CXXFLAGS := -Wall -Wextra -pedantic -std=c17 -march=native -O2 -fsanitize=address
NCURSES_LIBS := -lncurses -ltinfo
LD_FLAGS := -fsanitize=address

TARGET := Snake

SRC := main.c menu.c
OBJ := $(SRC:.c=.o)

all := $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(NCURSES_LIBS) $(LD_FLAGS)

%.o: %.c
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJ)
