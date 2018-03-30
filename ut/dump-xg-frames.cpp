
#define DEBUG_TRIANG 1
#include "triang.hpp"

#include <cstdio>
#include <cstring>

#define BUFLEN 1024
#define STREQ(A,B) (!strcmp(A,B))

int main (int argc, const char *argv[])
{

  char buf[BUFLEN+8];

  while (fgets (buf, BUFLEN, stdin)) {
    char* s = strtok (buf, " ");
    if (STREQ (dbgMarker.c_str(), s)) {
      s = strtok (NULL, " ");
      if (STREQ (s, "point")) {
	printf ("-- %s --\n", s);
      }
      else if (STREQ("triangle", s)) {
      }
    }
  }
  return 0;
}
