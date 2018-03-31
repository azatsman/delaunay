#ifndef INCLUDED_triang_hpp_85662733
#define INCLUDED_triang_hpp_85662733

#include <vector>
#include <set>
#include <stdexcept>

#ifndef DEBUG_TRIANG
#define  DEBUG_TRIANG 0
#endif

#if DEBUG_TRIANG
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <string>
static std::string dbgMarker ("========");
#endif

template <typename T> struct Point {  // 2D Point or vector
  T x;
  T y;
  Point<T> (T xx, T yy) : x(xx), y(yy) {};
  Point<T> operator+(Point<T> p) const {return Point<T> (x+p.x, y+p.y);};
  Point<T> operator-(Point<T> p) const {return Point<T> (x-p.x, y-p.y);};
  T dot   (Point<T> p) const {return x*p.x + y*p.y;}
  T norm2 ()           const {return (x*x+y*y);};
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



// Return true if point 'p' is inside the triangle formed by points 't0','t1' and 't2':

template <typename T> bool isInside (Point<T> p, Point<T> t0,Point<T> t1,Point<T> t2)
{
  T
    d01 = det (t0-p, t1-p),
    d12 = det (t1-p, t2-p),
    d20 = det (t2-p, t0-p);
  return (d01*d12 > 0) && (d12*d20 > 0) && (d20*d01 > 0);
}

// Return the center of the circle containing points 'a', 'b' and 'c':
// Note: no checking is made to make sure that the points do not lie on the same line.

template <typename T> Point<T> circumCtr (Point<T> a, Point<T> b, Point<T> c)
{
  T
    D = 2*(a.x*(b.y-c.y) + b.x*(c.y-a.y) + c.x*(a.y-b.y)),
    Dinv = 1.0 / D,
    a2 = Dinv * a.norm2(),
    b2 = Dinv * b.norm2(),
    c2 = Dinv * c.norm2();
  Point<T> rslt (a2 * (b.y - c.y) + b2 * (c.y - a.y) + c2 * (a.y - b.y),
                 a2 * (c.x - b.x) + b2 * (a.x - c.x) + c2 * (b.x - a.x));
  return rslt;
}

// Return true if 'p' is inside the cirlce circumscribed around the triangle:
// We return false if the point is exactly on the circle.

#if 1
template <typename T>
bool isInCircumCircle (Point<T> D, Point<T> A, Point<T> B, Point<T> C)
{
  T
    m11 = A.x - D.x,
    m12 = A.y - D.y,
    m13 = m11*m11 + m12*m12,

    m21 = B.x - D.x,
    m22 = B.y - D.y,
    m23 = m21*m21 + m22*m22,

    m31 = C.x - D.x,
    m32 = C.y - D.y,
    m33 = m31*m31 + m32*m32,
    dt1 =
    m11 * (m22 * m33 - m23 * m32) -
    m12 * (m21 * m33 - m23 * m31) +
    m13 * (m21 * m32 - m22 * m31),
    dt2 =
    (B.x -A.x) * (C.y-A.y) - (B.y-A.y)*(C.x-A.x);
  return dt1*dt2 > 0;
}



#else

template <typename T>
bool isInCircumCircle (Point<T> p, Point<T> tp0, Point<T> tp1, Point<T> tp2)
{
  Point<T>
    ctr   = circumCtr (tp0, tp1, tp2),
    dist0 = tp0 - ctr,
    distP = p   - ctr;
  T
    r2     = dist0.norm2(),
    pDist2 = distP.norm2();
  return pDist2 < r2;
}
#endif

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

  // Return true if the triangle contains points with indices 'k1' and 'k2':

  bool contains (int k1, int k2) const {
    if (k1 > k2)
      std::swap (k1, k2);
    return
      ((k1 == ix0) && (k2 == ix1)) ||
      ((k1 == ix0) && (k2 == ix2)) ||
      ((k1 == ix1) && (k2 == ix2));
  };

  // Return true if the triangle contains ix1 and ix2, but does NOT contain ox3:

  bool contains2 (int in1, int in2, int out3) const {
    if (in1 > in2)
      std::swap (in1, in2);
    return ((in1 == ix0) && (in2 == ix1) && (out3 != ix2)) ||
                 ((in1 == ix0) && (in2 == ix2) && (out3 != ix1)) ||
                 ((in1 == ix1) && (in2 == ix2) && (out3 != ix0));
  }

  // Does not seem to be used by the std::set operations, so we probably don't need it:

  bool operator== (Triangle that) const {
    return ((ix0 == that.ix0) &&
            (ix1 == that.ix1) &&
            (ix2 == that.ix2));
  };
};

// Find a triangle in trSet which contains ix0 and ix1, but not ox2.
// If found iterator pointing to the triangle is returned and the third index is written to *in3p.
// If not found the return iterator points to trSet.end();

std::set <Triangle>::iterator findAdjacent (const std::set<Triangle>&  trSet,
                                            int in1, int in2, int out3,
                                            int* in3p)
{
  std::set <Triangle>::iterator q=trSet.begin();
  if (in1 > in2)
    std::swap (in1, in2);
  for (; q != trSet.end(); q++) {
    int
      ix0 = q->ix0,
      ix1 = q->ix1,
      ix2 = q->ix2;
    if ((in1 == ix0) && (in2 == ix1) && (out3 != ix2)) {
      *in3p = ix2;
      return q;
    }
    if ((in1 == ix0) && (in2 == ix2) && (out3 != ix1)) {
      *in3p = ix1;
      return q;
    }
    if ((in1 == ix1) && (in2 == ix2) && (out3 != ix0)) {
      *in3p = ix0;
      return q;
    }
  }
  return trSet.end();
}


void removeTriangle (std::set<Triangle>& trSet,
		     std::set<Triangle>::iterator tp)
{
#if DEBUG_TRIANG
  std::cout << dbgMarker << " triangle removed "
	    << std::setw(4) << tp->ix0 << " "
	    << std::setw(4) << tp->ix1 << " "
	    << std::setw(4) << tp->ix2 << std::endl;
#endif
  trSet.erase (tp);
}

void addTriangle (int vertIx0, int vertIx1, int vertIx2,
		  std::set<Triangle>& trSet)
{
  Triangle a  (vertIx0, vertIx1, vertIx2);
  trSet.insert (a);
#if DEBUG_TRIANG
  std::cout << dbgMarker << " triangle added "
	    << std::setw(4) << vertIx0 << " "
	    << std::setw(4) << vertIx1 << " "
	    << std::setw(4) << vertIx2 << std::endl;
#endif
}

// Add new triangle with a new point newPnt and old points oldPnt1, oldPnt2.
// If oldPnt1,oldPnt2 belong to an old triangle, and newPnt is inside the circle
// circumscribed around the triangle than "flip" the new and old triangles.

template <typename T>
void addAndFlip (int newPnt, int oldPnt1, int oldPnt2,
                 const std::vector<Point<T> >& pntVector,
                 std::set <Triangle>&  trSet)
{
  int oldPnt3;
  std::set <Triangle>::iterator tp = findAdjacent (trSet, oldPnt1, oldPnt2, -1, &oldPnt3);
  if ((tp != trSet.end()) && (isInCircumCircle (pntVector[newPnt],
						pntVector[oldPnt1],
						pntVector[oldPnt2],
						pntVector[oldPnt3]))) {
    removeTriangle (trSet, tp);
    addAndFlip (newPnt, oldPnt1, oldPnt3, pntVector, trSet);
    addAndFlip (newPnt, oldPnt2, oldPnt3, pntVector, trSet);
  }
  else
    addTriangle (newPnt, oldPnt1, oldPnt2, trSet);
}

// Return true if the three points lie on a single line with relative
// precision of epsilon.  Note that if T is an integer type then
// 'epsilon' is usually 0.

template <typename T>
bool isInLine (Point<T> p0, Point<T> p1, Point<T> p2, T epsilon2)
{
  Point<T>
    side1 = p1 - p0,
    side2 = p2 - p0;
  T sideNorms  = side1.norm2() + side2.norm2();
  T area = det (side1, side2);
  bool rslt = area*area < epsilon2 * sideNorms * sideNorms;
  return rslt;
}

// Position of a point relative to a triangle:

enum PointVsTriangle {Inside,           // Inside, away from the sides.
		      OnASideInternal,          // On a side of the triangle, but not in a corner
		      OnASideExternal,  // On a side without adjacent triangle.
		      Duplicate,        // Too close to one of the corners.
		      Outside,          // Completely outside of this triangle.
		      NoTriangles,      // No triangles found for the point
		      Unknown};         // Usually happens because of an error.

// Try inserting new point in the triangle.
// Several cases depending on the returned value:
// == Inside: the point is inside the triangle and not "too close" to any of the sides 
//      -- 3 new triangles are created
//      -- Adjacent triangles, if any, are checked for flipping.
// == OnASideInternal:
//      -- 4 new triangles are created, two in the current triangle and two in the adjacent one;
//      -- 2 pairs of triangles are checked for flipping.
//      -- addVertex1 and adjVertex2 are filled with vertex indices of the side on which
//           new point is lying.
//      -- oppositeVertex is filled with the index of the remaining vertex.
//      -- adjacent triangle aexists than check for flippint.
// == OnASideExternal:
//      -- 2 new triangles are created.
//      -- addVertex1 and adjVertex2 are filled with vertex indices of the side on which
//           new point is lying;
//      -- oppositeVertex is left untouched;
// -- Duplicate: the point is too close to one of the vertices, in which case nothing
//       is changed, and, in the end, the point is not included in any of the triangles.
// -- Outside:   the point lines completely outside (not too closed to) the triangle, so
//       nothing is changed.

template <typename T>
PointVsTriangle tryTriangle (int pntNum,
			     std::set<Triangle>::iterator tp,
			     const std::vector<Point<T> > pntVector,
			     T epsilon2, // relative precision.
			     int* adjVertex1,
			     int* adjVertex2,
			     int* oppositeVertex,
			     std::set<Triangle>& trSet)
{
  Triangle trngl = *tp;
  int
    v0 = trngl.ix0,
    v1 = trngl.ix1,
    v2 = trngl.ix2;
  Point<T>
    p = pntVector[pntNum],
    t0 = pntVector[v0],
    t1 = pntVector[v1],
    t2 = pntVector[v2];
  T
    roughSize2 = (t1-t0).norm2() + (t2-t0).norm2(),
    absEpsilon2 = epsilon2 * roughSize2,  // absolute instead of relative
    d01 = det (t0-p, t1-p),
    d12 = det (t1-p, t2-p),
    d20 = det (t2-p, t0-p),
    d012 = d01 * d12,
    d120 = d12 * d20,
    d201 = d20 * d01;

  // First check if the point is outside the triangle,
  // which is to say that at least one pair of determinants have different signs:

  if (d012 < 0 || (d120 < 0) || (d201 < 0))
    return Outside;

  // Now check if the point is inside the triangle and not near or on any of the sides,
  // which means that none of the determinants is too small:

  if ((d012 > absEpsilon2) &&
      (d120 > absEpsilon2) &&
      (d201 > absEpsilon2)) {
    removeTriangle (trSet, tp);
    addAndFlip (pntNum, v0, v1, pntVector, trSet);
    addAndFlip (pntNum, v1, v2, pntVector, trSet);
    addAndFlip (pntNum, v2, v0, pntVector, trSet);
    return Inside;
  }

  // At this stage we know that the point is close to one of the sides of the triangle,
  // but we have to figure out which side. That side is determined by finding
  // the largest product of determinants.

  int vOpp;
  T dMax;
  if (d012 > d120) {
    v0   = trngl.ix0;
    v1   = trngl.ix2;
    vOpp = trngl.ix1;
    dMax = d012;
  }
  else {
    v0   = trngl.ix1;
    v1   = trngl.ix0;
    vOpp = trngl.ix2;
    dMax = d120;
  }
  if (d201 > dMax) {
    v0   = trngl.ix1;
    v1   = trngl.ix2;
    vOpp = trngl.ix0;
    dMax = d201;
  }

  // Now we know that the point is close to the side with indices 'v0' and 'v1',
  // and the opposite triangle vertex has index 'vOpp'.
  // If  the point is close to either 'v0' or 'v1' then do nothing:

  if (((p-pntVector[v0]).norm2() < absEpsilon2) || ((p-pntVector[v1]).norm2() < absEpsilon2))
    return Duplicate;

  // At this stage the point is very close to the [g0,g1] line, but not close to either end:

  *adjVertex1     = v0;
  *adjVertex2     = v1;
  *oppositeVertex = vOpp;

  int nbVert;

  // Check whether there is a triangle adjacent to the [v0,v1] side:

  std::set<Triangle>::iterator adjTr = findAdjacent (trSet, v0, v1, vOpp, &nbVert);

  removeTriangle (trSet, tp);
  addAndFlip (pntNum, v0, vOpp, pntVector, trSet);
  addAndFlip (pntNum, v1, vOpp, pntVector, trSet);
  PointVsTriangle rslt = OnASideExternal;
  
  if (adjTr != trSet.end())  {
    // If an adjacent triangle exists then do the same for the adjacent triangle as for
    // the current one; i.e., erase it and add two new triangles.
    removeTriangle (trSet, adjTr);
    addAndFlip (pntNum, v0, nbVert, pntVector, trSet);
    addAndFlip (pntNum, v1, nbVert, pntVector, trSet);
    rslt = OnASideInternal;
  }
  return rslt;
}


template <typename T>
bool isObtuse (Point<T> p1, Point<T> p2, Point<T> p3, T absEpsilon2)
{
  return (p2-p1).dot(p3-p2) > absEpsilon2;
}

// The line is defined as a vector of indices from 'pSet'.
// The new point with index pntNew is supposed to be on the same straight
// line as points from 'line'.
// The function adds 'pntNew' to 'line' so that the order of points is preserved.
// It might end up not adding the point if it is too close to one of the points in the line.

template <typename T>
void expandStraightLine (int pntNew,
			 std::vector<int>& line,
			 const std::vector<Point<T> > & pSet,
			 T epsilon2)
{
  int
    lineLen = line.size(),
    pnt0 = line[0],
    pnt1 = line[1],
    pnt49 = line[lineLen-2],
    pnt50 = line[lineLen-1];
  T
    avgLen2 = (pSet[pnt50]-pSet[pnt0]).norm2() / ((lineLen-1)*(lineLen-1)),
    absEpsilon2 = avgLen2 * epsilon2;

  if (isObtuse (pSet[pntNew], pSet[pnt0], pSet[pnt1], absEpsilon2))
    line.insert (line.begin(), pntNew);
  else if  (isObtuse (pSet[pnt49], pSet[pnt50], pSet[pntNew], absEpsilon2))
    line.push_back (pntNew);
  else {
    for (std::vector<int>::iterator q=line.begin();  q+1 != line.end(); q++) {
      if (isObtuse (pSet[*q], pSet[pntNew], pSet[*(q+1)], absEpsilon2)) {
	line.insert(q+1, pntNew);
	break;
      }
    }
  }
}

// Given a set of points 'pSet' compute triangulation with Delaunay property,
// i.e., for each point P in 'pSet' and each triangle R in the computed 'trSet'
// either P is a vertex of R, or P does not inside the circle circumscribed around R.
// Each Triangle object in 'trSet' contains indices of 3 points from pSet.
// epsilon is  the relative (to distances between points) precision for
// determining whether one point is too close to another (in which case it is ignored)
// or whether point A is too close to a line connecting points B and C, in which
// case  triangle A,B,C  is not created.

template <typename T>
void delaunay (const std::vector<Point<T> >& pSet,
               std::set <Triangle>&          trSet,
	       T epsilon = static_cast<T> (1e-8))
{
  T epsilon2 = epsilon * epsilon;
  // Indices of the oriented (counterclockwise) convex hull of all points:
  std::vector<int> orientedHull;
  trSet.clear();
  int numPoints = pSet.size();

  for (int pntNum=0; pntNum<numPoints; pntNum++) {
    Point<T> p = pSet[pntNum];
    // bool foundEnclosure = false;
    int sidePnt1, sidePnt2, oppPnt;

#if DEBUG_TRIANG
    int trNum = 0;
    // std::cout << std::fixed;
    // std::cout.precision (20);
    // std::cout.width (30);
    int pntWidth = 40, pntPrec=34;
    std::cout << dbgMarker << " point "
	      << std::setw(4) << pntNum << " " << std::fixed
	      << std::setw(pntWidth) << std::setprecision (pntPrec) << pSet[pntNum].x << " "
	      << std::setw(pntWidth) << std::setprecision (pntPrec) << pSet[pntNum].y << std::endl;
#endif
    
    PointVsTriangle tryResult = NoTriangles;

    for (std::set<Triangle>::iterator tp = trSet.begin(); tp != trSet.end(); tp++) {

#if DEBUG_TRIANG
      //      printf ("Point %7d Triangle %7d of %7ld\n", pntNum, trNum++, trSet.size());
#endif
      // Triangle tr = *tp;
      tryResult = tryTriangle<T> (pntNum,
				  tp,
				  pSet,
				  epsilon2,
				  &sidePnt1,
				  &sidePnt2,
				  &oppPnt,
				  trSet);

#if DEBUG_TRIANG
      std::cout
	//	<<  "Point " << pntNum
	//	<< " (" << pSet[pntNum].x << ", " << pSet[pntNum].y << ") "
	<< " Triangle " << trNum++
	<< " (" << tp->ix0 << ", " << tp->ix1 << ", " << tp->ix2 << ") "
	<< " of " <<  trSet.size()
	<< "    tryTriangle ==> " << tryResult << std::endl;
#endif
      if ((tryResult == Inside)    ||
	  (tryResult == Duplicate) ||
	  (tryResult == OnASideInternal) ||
	  (tryResult == OnASideExternal))
	break;
    }

    int hullLen = orientedHull.size();

    switch (tryResult) {

      case Inside:
	// The oriented hull need not change, so do nothing.
	break;

      case NoTriangles:
	// Getting here means that either there are fewer than 3 points, or that
	// all points are on the same straight line.
	switch (hullLen) {
	  case 0:
	  case 1:
	    orientedHull.push_back (pntNum);
	    break;
	  case 2:
	    {
	      int
		px0 = orientedHull[0],
		px1 = orientedHull[1];
	      if (isInLine (pSet[px0], pSet[px1], pSet[pntNum], epsilon2)) 
		expandStraightLine (pntNum, orientedHull, pSet, epsilon2);
	      else {
		// Form a triangle, but make sure the orientation of the hull is counter-clockwise:
		orientedHull.push_back (pntNum);
		if (det (pSet[orientedHull[1]]-pSet[orientedHull[0]],
			 pSet[orientedHull[2]]-pSet[orientedHull[1]]) < 0)
		  std::swap (orientedHull[1], orientedHull[2]);
		addTriangle (orientedHull[0], orientedHull[1], orientedHull[2], trSet);
		break;
	      }
	    }
	    break;

	  default:

	    // WRONG ! : orientedHull is a straight line, but we might add triangles !!

	    if (isInLine (pSet[pntNum], pSet[orientedHull[0]], pSet[orientedHull[1]],
			  epsilon2))
	      expandStraightLine (pntNum, orientedHull, pSet, epsilon2);
	    else {  // Add bunch of triangles and update the hull:

	      for (int k=0; k<hullLen-1; k++)
		addTriangle (orientedHull[k], orientedHull[k+1], pntNum, trSet);

	      Point<T>
		lineDir = pSet[orientedHull[1]] - pSet[orientedHull[0]],
		pntDir  = pSet[pntNum] - pSet[orientedHull[0]];
	      if (clockwise (lineDir, pntDir))
		orientedHull.insert (orientedHull.begin(), pntNum);
	      else
		orientedHull.push_back (pntNum);
	    }
	    break;
	}
	break;
      case Outside:
	// Build new triangles and update Oriented Hull
	{
	  // Connect 'p' to the "near" points of the oriented hull and update the hull.
	  std::vector<int> connectedPoints (hullLen, 0);
	  std::vector<int> newHull;
	  // Create new triangles and mark the 'connectedPoints' to prepare for
	  // the rebuilding.
	  for (int k0=0; k0<hullLen; k0++) {
	    int
	      k1 = (k0+1) % hullLen,
	      hk0 = orientedHull[k0],
	      hk1 = orientedHull[k1];
	    Point<T>
	      hp0 = pSet[hk0],
	      hp1 = pSet[hk1];
	    if (det  (p-hp0, p-hp1) < 0) {
	      addAndFlip (pntNum, hk0, hk1, pSet, trSet);
	      connectedPoints[k0]++;
	      connectedPoints[k1]++;
	      // addAndFlip (pntNum, orientedHull[k], orientedHull[k1], pSet, trSet);
	      // trSet.insert (Triangle (pntNum, orientedHull[k], orientedHull[k1]));
	    }
	  }
	  // Now rebuild the oriented hull using 'connectedPoints'.
	  // We always place the new point after the last 'one' and an increase.
	  int prevConnectedness = connectedPoints[hullLen-1];
	  // 'lastOne' is negative if A) no ones have been seen yet, or B) the new
	  //     point has been inserted;
	  int lastOne = -99;
	  for (int k=0; k<hullLen; k++) {
	    if (connectedPoints[k] < 2)
	      newHull.push_back(orientedHull[k]);
	    if (connectedPoints[k] == 1)
	      lastOne = k;
	    if (connectedPoints[k] > prevConnectedness) {
	      if (lastOne >= 0) {
		newHull.push_back(pntNum);
		lastOne = -99;
	      }
	    }
	    prevConnectedness = connectedPoints[k];
	  }
	  orientedHull = newHull;
	}
	break;

      case  OnASideExternal:
	// Just add the new point to the oriented hull. First find the locations of
	// adjacent virtices 'sidePnt1, sidePnt2:
	{
	  int
	    pntIx1 = -1, pntIx2 = -1;
	  for (int k=0; k<hullLen; k++) {
	    if (orientedHull[k] == sidePnt1)
	      pntIx1 = k;
	    if (orientedHull[k] == sidePnt2)
	      pntIx2 = k;
	  }
	  if (pntIx1 > pntIx2)
	    std::swap (pntIx1, pntIx2);
	  if ((0 <= pntIx1) && (pntIx2 == pntIx1 + 1))
	    orientedHull.insert (orientedHull.begin() + pntIx2, pntNum);
	  else if ((pntIx1 == 0) && (pntIx2 == hullLen-1))
	    orientedHull.push_back (pntNum);
	  else
	    throw std::runtime_error ("Did not find hull points in proper positions");
	}
	break;

      default:
#if DEBUG_TRIANG
	std::cerr << "   tryAngle returned " << tryResult << std::endl;
#endif
	throw std::runtime_error ("Bad result from tryTriangle");
	break;
    }
  }   // Loop by points
}

#endif /*INCLUDED_triang_hpp_85662733*/
