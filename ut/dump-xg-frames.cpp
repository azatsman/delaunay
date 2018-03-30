
#define DEBUG_TRIANG 1
#include "triang.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

#define BUFLEN 1024
#define STREQ(A,B) (!strcmp(A,B))

typedef double Number;


static void trAdd (int ix0, int ix1, int ix2,
		   std::vector <Point<Number> > pntSet,
		   int ctr)
{
  // TBD
}


static void trRemove (int ix0, int ix1, int ix2,
		      std::vector <Point<Number> > pntSet,
		      int ctr)
{
  // TBD
}

int main (int argc, const char *argv[])
{
  std::vector<Point<Number> > pntSet;
  char buf[BUFLEN+8];
  int ctr = 0;
  while (fgets (buf, BUFLEN, stdin)) {
    char* s = strtok (buf, " ");
    if (STREQ (dbgMarker.c_str(), s)) {
      s = strtok (NULL, " ");
      if (STREQ (s, "point")) {
	printf ("-- %s --\n", s);
	char* xp = strtok (NULL, " ");
	char* yp = strtok (NULL, " ");
	pntSet.push_back (Point<Number> (atof (xp), atof(yp)));
      }
      else if (STREQ("triangle", s)) {
	char* ap   = strtok (NULL, " ");
	char* ixp0 = strtok (NULL, " ");
	char* ixp1 = strtok (NULL, " ");
	char* ixp2 = strtok (NULL, " ");
	int ix0 = atoi(ixp0);
	int ix1 = atoi(ixp1);
	int ix2 = atoi(ixp2);
	if      (STREQ ("added",   ap))
	  trAdd    (ix0, ix1, ix2, pntSet, ctr++);
	else if (STREQ ("removed", ap))
	  trRemove (ix0, ix1, ix2, pntSet, ctr++);
	else
	  throw std::runtime_error ("Unexpected token after triangle");
      }
      else
	throw std::runtime_error ("Unexpected token after marker");
    }
  }
  return 0;
}
