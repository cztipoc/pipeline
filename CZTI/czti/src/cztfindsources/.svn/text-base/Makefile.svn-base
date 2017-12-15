CPP=g++
FLAGS=-w -c -Wall

EXE=cztfindsources
BINDIR=../../bin
INCLUDE=-I../include -I../../include

RM=rm
MK=mkdir -p

MODULELIBDIR=../lib
MODULELIB=$(MODULELIBDIR)/lib$(EXE).a

LIB=-L../../lib -lpil $(MODULELIBDIR)/libcommon.a -lcfitsio -lm -lglog -lpthread

SOURCES=cztfindsources.cpp 
OBJECTS=$(SOURCES:.cpp=.o) 
	
all: $(EXE) $(MODULELIB)

$(EXE): $(OBJECTS) 
	$(CPP) -o $(EXE) main.cpp $(OBJECTS) $(INCLUDE) $(LIB)
	cp $(EXE) $(BINDIR)	
	
$(MODULELIB): $(OBJECTS)
	ar cr $(MODULELIB) $(OBJECTS)
	
.cpp.o:
	$(CPP) $(FLAGS) $< -o $@ $(INCLUDE) 

clean:
	$(RM) $(OBJECTS)
