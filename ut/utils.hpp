#ifndef INCLUDED_utils_hpp_28196287
#define INCLUDED_utils_hpp_28196287

#include "triang.hpp"
#include <fstream>


// Exception class containing info about non-confirming triangulation.
// Specifically, it contains the triangle and the index of the point
//  which lies inside the circle circumscribed around the triangle.
// This exception is thrown by 'delaunayCheck'.

template <typename T>
class DelaunayError : public std::runtime_error {
public:
  std::vector<Point<T> > pointSet;
  Triangle triangle;
  int pointNum;

  DelaunayError (const std::vector<Point<T> >& pSet, Triangle tr, int pNum) :
    std::runtime_error ("Delaunay Error"),
    pointSet(pSet),
    triangle(tr),
    pointNum(pNum)
  {}
  ~DelaunayError () throw() {};
};

// Check every triangles against every "adjacent point", i.e. the farther point from
// an adjacent triangle. If an adjacent point lies inside the circumscribed circle then
// DelaunayError exception is raised.

template <typename T>
void delaunayCheck (const std::vector<Point<T> >& pSet,
		    const std::set <Triangle>&    trSet)
{
  for (std::set<Triangle>::const_iterator q=trSet.begin(); q != trSet.end(); q++) {
    Triangle tr = *q;
    std::set <Triangle>::iterator ap;
    int adjPnt;
    Point<T> ccCtr = circumCtr<T> (pSet[tr.ix0], pSet[tr.ix1], pSet[tr.ix2]);
    T ccRad2 = (pSet[tr.ix0] - ccCtr).norm2();
    if ((ap=findAdjacent (trSet, tr.ix0, tr.ix1, tr.ix2,  &adjPnt)) != trSet.end()) {
      if ((pSet[adjPnt] - ccCtr).norm2() < ccRad2)
	throw  DelaunayError<T> (pSet, tr, adjPnt);
    }
    if ((ap=findAdjacent (trSet, tr.ix1, tr.ix2, tr.ix0,  &adjPnt)) != trSet.end()) {
      if ((pSet[adjPnt] - ccCtr).norm2() < ccRad2)
	throw  DelaunayError<T> (pSet, tr, adjPnt);
    }
    if ((ap=findAdjacent (trSet, tr.ix2, tr.ix0, tr.ix1,  &adjPnt)) != trSet.end()) {
      if ((pSet[adjPnt] - ccCtr).norm2() < ccRad2)
	throw  DelaunayError<T> (pSet, tr, adjPnt);
    }
  }
}

// Output info about Delaunay error in 'xgraph' format:

template <typename T>
void dumpDelaunayError (const DelaunayError<T>& de, const char* fileName)
{
  Point<T>
    tp0 = de.pointSet[de.triangle.ix0],
    tp1 = de.pointSet[de.triangle.ix1],
    tp2 = de.pointSet[de.triangle.ix2],
    pnt = de.pointSet[de.pointNum];
  std::ofstream ofl(fileName);
  ofl << "\"Triangle\"" << std::endl;
  ofl << tp0.x << " " << tp0.y << std::endl;
  ofl << tp1.x << " " << tp1.y << std::endl;
  ofl << tp2.x << " " << tp2.y << std::endl;
  ofl << tp0.x << " " << tp0.y << std::endl;
  ofl << std::endl;
  ofl << "\"" << "Point" << "\"" << std::endl;
  ofl << "move " << pnt.x << " " << pnt.y << std::endl;
  ofl << tp0.x << " " << tp0.y << std::endl;
  ofl << "move " << pnt.x << " " << pnt.y << std::endl;
  ofl << tp1.x << " " << tp1.y << std::endl;
  ofl << "move " << pnt.x << " " << pnt.y << std::endl;
  ofl << tp2.x << " " << tp2.y << std::endl;
}

// Dump the triangles in PostScript format (expects a set of points in [0,1]x[0,1] square):

template <typename T>
void dumpPS (const std::vector<Point<T> >& pntSet,
	     const std::set<Triangle>&     trSet,
	     const char* fileName)
{
  T
    pWidth  = 72 *  8.5,
    pHeight = 72 * 11.0,
    xyScale = 0.9 * pWidth,
    xShift = 0.5 * (pWidth  - xyScale),
    yShift = 0.5 * (pHeight - xyScale);

  std::ofstream ofl(fileName);
  ofl
    << "%!" << std::endl
    << xShift << " " << yShift << " translate" << std::endl
    << xyScale << " dup scale " << std::endl
    << " 0.001 setlinewidth " << std::endl
    << " 1  setlinejoin " << std::endl;
  for (std::set<Triangle>::const_iterator q=trSet.begin(); q != trSet.end(); q++) {
    Point<T>
      tp0 = pntSet[q->ix0],
      tp1 = pntSet[q->ix1],
      tp2 = pntSet[q->ix2];
    ofl << tp0.x << " " << tp0.y << " moveto" << std::endl;
    ofl << tp1.x << " " << tp1.y << " lineto" << std::endl;
    ofl << tp2.x << " " << tp2.y << " lineto" << std::endl;
    ofl << tp0.x << " " << tp0.y << " lineto" << std::endl;
  }
  ofl
    << "stroke"   << std::endl
    << "showpage" << std::endl;
}

#endif /*INCLUDED_utils_hpp_28196287*/