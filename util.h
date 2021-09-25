/* util.h */

#ifndef UTIL_H
#define UTIL_H

#include <raylib.h>

float UtilDistance(Vector2 a, Vector2 b);
float UtilSmootherstep(float A, float B, float v);
float UtilOverlapArea(Rectangle a, Rectangle b);
char* UtilOrdToShortString(int ord);
char* UtilOrdToLongString(int ord);
char* UtilSuitToShortString(int suit);
char* UtilSuitToLongString(int suit);

#endif
