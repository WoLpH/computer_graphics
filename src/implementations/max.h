#ifndef MAX_H
#define MAX_H
#include "../Ray.h"

Ray performRayTracingMax(Ray ray);

Vec3Df crossProduct(Vec3Df in1, Vec3Df in2);
float dotProduct(Vec3Df in1, Vec3Df in2);
inline Vec3Df minus_(Vec3Df in1, Vec3Df in2);
bool intersect(Triangle t, Ray ray);

#endif
