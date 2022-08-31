#pragma once
#include "Matrix4.h"
#include "Vector3.h"
#include "Vector2.h"
#include "MathUtility.h"
#include "directxmath.h"
using namespace DirectX;
using namespace MathUtility;

typedef struct MyMatrix
{
	//スケール行列化
	Matrix4 MatrixScale(Vector3 scale);
	//X軸回転行列化
	Matrix4 MatrixRotationX(Vector3 rotation);
	//Y軸回転行列化
	Matrix4 MatrixRotationY(Vector3 rotation);
	//Z軸回転行列化
	Matrix4 MatrixRotationZ(Vector3 rotation);
	//平行移動行列化
	Matrix4 MatrixTranslation(Vector3 translation);
	//ベクトルと行列の掛け算
	Vector3 CrossVector(Vector3 vector, Matrix4 matrix);
	//ベクトルと行列の掛け算 W除算込み
	Vector3 CrossVectorW(Vector3 vector, Matrix4 matrix);
	//ワールド行列化
	Matrix4 MatrixWorld(Matrix4 scale, Matrix4 rotation, Matrix4 translation);
	//ビューポート行列化
	Matrix4 MatrixViewPort(float VpWidth,float VpHeight,Vector2 offset);
}MyMatrix;
