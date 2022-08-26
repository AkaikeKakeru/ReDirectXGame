#pragma once
#include "directxmath.h"
#include "MathUtility.h"
#include "Vector3.h"
#include <cmath>
using namespace DirectX;
using namespace MathUtility;

typedef struct MyVector3{
//���`���
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

//���ʐ��`���
Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t);
}MyVector3;