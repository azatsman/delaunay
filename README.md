Delaunay triangulation implemented as a C++ template. The implementation is pretty
straightforward, without any optimizaion. It runs in O(n^2) time, and might take forever
if you have more than a few hundred points.

The interface:

```
template <typename T>
void delaunay (const std::vector<Point<T> >& pSet, std::set <Triangle>& trSet)
```

* Input:  vector of `Point`s
* Output: set of `Triangle`s

Both `Point` and `Triangle` are defined and documented in `dtypes.h`

Typically type `T` is either `float` or `double`. When invoked with integer types it converts
them internally to `double`s.

Here is an example of invocation for a set of random points, where `DRAND` is expected to
return a randomly generated double-precision floating point number:

```
  std::vector<Point<double> > pSet;
  std::set <Triangle>        trSet;
  for (int k=0; k<numPoints; k++)
    pSet.push_back (Point<double> (DRAND(), DRAND()));
  delaunay (pSet, trSet);
```

