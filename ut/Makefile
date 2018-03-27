CXXFLAGS = -O3 -Wall -I..

ut-isinside: ut-isinside.cpp ../triang.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

ut-triang-random: ut-triang-random.cpp ../triang.hpp utils.hpp
	$(CXX) $(CXXFLAGS) -o $@ $< -lm

ut-triang-grid: ut-triang-grid.cpp ../triang.hpp utils.hpp
	$(CXX) $(CXXFLAGS) -o $@ $< -lm

ut-circum: ut-circum.cpp ../triang.hpp
	$(CXX) $(CXXFLAGS) -o $@ $< -lm

TARGETS = ut-isinside  ut-circum  ut-triang-grid  ut-triang-random

all: $(TARGETS)

clean:
	rm -rf *.o *~ $(TARGETS) *.xg *.ps
