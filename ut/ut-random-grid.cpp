// Test circumscribed circles

// #define DEBUG_TRIANG 2

#ifndef DEBUG_TRIANG
#define  DEBUG_TRIANG 1
#endif

#include "triang.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cmath>



template <typename T>
int runTest (int numPoints, int gridWidth, int gridHeight,
	     T delta, const char* xgFileName, const char* psFileName)
{
  int
    rcode = 0;
  std::vector<Point<T> > pSet;
  std::set <Triangle>    trSet;

  T
    delaynayCheckPrecision = static_cast<T> (1e-4);

  for (int k=0; k<numPoints; k++) {
    T
      xx = static_cast<T> (gridWidth  * DRAND()) + static_cast<T> (delta * DRAND()),
      yy = static_cast<T> (gridHeight * DRAND()) + static_cast<T> (delta * DRAND());
    pSet.push_back (Point<T> (xx, yy));
  }
  try {
    delaunay<T> (pSet, trSet);
  }
  catch (std::runtime_error re) {
    std::cerr
      << " Runtime Error caught: \"" << re.what() << "\"" << std::endl;
    dumpXG (pSet, trSet, xgFileName);
    dumpPS (pSet, trSet, psFileName);
    return 4;
  }
  const char* dumpName = "error.xg";
  try {
    edgeCrossingCheck (pSet, trSet);
    delaunayCheck     (pSet, trSet, delaynayCheckPrecision);
  }
  catch (EdgeCrossError<T> ee) {
    std::cerr << " Edge Crossing EXCEPTION. Dumping to \"" << dumpName << "\"" << std::endl;
    dumpEdgeCrossError (ee, dumpName);
    rcode = 1;
  }
  catch (DelaunayError<T> de) {
    std::cerr
      << " DelaunayError EXCEPTION caught. Dumping to \"" << dumpName << "\"" << std::endl;
    dumpDelaunayError (de, dumpName);
    rcode = 2;
  }
  catch (std::runtime_error re) {
    std::cerr
      << " Runtime Error caught: \"" << re.what() << "\"" << std::endl;
    rcode = 3;
  }
  dumpXG (pSet, trSet, xgFileName);
  dumpPS (pSet, trSet, psFileName);
  return rcode;
}

int main (int argc, const char *argv[])
{
  int
    numPoints = 200,
    gridWidth =  10,
    gridHeight =  7,
    seed = 19;
  double
    delta  = 0.0001;
  if (argc > 1)
    sscanf (argv[1], "%d", &numPoints);
  if (argc > 2)
    sscanf (argv[2], "%d", &gridWidth);
  if (argc > 3)
    sscanf (argv[3], "%d", &gridHeight);
  if (argc > 4)
    sscanf (argv[4], "%lf", &delta);
  SRAND (seed);

    runTest<int>    (numPoints, gridWidth, gridHeight,     0, "gr-int.xg", "gr-int.ps");
    //runTest<double> (numPoints, gridWidth, gridHeight, delta, "gr-dbl.xg", "gr-dbl.ps");

  return 0;
}
