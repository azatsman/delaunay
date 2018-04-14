
DEBUG_TRIANG = 0

ifeq ($(DEBUG_TRIANG), 0)
CXXFLAGS = -O3
else
CXXFLAGS = -g
endif

CXXFLAGS +=  -Wall -I..
CXXFLAGS +=  -DDEBUG_TRIANG=$(DEBUG_TRIANG)
CXXFLAGS += -ftrapv

ut-random: ut-random.cpp ../triang.hpp utils.hpp
	$(CXX) $(CXXFLAGS) -o $@ $< -lm

ut-random-grid: ut-random-grid.cpp ../triang.hpp utils.hpp
	$(CXX) $(CXXFLAGS) -o $@ $< -lm

ut-grid: ut-grid.cpp ../triang.hpp utils.hpp
	$(CXX) $(CXXFLAGS) -o $@ $< -lm

ut-greg2: ut-greg2.cpp ../triang.hpp utils.hpp
	$(CXX) $(CXXFLAGS) -o $@ $< -lm

ut-greg1: ut-greg1.cpp ../triang.hpp utils.hpp
	$(CXX) $(CXXFLAGS) -o $@ $< -lm

TARGETS =  ut-grid ut-random-grid  ut-random ut-greg1 ut-greg2 

all: $(TARGETS)

clean:
	rm -rf *.o *~ $(TARGETS) *.xg *.ps *.log
