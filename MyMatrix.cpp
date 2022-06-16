#include "MyMatrix.h"
Matrix4 MyMatrix::MatrixScale(Vector3 scale)
{
	Matrix4 mat = MathUtility::Matrix4Identity();
	mat =
	{ scale.x,0,0,0,
		0,scale.y,0,0,
		0,0,scale.z,0,
		0,0,0,1 };
	return mat;
}

Matrix4 MyMatrix::MatrixRotationX(Vector3 rotation)
{
	Matrix4 mat = MathUtility::Matrix4Identity();
	mat =
	{ 1,0,0,0,
		0,static_cast<float>(cos(rotation.x)),static_cast<float>(sin(rotation.x)),0,
		0,static_cast<float>(-sin(rotation.x)),static_cast<float>(cos(rotation.x)),0,
			0,0,0,1 };
	return mat;
}

Matrix4 MyMatrix::MatrixRotationY(Vector3 rotation)
{
	Matrix4 mat = MathUtility::Matrix4Identity();
	mat =
	{ static_cast<float>(cos(rotation.y)),0,static_cast<float>(-sin(rotation.y)),0,
		0,1,0,0,
		static_cast<float>(sin(rotation.y)),0,static_cast<float>(cos(rotation.y)),0,
			0,0,0,1 };
	return mat;
}

Matrix4 MyMatrix::MatrixRotationZ(Vector3 rotation)
{
	Matrix4 mat = MathUtility::Matrix4Identity();
	mat =
	{ static_cast<float>(cos(rotation.z)),static_cast<float>(sin(rotation.z)),0,0,
		static_cast<float>(-sin(rotation.z)),static_cast<float>(cos(rotation.z)),0,0,
		0,0,1,0,
		0,0,0,1 };
	return mat;
}

Matrix4 MyMatrix::MatrixTranslation(Vector3 translation)
{
	Matrix4 mat = MathUtility::Matrix4Identity();
	mat =
	{ 1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		translation.x,translation.y,translation.z,1 };
	return mat;
}

Matrix4 MyMatrix::MatrixWorld(Matrix4 scale,Matrix4 rotation, Matrix4 translation)
{
	Matrix4 mat = MathUtility::Matrix4Identity();
	mat *= scale;
	mat *= rotation;
	mat *= translation;
	return mat;
}