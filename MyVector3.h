#pragma once
#include "MathUtility.h"
#include "Vector3.h"

using namespace MathUtility;

typedef struct MyVector3{
//���`���
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);
}MyVector3;