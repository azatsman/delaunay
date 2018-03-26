CXXFLAGS = -O3 -Wall -I..

ut-isinside: ut-isinside.cpp ../triang.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

ut-rand-det: ut-rand-det.cpp ../triang.hpp
	$(CXX) $(CXXFLAGS) -o $@ $< -lm

ut-triang1: ut-triang1.cpp ../triang.hpp
	$(CXX) $(CXXFLAGS) -o $@ $< -lm

ut-triang2: ut-triang2.cpp ../triang.hpp utils.hpp
	$(CXX) $(CXXFLAGS) -o $@ $< -lm

ut-circum: ut-circum.cpp ../triang.hpp
	$(CXX) $(CXXFLAGS) -o $@ $< -lm

TARGETS = ut-isinside ut-rand-det ut-triang1 ut-circum  ut-triang2

all: $(TARGETS)

clean:
	rm -rf *.o *~ $(TARGETS)
