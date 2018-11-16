#ifndef BASETYPES_H
#define BASETYPES_H
#include <Eigen/Core>
#include <vector>
#include <stack>

typedef Eigen::Vector2i Vec2i;
typedef Eigen::Vector2f Vec2f;
typedef Eigen::Vector2d Vec2d;

typedef std::vector<Vec2i, Eigen::aligned_allocator<Vec2i> > VectorVec2i;
typedef std::vector<Vec2f, Eigen::aligned_allocator<Vec2f> > VectorVec2f;
typedef std::vector<Vec2d, Eigen::aligned_allocator<Vec2d> > VectorVec2d;

#endif