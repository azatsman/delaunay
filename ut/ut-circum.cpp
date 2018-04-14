// Check circumscribed circles by piping the output to 'xgraph'
//
// The program generates random triangle, then for each random point
//   it prints it out only when the point is inside the circle circumscribed around the triangle.
// Displaying results by 'xgraph' allows visually assess the correctness of the 'isInCircumCircle'
//   function

#include "triang.hpp"
#include <cstdlib>
#include <fstream>

int main (int argc, const char *argv[])
{
  int seed = 17;
  int nn = 99999;
  if (argc > 1)
    sscanf (argv[1], "%d", &seed);
  if (argc > 2)
    sscanf (argv[2], "%d", &nn);
  srand48(seed);
  Point<double>
    a (drand48() - 0.5,drand48() - 0.5),
    b (drand48() - 0.5,drand48() - 0.5),
    c (drand48() - 0.5,drand48() - 0.5);
  for (int k=0; k<nn; k++) {
    Point<double> p (2*(drand48()-0.5), 2*(drand48()-0.5));
    if (isInCircumCircle (p, a, b, c))
      printf ("%lf %lf\n", p.x, p.y);
  }
  printf ("\n\n");
  printf ("%lf %lf\n", a.x, a.y);
  printf ("%lf %lf\n", b.x, b.y);
  printf ("%lf %lf\n", c.x, c.y);
  printf ("%lf %lf\n", a.x, a.y);
  return 0;
}
