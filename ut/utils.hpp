#ifndef INCLUDED_utils_hpp_28196287
#define INCLUDED_utils_hpp_28196287

#include "triang.hpp"
#include <fstream>

#include <cstdlib>

#define SRAND(N) srand48(N)
#define DRAND()  drand48()

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


#define CHECK_FLIPPED 1

template <typename T>
void delaunayCheck (const std::vector<Point<T> >& pSet,
		    const std::set <Triangle>&    trSet)
{
  for (std::set<Triangle>::const_iterator q=trSet.begin(); q != trSet.end(); q++) {
    Triangle tr = *q;
    std::set <Triangle>::iterator ap;
    int adjPnt;

    if ((ap=findAdjacent (trSet, tr.ix0, tr.ix1, tr.ix2,  &adjPnt)) != trSet.end()) {
      if (isInCircumCircle (pSet[adjPnt], pSet[tr.ix0], pSet[tr.ix1], pSet[tr.ix2])
#if CHECK_FLIPPED
	  && ! isInCircumCircle (pSet[tr.ix0], pSet[adjPnt], pSet[tr.ix1], pSet[tr.ix2])
#endif
	    )
	throw  DelaunayError<T> (pSet, tr, adjPnt);
    }
    if ((ap=findAdjacent (trSet, tr.ix1, tr.ix2, tr.ix0,  &adjPnt)) != trSet.end()) {
      if (isInCircumCircle (pSet[adjPnt], pSet[tr.ix0], pSet[tr.ix1], pSet[tr.ix2])
#if CHECK_FLIPPED
	  && ! isInCircumCircle (pSet[tr.ix1], pSet[adjPnt], pSet[tr.ix0], pSet[tr.ix2])
#endif
	  )
	throw  DelaunayError<T> (pSet, tr, adjPnt);
    }
    if ((ap=findAdjacent (trSet, tr.ix2, tr.ix0, tr.ix1,  &adjPnt)) != trSet.end()) {
      if (isInCircumCircle (pSet[adjPnt], pSet[tr.ix0], pSet[tr.ix1], pSet[tr.ix2])
#if CHECK_FLIPPED
	  && ! isInCircumCircle (pSet[tr.ix0], pSet[adjPnt], pSet[tr.ix1], pSet[tr.ix2])
#endif
	  )
	throw  DelaunayError<T> (pSet, tr, adjPnt);
    }
  }
}

template<typename T>
void dumpPair (std::ostream& s, T x, T y)
{
  int ww = 40, prec = 35;
  s
    << std::fixed << std::setw(ww) << std::setprecision (prec)
    << x << " "
    << std::fixed << std::setw(ww) << std::setprecision (prec)
    << y;
}

// Output info about Delaunay error in 'xgraph' format:

template <typename T>
void dumpDelaunayError (const DelaunayError<T>& de, const char* fileName)
{
  int
    tix0 = de.triangle.ix0,
    tix1 = de.triangle.ix1,
    tix2 = de.triangle.ix2;
  Point<T>
    tp0 = de.pointSet[tix0],
    tp1 = de.pointSet[tix1],
    tp2 = de.pointSet[tix2],
    pnt = de.pointSet[de.pointNum];
  std::ofstream ofl(fileName);
  ofl << "\"Triangle\"" << std::endl;

  int ww = 40, prec = 35;

  //  dumpPair (ofl, tp0.x, tp0.y);
  ofl
    << std::fixed << std::setw(ww) << std::setprecision (prec)
    << tp0.x << " "
    << std::fixed << std::setw(ww) << std::setprecision (prec)
    << tp0.y << " ## " << tix0 << std::endl
    //  ofl << " ## " << tix0 << std::endl;
    << std::fixed << std::setw(ww) << std::setprecision (prec)
    << tp1.x << " "
    << std::fixed << std::setw(ww) << std::setprecision (prec)
    << tp1.y << " ## " << tix1 << std::endl
    << std::fixed << std::setw(ww) << std::setprecision (prec)
    << tp2.x << " "
    << std::fixed << std::setw(ww) << std::setprecision (prec)
    << tp2.y << " ## " << tix2 << std::endl
    << std::fixed << std::setw(ww) << std::setprecision (prec)
    << tp0.x << " "
    << std::fixed << std::setw(ww) << std::setprecision (prec)
    << tp0.y << " ## " << tix0 << std::endl
    << std::endl

    << "\"" << "Point" << "\"" << std::endl

    << "move "
    << std::fixed << std::setw(ww) << std::setprecision (prec)
    << pnt.x << " "
    << std::fixed << std::setw(ww) << std::setprecision (prec)
    << pnt.y << " ## pnt " << de.pointNum << std::endl
    << "     "
    << std::fixed << std::setw(ww) << std::setprecision (prec)
    << tp0.x << " "
    << std::fixed << std::setw(ww) << std::setprecision (prec)
    << tp0.y << std::endl
    << "move "
    << std::fixed << std::setw(ww) << std::setprecision (prec)
    << pnt.x << " "
    << std::fixed << std::setw(ww) << std::setprecision (prec)
    << pnt.y << std::endl
    << "     "
    << std::fixed << std::setw(ww) << std::setprecision (prec)
    << tp1.x << " "
    << std::fixed << std::setw(ww) << std::setprecision (prec)
    << tp1.y << std::endl
    << "move " 
    << std::fixed << std::setw(ww) << std::setprecision (prec)
    << pnt.x << " "
    << std::fixed << std::setw(ww) << std::setprecision (prec)
    << pnt.y << std::endl
    << "     "
    << std::fixed << std::setw(ww) << std::setprecision (prec)
    << tp2.x << " "
    << std::fixed << std::setw(ww) << std::setprecision (prec)
    << tp2.y << std::endl;
}

// Dump the triangles in PostScript format (expects a set of points in [0,1]x[0,1] square):

template <typename T>
void dumpPS (const std::vector<Point<T> >& pntSet,
	     const std::set<Triangle>&     trSet,
	     const char* fileName,
	     T xMax = 1,
	     T yMax = 1)
{
  T
    pWidth  = 72 *  8.5,
    pHeight = 72 * 11.0,
    xScale = 0.9 * pWidth / xMax,
    yScale = 0.9 * pHeight / yMax,
    xyScale = std::min (xScale, yScale),
    xShift = 0.5 * (pWidth  - xyScale * xMax),
    yShift = 0.5 * (pHeight - xyScale * yMax);
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

template <typename T>
void dumpXG (const std::vector<Point<T> >& pntSet,
	     const std::set<Triangle>&     trSet,
	     const char* fileName)
{
  std::ofstream xgf (fileName);
  xgf << "TitleText: " << pntSet.size() << " points "
      << trSet.size() << " triangles" << std::endl;
  for (std::set<Triangle>::const_iterator q = trSet.begin(); q != trSet.end(); q++) {
    xgf
      << "move " << pntSet[q->ix0].x << " " <<  pntSet[q->ix0].y << std::endl
      << "     " << pntSet[q->ix1].x << " " <<  pntSet[q->ix1].y << std::endl
      << "     " << pntSet[q->ix2].x << " " <<  pntSet[q->ix2].y << std::endl
      << "     " << pntSet[q->ix0].x << " " <<  pntSet[q->ix0].y << std::endl;
  }
}



#if 0
template<T> struct TriangleTracker {
  std::vector<Point<T> > pointSet;
  const std::string marker ("======");
  void setPointSet (std::vector<P> pSet) {pointSet = pSet;};

  markAddition (int ix0, int ix1, int ix2) {
    std::cout << " ========= Added   Triangle "
	      << a.ix0 << " " << a.ix1 << " " << a.ix2 << std::endl;
  };
  markRemoval (int ix0, int ix1, int ix2) {
  };
};
#endif



#endif /*INCLUDED_utils_hpp_28196287*/
