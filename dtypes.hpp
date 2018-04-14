#ifndef INCLUDED_dtypes_hpp_69036498
#define INCLUDED_dtypes_hpp_69036498

// Simple geometric structures for points and triangles, where the triangles
// are specified by point indices.

#include <algorithm>
#include <vector>

template <typename T> struct Point {  // 2D Point or vector
  T x;
  T y;
  Point<T> () {};
  Point<T> (T xx, T yy) : x(xx), y(yy) {};
  Point<T> operator+(Point<T> p) const {return Point<T> (x+p.x, y+p.y);};
  Point<T> operator-(Point<T> p) const {return Point<T> (x-p.x, y-p.y);};
  T dot   (Point<T> p) const {return x*p.x + y*p.y;};
  T norm2 ()           const {return (x*x+y*y);};
};


// Integer points often cause overflows, converting arrays of points from
// int to doubles usuall solves the problem:

template <typename T>
static void pointVectorToDouble (const std::vector<Point<T> >& pSet,
				 std::vector<Point<double> >& pSetDouble)
{
  int pLen = pSet.size();
  pSetDouble.resize (pLen);
  for (int k=0; k<pLen; k++)
    pSetDouble[k] = Point<double> (static_cast<double> (pSet[k].x),
				   static_cast<double> (pSet[k].y));
}


// Triangle contains indices of the points. It is only useful in the context
// with an array or a vector of points.

struct Triangle {
  int ix0;
  int ix1;
  int ix2;

  // We keep indices sorted:

  Triangle (int i0, int i1, int i2) {
    if (i0 > i1)
      std::swap (i0, i1);
    if (i1 > i2)
      std::swap (i1, i2);
    if (i0 > i1)
      std::swap (i0, i1);
    ix0 = i0;
    ix1 = i1;
    ix2 = i2;
  }

  // Operator '<' is required for operations on sets of triangles. The operator relies on
  // indices being in ascending order:

  bool operator< (Triangle that) const {
    if (ix0 < that.ix0)
      return true;
    if (ix0 > that.ix0)
      return false;
    if (ix1 < that.ix1)
      return true;
    if (ix1 > that.ix1)
      return false;
    if (ix2 < that.ix2)
      return true;
    else
      return false;
  };
};

// Return the determinant of the matrix formed by 'a' and 'b'

template <typename T> T det (Point<T> a, Point<T> b)
{
  return (a.x*b.y - a.y*b.x);
}

template <typename T>  bool clockwise (Point<T> a, Point<T> b)
{
  return det (a,b) < 0;
}

#endif /*INCLUDED_dtypes_hpp_69036498*/
