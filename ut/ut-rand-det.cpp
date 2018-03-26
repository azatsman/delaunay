#include "triang.hpp"
#include <cstdio>
#include <cstdlib>
#include <cmath>

int main (int argc, const char *argv[])
{
  srand48 (17);
  double sum = 0;
  int nn = 100000000;
  for (int k=0; k<nn; k++) {
    Point<double>
      tp1 (drand48(), drand48()),
      tp2 (drand48(), drand48()),
      tp3 (drand48(), drand48());
    double
      d = det (tp2-tp1, tp3-tp1);
    printf ("%8d %12.8lf\n", k, d);
    sum += fabs(d);
  }
  printf ("TitleText: Average=%f", sum / nn);
  return 0;
}
