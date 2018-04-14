// Test triangulation for a specific set of points where a bug has
// been detected before.

#include "triang.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <iostream>
#include <fstream>

static int pointsXY[][2] = {
  {181,271}, {183,310}, {185,349}, {189,388}, {199,425}, {218,459}, {242,488}, {269,517},
  {305,529}, {344,526}, {378,505}, {412,478}, {441,446}, {458,406}, {467,363}, {476,319},
  {483,274}, {198,276}, {216,261}, {241,261}, {264,269}, {287,279}, {341,276}, {369,267},
  {396,263}, {425,265}, {447,281}, {313,305}, {311,333}, {308,360}, {305,389}, {276,392},
  {290,400}, {306,408}, {324,401}, {341,395}, {230,300}, {246,292}, {265,293}, {280,306},
  {262,308}, {244,307}, {230,300}, {356,309}, {373,297}, {392,297}, {409,305}, {394,312},
  {374,312}, {356,309}, {248,417}, {271,418}, {292,419}, {308,423}, {326,421}, {350,422},
  {376,425}, {350,454}, {324,469}, {305,470}, {288,466}, {267,449}, {248,417}, {258,422},
  {291,427}, {307,430}, {325,429}, {366,428}, {325,449}, {307,450}, {290,447}, {258,422},
  {543,328}, {551,359}, {561,388}, {579,415}, {598,441}, {618,464}, {638,486}, {659,508},
  {687,513}, {717,503}, {745,478}, {771,448}, {793,414}, {804,378}, {809,338}, {811,297},
  {807,255}, {540,297}, {550,289}, {564,292}, {579,295}, {595,301}, {654,292}, {672,276},
  {693,266}, {717,259}, {742,261}, {631,324}, {634,353}, {636,382}, {639,411}, {625,412},
  {636,420}, {651,424}, {665,414}, {678,404}, {565,326}, {577,317}, {595,316}, {612,327},
  {595,333}, {577,334}, {565,326}, {675,316}, {686,301}, {704,295}, {722,299}, {710,310},
  {692,315}, {675,316}, {626,434}, {638,433}, {650,432}, {664,433}, {678,427}, {702,421},
  {731,413}, {710,441}, {689,457}, {673,461}, {658,461}, {643,453}, {626,434}, {635,439},
  {652,439}, {666,439}, {681,435}, {722,419}, {684,441}, {669,444}, {655,443}, {635,439}
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
