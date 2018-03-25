CXXFLAGS = -std=c++14
LFLAGS = -lpcap -lcurl
CXX = g++

testpost.out: testpost.cpp
	$(CXX) $< $(CXXFLAGS) $(LFLAGS) -o $@

capture.out: capture.cpp
	$(CXX) $< $(CXXFLAGS) $(LFLAGS) -o $@

