#ifdef DEBUG_TRIANG
#undef DEBUG_TRIANG
#endif

#define DEBUG_TRIANG 1

#include "triang.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <fstream>

#define BUFLEN 1024
#define STREQ(A,B) (!strcmp(A,B))

typedef double Number;
struct TrTrack {
  std::vector <Point<Number> >  pointSet;
  std::set<Triangle>            triangleSet;
  int ctr;
  char nameBuf [222];

  TrTrack () : pointSet(), triangleSet(), ctr(0) {}

  void printTriangle (std::ostream& f, Triangle tr) {
    Point<Number>
      p0 = pointSet [tr.ix0],
      p1 = pointSet [tr.ix1],
      p2 = pointSet [tr.ix2];

    Number area = 0.5 * fabs (det (p1-p0,p2-p0));

    f << " #### area "  << area
      << " ix " << tr.ix0 << " " << tr.ix1 << " " << tr.ix2 << std::endl;
    f << "move " << p0.x << " " << p0.y << std::endl
      << "     " << p1.x << " " << p1.y << std::endl
      << "     " << p2.x << " " << p2.y << std::endl
      << "     " << p0.x << " " << p0.y << std::endl;
  }

  const char* nextFileName () {
    sprintf (nameBuf, "tr-%04d.xg", ctr);
    ctr++;
    return nameBuf;
  }

  void dumpOldTriangles (std::ostream& f) {
    for (std::set<Triangle>::const_iterator q=triangleSet.begin();
	 q != triangleSet.end(); q++) {
      printTriangle (f, *q);
    }
  }

  void pntAdd (Number x, Number y) {
    pointSet.push_back (Point<Number> (x,y));
  }

  void trAdd (int ix0, int ix1, int ix2) {
    std::ofstream xgf (nextFileName ());
    xgf << "TitleText: Add " << ix0 << " " << ix1 << " " << ix2 << std::endl;
    Triangle tr (ix0, ix1, ix2);
    dumpOldTriangles (xgf);
    xgf << std::endl;
    printTriangle (xgf, tr);
    addTriangle (ix0, ix1, ix2, triangleSet);
    // triangleSet.insert (tr);
  }

  void trRemove (int ix0, int ix1, int ix2) {
    std::ofstream xgf (nextFileName ());
    xgf << "TitleText: Remove " << ix0 << " " << ix1 << " " << ix2 << std::endl;
    Triangle tr (ix0, ix1, ix2);
    triangleSet.erase (tr);

    std::cout << dbgMarker << " triangle removed "
	      << std::setw(4) << tr.ix0 << " "
	      << std::setw(4) << tr.ix1 << " "
	      << std::setw(4) << tr.ix2 << std::endl;


    dumpOldTriangles (xgf);
    xgf << std::endl;
    printTriangle (xgf, tr);
  }
};

int main (int argc, const char *argv[])
{
  std::vector<Point<Number> > pntSet;
  char buf[BUFLEN+8];
  TrTrack trck;
  while (fgets (buf, BUFLEN, stdin)) {
    char* s = strtok (buf, " ");
    if (STREQ (dbgMarker.c_str(), s)) {
      s = strtok (NULL, " ");
      if (STREQ (s, "point")) {
	// char* pNum = strtok (NULL, " ");
	(void) strtok (NULL, " ");
	char* xp   = strtok (NULL, " ");
	char* yp   = strtok (NULL, " ");
	trck.pntAdd (atof (xp), atof(yp));
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
	  trck.trAdd    (ix0, ix1, ix2);
	else if (STREQ ("removed", ap))
	  trck.trRemove (ix0, ix1, ix2);
	else
	  throw std::runtime_error ("Unexpected token after triangle");
      }
      else
	throw std::runtime_error ("Unexpected token after marker");
    }
  }

  int numPoints = trck.pointSet.size();
  for (int k=0; k<numPoints; k++) {
    std::cout << "   Point " << k
	      << "  " << trck.pointSet[k].x
	      << "  " << trck.pointSet[k].y << std::endl;
  }
  

  return 0;
}
