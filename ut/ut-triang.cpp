// Test circumscribed circles

#define WITH_CHECKS 0

#include "triang.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <iostream>
#include <fstream>

static bool useGrid = false;

int main (int argc, const char *argv[])
{
  const char* psFileName = "triang.ps";
  const char* xgFileName = "triang.xg";
    
  int WW = 7;
  int HH = 5;
  double delta = 0.0;
  srand48 (17);
  if (argc > 1)
    sscanf (argv[1], "%d", &WW);
  if (argc > 2)
    sscanf (argv[2], "%d", &HH);
  if (argc > 3)
    sscanf (argv[3], "%lf", &delta);

  std::vector<Point<double> > pSet;
  std::set <Triangle>  trSet;

  if (useGrid) {
    for   (int iy=0; iy<HH; iy++) {
      for (int ix=0; ix<WW; ix++) {
	pSet.push_back (Point<double> (ix + delta * (drand48()-0.5),
				       iy + delta * (drand48()-0.5)));
      }
    }
  }
  else { // Do random set
    for (int k=0; k<WW; k++)
      pSet.push_back (Point<double> (drand48(), drand48()));
  }
  try {
    delaunay (pSet, trSet);
  }
  catch (DelaunayError<double> de) {
    const char* dumpName = "delaunay-error.xg";
    std::cerr
      << " DelaunayError EXCEPTION caught. Dumping to \"" << dumpName << "\"" << std::endl;
    dumpDelaunayError (de, dumpName);
  }

  {
    std::ofstream xgf (xgFileName);
    xgf << "TitleText: " << pSet.size() << " points "
	<< trSet.size() << " triangles" << std::endl;
    for (std::set<Triangle>::const_iterator q = trSet.begin(); q != trSet.end(); q++) {
      xgf
	<< "move " << pSet[q->ix0].x << " " <<  pSet[q->ix0].y << std::endl
	<< "     " << pSet[q->ix1].x << " " <<  pSet[q->ix1].y << std::endl
	<< "     " << pSet[q->ix2].x << " " <<  pSet[q->ix2].y << std::endl
	<< "     " << pSet[q->ix0].x << " " <<  pSet[q->ix0].y << std::endl;
    }
  }
  dumpPS (pSet, trSet, psFileName);

  // trSet.clear();
  // delaunay (pSet, trSet);
  return 0;
}
