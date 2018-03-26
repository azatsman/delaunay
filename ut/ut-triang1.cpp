// Test circumscribed circles

#include "triang.hpp"
#include <cstdio>
#include <cstdlib>

int main (int argc, const char *argv[])
{
  int numPoints = 20;
  srand48 (17);
  if (argc > 1)
    sscanf (argv[1], "%d", &numPoints);

  std::vector<Point<double> > pSet;
  std::set <Triangle>  trSet;
  for (int k=0; k<numPoints; k++) 
    pSet.push_back (Point<double> (drand48(), drand48()));
  delaunay (pSet, trSet);
  printf ("TitleText: %ld points %ld triangles\n", pSet.size(), trSet.size());
  for (std::set<Triangle>::const_iterator q = trSet.begin(); q != trSet.end(); q++) {
    printf ("move %lf %lf\n", pSet[q->ix0].x, pSet[q->ix0].y);
    printf ("     %lf %lf\n", pSet[q->ix1].x, pSet[q->ix1].y);
    printf ("     %lf %lf\n", pSet[q->ix2].x, pSet[q->ix2].y);
    printf ("     %lf %lf\n", pSet[q->ix0].x, pSet[q->ix0].y);
  }
  trSet.clear();
  delaunay (pSet, trSet);
  return 0;
}
