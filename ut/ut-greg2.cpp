// Test triangulation for a specific set of points where a bug has
// been detected before.

#if 0
#ifdef DEBUG_TRIANG
#undef DEBUG_TRIANG
#endif
#define DEBUG_TRIANG 2
#endif

#include "triang.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <iostream>
#include <fstream>

static int pointsXY[][2] = {
  {175, 271}, {177, 310}, {179, 348}, {183, 386}, {195, 423}, {216, 456}, {242, 487},
  {269, 518}, {305, 528}, {345, 526}, {379, 503}, {413, 477}, {443, 444}, {458, 405},
  {467, 363}, {476, 321}, {483, 277}, {198, 277}, {216, 262}, {241, 262}, {265, 269},
  {288, 280}, {340, 277}, {369, 267}, {397, 263}, {425, 266}, {446, 282}, {313, 305},
  {311, 333}, {308, 361}, {305, 389}, {276, 391}, {290, 400}, {306, 408}, {323, 401},
  {340, 395}, {230, 300}, {246, 292}, {265, 293}, {280, 306}, {263, 308}, {244, 307},
  {230, 300}, {357, 308}, {373, 296}, {393, 296}, {409, 305}, {394, 313}, {375, 312},
  {357, 308}, {248, 417}, {268, 416}, {289, 419}, {306, 423}, {325, 420}, {350, 421},
  {377, 425}, {351, 454}, {324, 469}, {305, 470}, {286, 467}, {265, 449}, {248, 417},
  {257, 422}, {289, 427}, {306, 430}, {324, 429}, {367, 428}, {325, 446}, {306, 448},
  {288, 444}, {257, 422}, {543, 327}, {551, 358}, {562, 387}, {579, 414}, {598, 441},
  {619, 465}, {638, 487}, {660, 509}, {688, 513}, {717, 503}, {746, 478}, {771, 449},
  {793, 415}, {805, 378}, {809, 338}, {811, 297}, {807, 255}, {540, 297}, {550, 289},
  {564, 292}, {579, 296}, {596, 302}, {654, 292}, {672, 277}, {693, 267}, {716, 259},
  {742, 261}, {632, 324}, {635, 353}, {637, 382}, {640, 411}, {625, 412}, {636, 420},
  {652, 424}, {666, 414}, {679, 404}, {566, 326}, {578, 317}, {595, 317}, {612, 327},
  {595, 332}, {578, 333}, {566, 326}, {675, 316}, {686, 301}, {704, 295}, {722, 298},
  {710, 310}, {692, 315}, {675, 316}, {627, 435}, {639, 434}, {651, 432}, {665, 433},
  {679, 427}, {701, 421}, {730, 413}, {710, 442}, {689, 457}, {674, 461}, {659, 461},
  {644, 454}, {627, 435}, {636, 439}, {653, 439}, {667, 438}, {681, 434}, {721, 418},
  {684, 441}, {670, 445}, {656, 444}, {636, 439},
};

template <typename T>
static void initPointSet (std::vector<Point<T> >& pSet, int first = 0, int last = -1)
{
  if (last < 0)
    last = sizeof (pointsXY) / sizeof (pointsXY[0]);
  for (int k=first; k<last; k++)
    pSet.push_back (Point<T> (static_cast<T> (pointsXY[k][0]),
			      static_cast<T> (pointsXY[k][1])));
}

template <typename T> void run1test ()
{
  std::vector<Point<T> >    pSet;

  // initPointSet (pSet, 71, 85);
  initPointSet (pSet);

  std::set <Triangle>         trSet;

  delaunay<T> (pSet, trSet);
  dumpXG<T>   (pSet, trSet, "ug.xg");
  dumpPS<T>   (pSet, trSet, "ug.ps", 200., 450.);

  try {
    edgeCrossingCheck<T> (pSet, trSet);
    delaunayCheck<T>     (pSet, trSet);
  }
  catch (EdgeCrossError<T> ee) {
    const char* dumpName = "edge-cross.xg";
    std::cerr << " Edge Crossing EXCEPTION. Dumping to \"" << dumpName << "\"" << std::endl;
    dumpEdgeCrossError<T> (ee, dumpName);
  }
  catch (DelaunayError<T> de) {
    const char* dumpName = "delaunay-error.xg";
    std::cerr << " DelaunayError EXCEPTION. Dumping to \"" << dumpName << "\"" << std::endl;
    dumpDelaunayError<T> (de, dumpName);
  }
  catch (std::runtime_error re) {
    std::cerr << " Runtime Error caught: " << re.what() << std::endl;
  }
}

int main (int argc, const char *argv[])
{
  //run1test<double> ();
   run1test<int> ();
  return 0;
}
