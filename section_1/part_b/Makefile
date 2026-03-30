# Variables
SYSTEMC_HOME = /opt/systemc
CXX          = g++
CXXFLAGS     = -I$(SYSTEMC_HOME)/include -L$(SYSTEMC_HOME)/lib \
               -lsystemc -lm -O3 -std=c++17

# Target executable
TARGET = soc_sim
IMAGES = output_frame_one.ppm output_frame_two.ppm

# Source files
SRCS = sc_main.cpp
HEADERS = producer.h consumer.h router.h

all: $(TARGET)

$(TARGET): $(SRCS) $(HEADERS)
	$(CXX) $(SRCS) -o $(TARGET) $(CXXFLAGS)

clean:
	rm -f $(TARGET) *.o $(IMAGES)


run: all
	./$(TARGET)
