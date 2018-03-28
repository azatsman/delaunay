#ifndef INCLUDED_triang_hpp_85662733
#define INCLUDED_triang_hpp_85662733

#include <vector>
#include <set>
#include <stdexcept>

template <typename T>
struct Point {
  T x;
  T y;
  Point<T> (T xx, T yy) : x(xx), y(yy) {};
  Point<T> operator+(Point<T> p) const {return Point<T> (x+p.x, y+p.y);};
  Point<T> operator-(Point<T> p) const {return Point<T> (x-p.x, y-p.y);};
  T norm2 () const {return (x*x+y*y);};
};

// Return the determinant of the matrix formed by 'a' and 'b'

template <typename T> T det (Point<T> a, Point<T> b)
{
  return (a.x*b.y - a.y*b.x);
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

// Add new triangle with a new point pntNum and old points i0, i1.
// If i0,i1 belong to an old triangle, and pntNum is inside the circle
// circumscribed around the triangle than "flip" the new and old triangles.

template <typename T>
void addAndFlip (int pntNum, int i0, int i1,
                 const std::vector<Point<T> >& pntVector,
                 std::set <Triangle>&  trSet)
{
  int i2;
  std::set <Triangle>::iterator tp = findAdjacent (trSet, i0, i1, -1, &i2);
  if (tp != trSet.end()) {
    if (isInCircumCircle (pntVector[pntNum], pntVector[i0], pntVector[i1], pntVector[i2])) {
      trSet.erase (tp);
      addAndFlip (pntNum, i0, i2, pntVector, trSet);
      addAndFlip (pntNum, i1, i2, pntVector, trSet);
    }
    else {
      trSet.insert (Triangle (pntNum, i0, i1));
    }
  }
  else
    trSet.insert (Triangle (pntNum, i0, i1));
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
		      OnASide,          // On a side of the triangle, but not in a corner
		      OnASideExternal,  // On a side without adjacent triangle.
		      Duplicate,        // Too close to one of the corners.
		      Outside,          // Completely outside of this triangle.
		      Unknown};

void addTriangle (int vertIx0, int vertIx1, int vertIx2,
		  std::set<Triangle> trSet)
{
  trSet.insert (Triangle (vertIx0, vertIx1, vertIx2));
}

// Return true if two adjacent triangles
//    (adjVert1, adjVert2, oppVert1)   and   (adjVert1, adjVert2, oppVert2)
// are "non-Delaunay" and have to be flipped to 
//    (adjVert1, oppVert1, oppVert2)   and   (adjVert2, oppVert1, oppVert2):

template <typename T>
bool mustFlip (int adjVert1, int adjVert2,
	       int oppVert1, int oppVert2,
	       std::vector<Point<T> > pntVector)
{
  Point<T>
    adjPnt1 = pntVector[adjVert1],
    adjPnt2 = pntVector[adjVert2],
    oppPnt1 = pntVector[oppVert1],
    oppPnt2 = pntVector[oppVert2];
  return
    isInCircumCircle (oppPnt2, adjPnt1, adjPnt2, oppPnt1);
}

// ...............

template <typename T>
void addSideTriange (int newPnt,  int sidePnt1, int sidePnt2,
		     std::vector<Point<T> > pntVector,
		     std::set<Triangle> trSet)
{
  int nbVert;
  std::set<Triangle>::iterator adjTr = findAdjacent (trSet, sidePnt1, sidePnt2, newPnt, &nbVert);
  if ((adjTr != trSet.end()) &&
      (mustFlip (sidePnt1, sidePnt2, newPnt, nbVert, pntVector)) ) {
    trSet.erase (adjTr);
    addTriangle (newPnt, sidePnt1, nbVert, trSet);
    addTriangle (newPnt, sidePnt2, nbVert, trSet);
  }
  else
    addTriangle (newPnt, sidePnt1, sidePnt2, trSet);
}


// Try inserting new point in the triangle.
// Several cases depending on the returned value:
// == Inside: the point is inside the triangle and not "too close" to any of the sides 
//      -- 3 new triangles are created
//      -- Adjacent triangles, if any, are checked for flipping.
// == OnASide:
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
			     const std::vector<T> pntVector,
			     T epsilon2, // relative precision.
			     int* adjVertex1, int* adjVertex2, int* oppositeVertex,
			     std::set<Triangle> trSet)
{
  PointVsTriangle rslt = Unknown;
  Triangle trngl = *tp;
  Point<T>
    p = pntVector[pntNum],
    v0 = trngl.ix0,
    v1 = trngl.ix1,
    v2 = trngl.ix2,
    t0 = pntVector[v0],
    t1 = pntVector[v1],
    t2 = pntVector[v2];
  T
    roughSize2 = (t1-t0).norm2() + (t2-t0).norm2(),
    absEpsilon2 = absEpsilon2,  // absolute instead of relative
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
    trSet.erase (tp);
    addSideTriangle (pntNum, v0, v1, pntVector, trSet);
    addSideTriangle (pntNum, v1, v2, pntVector, trSet);
    addSideTriangle (pntNum, v2, v0, pntVector, trSet);
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

  if (((p-pntVector[v0]).norm2 < absEpsilon2) || ((p-pntVector[v1]).norm2 < absEpsilon2))
    return Duplicate;

  // At this stage the point is very close to the [g0,g1] line, but not close to either end:

  *adjVertex1     = v0;
  *adjVertex2     = v1;
  *oppositeVertex = vOpp;

  int nbVert;

  // Check whether there is a triangle adjacent to the [v0,v1] side:

  std::set<Triangle>::iterator adjTr = findAdjacent (trSet, v0, v1,
						    vOpp, &nbVert);

  if (adjTr == trSet.end()) {
    // No adjacent triangles, no flipping possible
    trSet.erase(tp);
    addTriangle (pntNum, v0, vOpp,  trSet);
    addTriangle (pntNum, v1, vOpp,  trSet);
    return OnASideExternal;
  }
  else {
    // If an adjacent triangle exists then erase both the current triangle and
    // the adjacent one, then check for flipping "around" [v0,v1]:

    trSet.erase (tp);
    trSet.erase (adjTr);

    // Check the triangle containing v0 for flipping:
    if (mustFlip (v0, pntNum, vOpp, nbVert, pntVector)) {
      addTriangle (pntNum, vOpp, nbVert, trSet);
      addTriangle (v0    , vOpp, nbVert, trSet);
    }
    else {
      addTriangle (pntNum, v0, vOpp, trSet);
      addTriangle (pntNum, v0, nbVert, trSet);
    }
    // Check the triangle containing v1 for flipping:
    if (mustFlip (v1, pntNum, vOpp, nbVert, pntVector)) {
      addTriangle (pntNum, vOpp, nbVert, trSet);
      addTriangle (v1    , vOpp, nbVert, trSet);
    }
    else {
      addTriangle (pntNum, v1, vOpp,   trSet);
      addTriangle (pntNum, v1, nbVert, trSet);
    }
    return OnASideExternal;
  }
}


template <typename T>
void delaunay (const std::vector<Point<T> >& pSet,
               std::set <Triangle>&          trSet)
{
  // Indices of the oriented (counterclockwise) convex hull of all points:
  std::vector<int> orientedHull;
  trSet.clear();
  int numPoints = pSet.size();
  for (int pntNum=0; pntNum<numPoints; pntNum++) {
    Point<T> p = pSet[pntNum];
    bool foundEnclosure = false;
    for (std::set<Triangle>::iterator tp = trSet.begin(); tp != trSet.end(); tp++) {
      Triangle tr = *tp;
      if (isInside (p, pSet[tr.ix0], pSet[tr.ix1], pSet[tr.ix2])) {
        trSet.erase (tp);
        addAndFlip (pntNum, tr.ix0, tr.ix1, pSet, trSet);
        addAndFlip (pntNum, tr.ix1, tr.ix2, pSet, trSet);
        addAndFlip (pntNum, tr.ix2, tr.ix0, pSet, trSet);
        foundEnclosure = true;
        break;
      }
    }
    if (! foundEnclosure) {
      switch (orientedHull.size()) {
        case 0:
        case 1:
          orientedHull.push_back (pntNum);
          break;
        case 2:
          orientedHull.push_back (pntNum);
          if (det (pSet[orientedHull[1]]-pSet[orientedHull[0]],
                   pSet[orientedHull[2]]-pSet[orientedHull[1]]) < 0)
            std::swap (orientedHull[1], orientedHull[2]);
          trSet.insert (Triangle (orientedHull[0], orientedHull[1], orientedHull[2]));
          break;
        default:
          {
            // Connect 'p' to the "near" points of the oriented hull and update the hull.
            int hullLen = orientedHull.size();
            std::vector<int> connectedPoints (hullLen, 0);
            std::vector<int> newHull;
            // Create new triangles and mark the 'connectedPoints' to prepare for
            // the rebuilding.
            for (int k=0; k<hullLen; k++) {
              int k1 = (k+1) % hullLen;
              Point<T>
                hp0 = pSet[orientedHull[k]],
                hp1 = pSet[orientedHull[k1]];
              if (det  (p-hp0, p-hp1) < 0) {
                connectedPoints[k ]++;
                connectedPoints[k1]++;
                addAndFlip (pntNum, orientedHull[k], orientedHull[k1], pSet, trSet);
                // trSet.insert (Triangle (pntNum, orientedHull[k], orientedHull[k1]));
              }
            }
            // Now rebuild the oriented hull using 'connectedPoints'.
            // We always place the new point after the last one and an increase.
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
      }
    }
  }
}

#endif /*INCLUDED_triang_hpp_85662733*/
