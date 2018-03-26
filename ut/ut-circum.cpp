// Test circumscribed circles

#include "triang.hpp"
#include <cstdlib>
#include <fstream>

int main (int argc, const char *argv[])
{
  int nn = 100000;
  srand48 (17);
  const char* fileName = "circum.log";
  std::ofstream ofl (fileName);
  
  for (int k=0; k<nn; k++) {
    Point<double>
      tp1 (drand48(), drand48()),
      tp2 (drand48(), drand48()),
      tp3 (drand48(), drand48()),
      ctr = circumCtr (tp1, tp2, tp3);

#if 1
    if (k == 0) {
      std::ofstream xgf ("circum.xg");
      xgf
	<< "\"Triangle xg \"" << std::endl
	<< tp1.x << " " << tp1.y  << std::endl
	<< tp2.x << " " << tp2.y  << std::endl
	<< tp3.x << " " << tp3.y  << std::endl
	<< tp1.x << " " << tp1.y  << std::endl
	<< std::endl
	<< "\"Ctr  xg \"" << std::endl
	<< " move " << ctr.x << " " << ctr.y  << std::endl
	<< "      " << tp1.x << " " << tp1.y  << std::endl
	<< " move " << ctr.x << " " << ctr.y  << std::endl
	<< "      " << tp2.x << " " << tp2.y  << std::endl
	<< " move " << ctr.x << " " << ctr.y  << std::endl
	<< "      " << tp3.x << " " << tp3.y  << std::endl;
    }
#endif

    double
      d1 = (tp1 - ctr).norm2(),
      d2 = (tp2 - ctr).norm2(),
      d3 = (tp3 - ctr).norm2(),
      d12 = d2 - d1,
      d13 = d3 - d1,
      dd = d12*d12 + d13*d13;
    ofl << dd << " err " << std::endl;
  }
  return 0;
}
