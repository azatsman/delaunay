// Test circumscribed circles

#include "triang.hpp"
#include <cstdio>
#include <cstdlib>


int main (int argc, const char *argv[])
{
  int nn = 100000;
  srand48 (17);
  for (int k=0; k<nn; k++) {
    Point<double>
      tp1 (drand48(), drand48()),
      tp2 (drand48(), drand48()),
      tp3 (drand48(), drand48()),
      ctr = circumCtr (tp1, tp2, tp3);

#if 1
    printf ("%8.5lf %8.5lf tp1\n", tp1.x, tp1.y);
    printf ("%8.5lf %8.5lf tp2\n", tp2.x, tp2.y);
    printf ("%8.5lf %8.5lf tp3\n", tp3.x, tp3.y);
    printf ("%8.5lf %8.5lf ctr\n", ctr.x, ctr.y);
    printf ("\n");
#endif

    double
      d1 = (tp1 - ctr).norm2(),
      d2 = (tp2 - ctr).norm2(),
      d3 = (tp3 - ctr).norm2(),
      d12 = d2 - d1,
      d13 = d3 - d1,
      dd = d12*d12 + d13*d13;
    printf ("%e err\n",  dd);
  }
  return 0;
}
