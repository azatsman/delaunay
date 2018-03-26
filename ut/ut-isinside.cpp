#include "triang.hpp"
#include <fstream>
#include <cstdlib>

#define GET_STATS 1

static bool doOne ()
{
  const char* fileName = "isinside.xg";
  Point<double>
    tp1 (drand48(), drand48()),
    tp2 (drand48(), drand48()),
    tp3 (drand48(), drand48()),
    p   (drand48(), drand48());
  bool isIn = isInside (p, tp1,tp2,tp3);

  {
    std::ofstream xgf (fileName);
    const char* title = isIn ? "In" : "Out";
    xgf
      <<  "TitleText: "   << title << std::endl
      << "move " << p.x   << " " << p.y   << std::endl
      << "     " << tp1.x << " " << tp1.y << std::endl
      << "move " << p.x   << " " << p.y   << std::endl
      << "     " << tp2.x << " " << tp2.y << std::endl
      << "move " << p.x   << " " << p.y   << std::endl
      << "     " << tp3.x << " " << tp3.y << std::endl;
  }
  return isIn;
}

int main (int argc, const char *argv[])
{
  srand48 (17);
  doOne();
  return 0;
}
