// Test circumscribed circles

#include "triang.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <iostream>
#include <fstream>

#ifndef DEBUG_TRIANG
#define  DEBUG_TRIANG 0
#endif

int main (int argc, const char *argv[])
{
  int numPoints = 100;

  bool runChecks = false;  // Checking can double the total execution time.
#if DEBUG_TRIANG
  runChecks = true;
#endif
  
  if (argc > 1)
    sscanf (argv[1], "%d", &numPoints);

  const char* psFileName = "random.ps";
  const char* xgFileName = "random.xg";

  SRAND (17);

  std::vector<Point<double> > pSet;
  std::set <Triangle>        trSet;
  for (int k=0; k<numPoints; k++)
    pSet.push_back (Point<double> (DRAND(), DRAND()));

  delaunay (pSet, trSet);

  dumpXG (pSet, trSet, xgFileName);
  dumpPS (pSet, trSet, psFileName);

  if (runChecks) {
    try {
      delaunayCheck (pSet, trSet);
    }
    catch (DelaunayError<double> de) {
      const char* dumpName = "delaunay-error.xg";
      std::cerr
	<< " DelaunayError EXCEPTION caught. Dumping to \"" << dumpName << "\"" << std::endl;
      dumpDelaunayError (de, dumpName);
    }
  }
  return 0;
}
