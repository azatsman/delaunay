#ifndef INCLUDED_utils_hpp_28196287
#define INCLUDED_utils_hpp_28196287

#include "triang.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>

#include <cstdlib>

#define SRAND(N) srand48(N)
#define DRAND()  drand48()

// Exception class containing info about non-confirming triangulation.
// Specifically, it contains the triangle and the index of the point
//  which lies inside the circle circumscribed around the triangle.
// This exception is thrown by 'delaunayCheck'.

template <typename T>
class DelaunayError {
public:
  std::vector<Point<T> > pointSet;
  Triangle triangle;
  int pointNum;
  DelaunayError (const std::vector<Point<T> >& pSet, Triangle tr, int pNum) :
    pointSet(pSet),
    triangle(tr),
    pointNum(pNum)
  {}
  //  ~DelaunayError () throw() {};
};

// Check every triangles against every "adjacent point", i.e. the farther point from
// an adjacent triangle. If an adjacent point lies inside the circumscribed circle then
// DelaunayError exception is raised.


#define CHECK_FLIPPED 0

template <typename T> void checkOneSide (const std::vector<Point<T> >& pSet,
					 const std::set <Triangle>&    trSet,
					 int ix0, int ix1, int ix2,
					 T absEpsilon2)
{
  int adjPnt;
  std::set <Triangle>::iterator ap;
  if ((ap=findAdjacent (trSet, ix0, ix1, ix2,  &adjPnt)) != trSet.end()) {
    if ((circumCirclePosition (pSet[adjPnt], pSet[ix0], pSet[ix1], pSet[ix2]) > absEpsilon2)
	
#if CHECK_FLIPPED
	&& ! isInCircumCircle (pSet[ix0], pSet[adjPnt], pSet[ix1], pSet[ix2])
#endif
	)
      {
	if (isInCircumCircle (pSet[ix2], pSet[ix0], pSet[ix1], pSet[adjPnt])) {
	  std::cerr << "Symmetric error" << std::endl;
	  throw  DelaunayError<T> (pSet, Triangle (ix0, ix1, ix2), adjPnt);
	}
	else
	  std::cerr << "WARNING: Asymmetric error !!! " << std::endl;
      }
  }
}


template <typename T>
void delaunayCheck (const std::vector<Point<T> >& pSet,
		    const std::set <Triangle>&    trSet,
		    T relEpsilon2 = static_cast<T> (1e-10))
{
  for (std::set<Triangle>::const_iterator q=trSet.begin(); q != trSet.end(); q++) {
    Triangle tr = *q;

    Point<T>
      d01 = pSet[tr.ix1] - pSet[tr.ix0],
      d12 = pSet[tr.ix2] - pSet[tr.ix1],
      d20 = pSet[tr.ix0] - pSet[tr.ix2];
    T
      roughSize2 = d01.norm2() + d12.norm2() + d20.norm2(),
      absEpsilon2 = relEpsilon2 * roughSize2;
    checkOneSide (pSet, trSet, tr.ix0, tr.ix1, tr.ix2, absEpsilon2);
    checkOneSide (pSet, trSet, tr.ix1, tr.ix2, tr.ix0, absEpsilon2);
    checkOneSide (pSet, trSet, tr.ix2, tr.ix0, tr.ix1, absEpsilon2);
  }
}

template <>
void delaunayCheck<int> (const std::vector<Point<int> >& pSet,
			 const std::set <Triangle>&    trSet,
			 int relEpsilon2)
{
  std::vector<Point<double> > pSet2;
  pointVectorToDouble (pSet, pSet2);
  delaunayCheck<double> (pSet2, trSet, relEpsilon2);
}

struct Edge {
  int ix0;
  int ix1;
  Edge (int a, int b) : ix0(a), ix1(b) {
    if (ix0 > ix1)
      std::swap (ix0, ix1);
  }

  bool operator< (const Edge& e) const {
    if (ix0 < e.ix0)
      return true;
    else if (ix0 > e.ix0)
      return false;
    else
      return (ix1 < e.ix1);
  }

  bool adjoins (const Edge& e) const {
    return
      (ix0 == e.ix0) ||
      (ix0 == e.ix1) ||
      (ix1 == e.ix0) ||
      (ix1 == e.ix1);
  }

  template <typename T>
  bool crosses (const Edge& e, std::vector<Point<T> > pSet) const {
    Point<T>
      a (pSet[ix0]),
      b (pSet[ix1]),
      c (pSet[e.ix0]),
      d (pSet[e.ix1]),
      p (c-a),
      z (0,0),
      c0 (0, 0),
      c1 (b-a),
      c3 (c-d),
      c2 (c1+c3);
    T
      d01 = det (c0-p,c1-p),
      d12 = det (c1-p,c2-p),
      d23 = det (c2-p,c3-p),
      d30 = det (c3-p,c0-p);
    //   std::cout << d01 << " " << d12 << " " << d23 << " " << d30 << std::endl;

    // Result is true if all determinants have the same sign:

    bool rslt = (((d01 > 0) && (d12 > 0) && (d23 > 0) && (d30 > 0)) ||
		 ((d01 < 0) && (d12 < 0) && (d23 < 0) && (d30 < 0)));
    return rslt;
  };
};


template <typename T>
class EdgeCrossError {
public:
  std::vector<Point<T> > pointSet;
  Edge e1;
  Edge e2;
  
  EdgeCrossError (const std::vector<Point<T> >& pSet, Edge e1, Edge e2) :
    // std::runtime_error ("Edge Crossing Error"),
    pointSet(pSet),
    e1 (e1),
    e2 (e2)
  {}
  //  ~EdgeCrossError () throw() {};
};

template <typename T>
void edgeCrossingCheck (const std::vector<Point<T> >& pSet,
			const std::set <Triangle>&    trSet)
{
  //  std::vector<Edge> edgeArray;

  std::set   <Edge> edgeSet;
  for (std::set<Triangle>::const_iterator tp = trSet.begin(); tp != trSet.end(); tp++) {
    edgeSet.insert (Edge (tp->ix0, tp->ix1));
    edgeSet.insert (Edge (tp->ix0, tp->ix2));
    edgeSet.insert (Edge (tp->ix1, tp->ix2));
  }
  for   (std::set<Edge>::const_iterator p1 = edgeSet.begin(); p1 != edgeSet.end(); p1++) {
    std::set<Edge>::const_iterator p2 = p1;
    p2++;
    for (; p2 != edgeSet.end(); p2++) {
      if ((! p1->adjoins(*p2)) && (p1->crosses (*p2, pSet)))
	throw EdgeCrossError<T> (pSet, *p1, *p2);
    }
  }
  //    for (std::set<Edge>::const_iterator ep = edgeSet.begin; ep != edgeSet.end(); ep++)
  //      edgeArray.push_back (*ep);
}

// Convert integers to doubles because integers tend to overflow:

template <>
void edgeCrossingCheck<int> (const std::vector<Point<int> >& pSet,
			     const std::set <Triangle>&    trSet)
{
  std::vector<Point<double> > pSet2;
  pointVectorToDouble (pSet, pSet2);
  edgeCrossingCheck<double> (pSet2, trSet);
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

// Output info about edge crossing error in 'xgraph' format:

template <typename T>
void dumpEdgeCrossError (const EdgeCrossError<T>& ee, const char* fileName)
{
  Point<T>
    a0 = ee.pointSet[ee.e1.ix0],
    a1 = ee.pointSet[ee.e1.ix1],
    b0 = ee.pointSet[ee.e2.ix0],
    b1 = ee.pointSet[ee.e2.ix1];
  std::ofstream ofl(fileName);
  ofl
    << "TitleText: Crossing Edges" << std::endl
    << "\"Points  " << ee.e1.ix0 << ", " << ee.e1.ix1 << std::endl
    << a0.x << " " << a0.y << std::endl
    << a1.x << " " << a1.y << std::endl
    << std::endl
    << "\"Points  " << ee.e2.ix0 << ", " << ee.e2.ix1 << std::endl
    << b0.x << " " << b0.y << std::endl
    << b1.x << " " << b1.y << std::endl;
}

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

    Point<T>
      p0 = pntSet[q->ix0],
      p1 = pntSet[q->ix1],
      p2 = pntSet[q->ix2];
    T area = 0.5 * det (p1-p0, p2-p1);
    xgf
      << "move " << p0.x << " " <<  p0.y << " ## area " << area << std::endl
      << "     " << p1.x << " " <<  p1.y << std::endl
      << "     " << p2.x << " " <<  p2.y << std::endl
      << "     " << p0.x << " " <<  p0.y << std::endl;
  }
}

#endif /*INCLUDED_utils_hpp_28196287*/
