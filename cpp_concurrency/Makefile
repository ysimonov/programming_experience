# Barrier Demo Makefile
CXX		 = g++
CXXFLAGS = -Wall -std=c++17
TARGET	 = barrier
SRC		 = barrier.cpp
LDLIBS	 = -lboost_thread -lboost_system

# $(TARGET): $(SRC)
# 	$(CXX) $(CXXFLAGS) $(SRC) $(LDLIBS) -o $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) $(LDLIBS)

.PHONY: clean
clean:
	rm -f $(TARGET)