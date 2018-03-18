CC=g++
CFLAGS=-c -g -O3 -Wall -Idtk `pkg-config gtkmm-3.0 --cflags`
LDFLAGS=`pkg-config --libs gtkmm-3.0` -lpthread
SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(SOURCES:src/%.cpp=obj/%.o)
EXECUTABLE=fieshdom

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@  $(LDFLAGS) 

${OBJECTS}: obj/%.o : src/%.cpp
	$(CC) $(CFLAGS) $< -o $@


clean:
	rm $(OBJECTS)
	rm $(EXECUTABLE)
