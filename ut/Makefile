
DEBUG_TRIANG = 2

ifeq ($(DEBUG_TRIANG), 0)
CXXFLAGS = -O3 -Wall -I..
else
CXXFLAGS = -g -Wall -I.. -DDEBUG_TRIANG=$(DEBUG_TRIANG)
endif

ut-isinside: ut-isinside.cpp ../triang.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

ut-triang-random: ut-triang-random.cpp ../triang.hpp utils.hpp
	$(CXX) $(CXXFLAGS) -o $@ $< -lm

ut-triang-grid: ut-triang-grid.cpp ../triang.hpp utils.hpp
	$(CXX) $(CXXFLAGS) -o $@ $< -lm

ut-circum: ut-circum.cpp ../triang.hpp
	$(CXX) $(CXXFLAGS) -o $@ $< -lm

dump-xg-frames: dump-xg-frames.cpp ../triang.hpp
	$(CXX) $(CXXFLAGS) -o $@ $< -lm

TARGETS = ut-isinside  ut-triang-grid  ut-triang-random dump-xg-frames ut-circum

all: $(TARGETS)

clean:
	rm -rf *.o *~ $(TARGETS) *.xg *.ps
