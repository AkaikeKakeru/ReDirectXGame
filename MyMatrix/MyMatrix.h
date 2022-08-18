#pragma once
#include "Matrix4.h"
#include "Vector3.h"
#include "MathUtility.h"
#include "directxmath.h"
using namespace DirectX;
using namespace MathUtility;

typedef struct MyMatrix
{
	Matrix4 MatrixScale(Vector3 scale);
	Matrix4 MatrixRotationX(Vector3 rotation);
	Matrix4 MatrixRotationY(Vector3 rotation);
	Matrix4 MatrixRotationZ(Vector3 rotation);
	Matrix4 MatrixTranslation(Vector3 translation);
	Vector3 CrossVector(Vector3 vector, Matrix4 matrix);
	Matrix4 MatrixWorld(Matrix4 scale, Matrix4 rotation, Matrix4 translation);
}MyMatrix;
