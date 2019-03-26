IDIR=include
CC=g++
CFLAGS=-std=c++17 -I$(IDIR) -MMD -MP

SDIR=src
ODIR=obj
LDIR=lib

LIBS=-lglew.2.1 -lglfw.3.2 -framework OpenGL

SRC=$(wildcard $(SDIR)/*.cpp)

OBJ=$(patsubst $(SDIR)/%.cpp,$(ODIR)/%.o,$(SRC))

DEP=$(wildcard $(IDIR)/*.h)

EXE=rel

all: $(OBJ) $(EXE)

$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LIBS)

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

-include $(wildcard $(ODIR)/*.d)

.PHONY: clean
clean:
	-rm -f $(ODIR)/*.o $(ODIR)/*.d *~ core $(IDIR)/*~ rel