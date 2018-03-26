CXXFLAGS = -O3 -Wall -I..

ut-isinside: ut-isinside.cpp ../triang.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

ut-triang: ut-triang.cpp ../triang.hpp utils.hpp
	$(CXX) $(CXXFLAGS) -o $@ $< -lm

ut-circum: ut-circum.cpp ../triang.hpp
	$(CXX) $(CXXFLAGS) -o $@ $< -lm

TARGETS = ut-isinside  ut-circum  ut-triang

all: $(TARGETS)

clean:
	rm -rf *.o *~ $(TARGETS)
