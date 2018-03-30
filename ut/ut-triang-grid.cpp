// Test circumscribed circles

#include "triang.hpp"
#include "utils.hpp"
#include <iostream>
#include <fstream>

#ifndef DEBUG_TRIANG
#define  DEBUG_TRIANG 0
#endif

int main (int argc, const char *argv[])
{
  int gridWidth = 7;
  int gridHeight = 5;
  double delta = 0.0;  // amplitude of random deviation from the grid

  bool runChecks = false;  // Checking takes long time for large arrays.
#if DEBUG_TRIANG
  runChecks = true;
#endif

  if (argc > 1)
    sscanf (argv[1], "%d", &gridWidth);
  if (argc > 2)
    sscanf (argv[2], "%d", &gridHeight);
  if (argc > 3)
    sscanf (argv[3], "%lf", &delta);

  SRAND (17);
  const char* psFileName = "grid.ps";
  const char* xgFileName = "grid.xg";

  std::vector<Point<double> > pSet;
  std::set <Triangle>         trSet;
  for   (int iy=0; iy<gridHeight; iy++)
    for (int ix=0; ix<gridWidth; ix++)
      pSet.push_back (Point<double> (ix + delta * (DRAND()-0.5),
				     iy + delta * (DRAND()-0.5)));

  delaunay (pSet, trSet);

  dumpXG (pSet, trSet, xgFileName);
  dumpPS (pSet, trSet, psFileName,
	  static_cast<double> (gridWidth), static_cast<double> (gridHeight));

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
