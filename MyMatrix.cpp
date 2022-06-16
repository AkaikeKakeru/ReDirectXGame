#include "MyMatrix.h"
Matrix4 MatrixScale(Vector3 scale)
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
		0,cos(rotation.x),sin(rotation.x),0,
		0,-sin(rotation.x),cos(rotation.x),0,
		0,0,0,1 };
	return mat;
}

Matrix4 MyMatrix::MatrixRotationY(Vector3 rotation)
{
	Matrix4 mat = MathUtility::Matrix4Identity();
	mat =
	{ cos(rotation.y),0,-sin(rotation.y),0,
		0,1,0,0,
		sin(rotation.y),0,cos(rotation.y),0,
		0,0,0,1 };
	return mat;
}

Matrix4 MyMatrix::MatrixRotationZ(Vector3 rotation)
{
	Matrix4 mat = MathUtility::Matrix4Identity();
	mat =
	{ cos(rotation.z),sin(rotation.z),0,0,
		-sin(rotation.z),cos(rotation.z),0,0,
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