IDIR=./include
CC=g++
CFLAGS=-std=c++17 -I$(IDIR)

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

include $(wildcard $(ODIR)/*.d)

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CC) -c $(CFLAGS) $< -o $@
	$(CC) -MM $(CFLAGS) $< > $(ODIR)/$*.d

.PHONY: clean
clean:
	-rm -f $(ODIR)/*.o $(ODIR)/*.d *~ core $(IDIR)/*~