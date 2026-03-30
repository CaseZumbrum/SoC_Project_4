# Variables
SYSTEMC_HOME = /opt/systemc
CXX          = g++
CXXFLAGS     = -I$(SYSTEMC_HOME)/include -L$(SYSTEMC_HOME)/lib \
               -lsystemc -lm -O3 -std=c++17

# Target executable
TARGET = soc_sim

# Source files
SRCS = sc_main.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(SRCS) -o $(TARGET) $(CXXFLAGS)

clean:
	rm -f $(TARGET) *.o output_frame.ppm

run: all
	./$(TARGET)
