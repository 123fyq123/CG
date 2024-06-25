//
// Created by LEI XU on 5/16/19.
//

#ifndef RAYTRACING_INTERSECTION_H
#define RAYTRACING_INTERSECTION_H
#include "Vector.hpp"
#include "Material.hpp"
class Object;
class Sphere;

struct Intersection
{
    Intersection()
    {
        happened = false;                              // 是否相交
        coords = Vector3f();                           // 交点坐标
        normal = Vector3f();                           // 法向量
        distance = std::numeric_limits<double>::max(); // 举例
        obj = nullptr;
        m = nullptr;
    }
    bool happened;
    Vector3f coords;
    Vector3f normal;
    double distance;
    Object *obj;
    Material *m;
};
#endif // RAYTRACING_INTERSECTION_H
