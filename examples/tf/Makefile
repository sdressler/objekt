TARGET = tf

LLVM_DIR = /usr

CXX = $(LLVM_DIR)/bin/clang++
LLC = $(LLVM_DIR)/bin/llc

CXXFLAGS = -Wall -emit-llvm -O3
LD = $(LLVM_DIR)/bin/llvm-link

all:
	$(CXX) $(CXXFLAGS) -c $(TARGET).cpp -o $(TARGET).bc
	$(CXX) -O3 -Wall $(TARGET).cpp -o $(TARGET).x -L/home/bzcdress/repos/elapsd/lib -lelapsd 

tf:
	$(LLC) $(TARGET).tf.bc -o $(TARGET).tf.s
	$(CXX) -O3 -g -Wall $(TARGET).tf.s ../../aux/helper.cpp -o $(TARGET).tf.x -L/home/bzcdress/repos/elapsd/lib -lelapsd

clean:
	rm -f $(TARGET) $(TARGET).bc $(TARGET).tf* graph* $(TARGET).x

