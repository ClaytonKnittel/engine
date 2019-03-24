IDIR=./include
CC=g++
CFLAGS=-c -std=c++17 -I$(IDIR)

SDIR=src
ODIR=obj
LDIR=lib

LIBS=-lglew.2.1 -lglfw.3.2 -framework OpenGL

SRC=$(wildcard $(SDIR)/*.cpp)

OBJ=$(patsubst $(SDIR)/%.cpp,$(ODIR)/%.o,$(SRC))

EXE=rel

all: $(OBJ) $(EXE)

$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LIBS)

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	-rm -f $(ODIR)/*.o *~ core $(IDIR)/*~