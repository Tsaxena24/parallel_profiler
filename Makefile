# Compiler settings
CXX = g++
CXXFLAGS = -O3 -std=c++11

# Build target
all:
	$(CXX) $(CXXFLAGS) src/profiler.cpp -o pro

# Clean target to remove compiled files
clean:
	rm -f pro *.exe
