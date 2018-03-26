# Delaunay triangulation

This is a C++ implementation of [Delaunay triangulation][wikipedia_link].

[wikipedia_link]: https://en.wikipedia.org/wiki/Delaunay_triangulation

All the required code is in a single include file `triang.hpp`, where the `delaunay`
function does the work.

The `ut` subdirectory contains unit tests, which can be used as examples of how to use the
functions from `triang.hpp`. The main test is in `ut-triang.cpp`. The resulting executable
can be run as
```
   ut-triang [<num-points>]
```
where `<num-points` defaults to 7.
The test creates two output files:
- `triang.xg`, which can be displayed by the [xgraph] program.
- `triang.ps`, which is a PostScript file.

[xgraph]: https://www.isi.edu/nsnam/xgraph/
