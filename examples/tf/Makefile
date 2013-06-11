TARGET = tf

LLVM_DIR = /opt/llvm/3.2

CXX = $(LLVM_DIR)/bin/clang++
LLC = $(LLVM_DIR)/bin/llc

CXXFLAGS = -Wall -emit-llvm -O0 -fno-exceptions
LD = $(LLVM_DIR)/bin/llvm-link

all:
	$(CXX) $(CXXFLAGS) -c $(TARGET).cpp -o $(TARGET).bc

tf:
	$(LLC) $(TARGET).tf.bc -o $(TARGET).tf.s
	$(CXX) -O0 -g -Wall $(TARGET).tf.s ../../aux/helper.cpp -o $(TARGET).x

clean:
	rm -f $(TARGET) $(TARGET).bc $(TARGET).tf* graph* $(TARGET).x

