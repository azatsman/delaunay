#include "triang.hpp"
#include <cstdio>
#include <cstdlib>

#define GET_STATS 1

static bool doOne (bool withPrint)
{
  Point<double>
    tp1 (drand48(), drand48()),
    tp2 (drand48(), drand48()),
    tp3 (drand48(), drand48()),
    p   (drand48(), drand48());
  bool isIn = isInside (p, tp1,tp2,tp3);
  if (withPrint) {
    const char* title = isIn ? "In" : "Out";
    printf ("TitleText: %s\n", title);
    printf ("move %.8f %.8f\n", p.x,   p.y);
    printf ("     %.8f %.8f\n", tp1.x, tp1.y);
    printf ("move %.8f %.8f\n", p.x,   p.y);
    printf ("     %.8f %.8f\n", tp2.x, tp2.y);
    printf ("move %.8f %.8f\n", p.x,   p.y);
    printf ("     %.8f %.8f\n", tp3.x, tp3.y);
  }
  return isIn;
}

int main (int argc, const char *argv[])
{
  srand48 (17);
#if GET_STATS
  int nn = 10000000;
  int
    inCtr = 0,
    outCtr = 0;
  for (int k=0; k<nn; k++) {
    if (doOne (false))
      inCtr++;
    else
      outCtr++;
  }
  printf ("%d,%d ~ %lf,%lf  ~  1:%lf\n",
	  inCtr, outCtr, ((double) inCtr ) / nn, ((double) outCtr) / nn,
	  ((double) outCtr) / ((double) inCtr));
#else
  doOne(true);
#endif
  return 0;
}
