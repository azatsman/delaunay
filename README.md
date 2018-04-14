Delaunay triangulation implemented as a C++ template:

template <typename T>
void delaunay (const std::vector<Point<T> >& pSet, std::set <Triangle>& trSet)

Input:  vector of Point's
Output: set of Triangle's

Both Point's and Triangle's are defined in 'dtypes.h'
