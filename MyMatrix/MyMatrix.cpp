#include "MyMatrix.h"
Matrix4 MyMatrix::MatrixScale(Vector3 scale)
{
	Matrix4 mat = MathUtility::Matrix4Identity();
	mat = {
		scale.x,0,0,0,
		0,scale.y,0,0,
		0,0,scale.z,0,
		0,0,0,1 };
	return mat;
}

Matrix4 MyMatrix::MatrixRotationX(Vector3 rotation)
{
	Matrix4 mat = MathUtility::Matrix4Identity();
	mat = {
		1,0,0,0,
		0,static_cast<float>(cos(rotation.x)),static_cast<float>(sin(rotation.x)),0,
		0,static_cast<float>(-sin(rotation.x)),static_cast<float>(cos(rotation.x)),0,
		0,0,0,1 };
	return mat;
}

Matrix4 MyMatrix::MatrixRotationY(Vector3 rotation)
{
	Matrix4 mat = MathUtility::Matrix4Identity();
	mat = {
		static_cast<float>(cos(rotation.y)),0,static_cast<float>(-sin(rotation.y)),0,
		0,1,0,0,
		static_cast<float>(sin(rotation.y)),0,static_cast<float>(cos(rotation.y)),0,
		0,0,0,1 };
	return mat;
}

Matrix4 MyMatrix::MatrixRotationZ(Vector3 rotation)
{
	Matrix4 mat = MathUtility::Matrix4Identity();
	mat = {
		static_cast<float>(cos(rotation.z)),static_cast<float>(sin(rotation.z)),0,0,
		static_cast<float>(-sin(rotation.z)),static_cast<float>(cos(rotation.z)),0,0,
		0,0,1,0,
		0,0,0,1 };
	return mat;
}


Vector3 MyMatrix::CrossVector(Vector3 vector, Matrix4 matrix)
{
	Vector3 ans(0, 0, 0);
	Matrix4 mat = matrix;
	Vector3 vec = vector;

	ans.x = vec.x * mat.m[0][0]
		+ vec.y * mat.m[1][0]
		+ vec.z * mat.m[2][0];

	ans.y = vec.x * mat.m[0][1]
		+ vec.y * mat.m[1][1]
		+ vec.z * mat.m[2][1];

	ans.z = vec.x * mat.m[0][2]
		+ vec.y * mat.m[1][2]
		+ vec.z * mat.m[2][2];

	return ans;
}

Matrix4 MyMatrix::MatrixTranslation(Vector3 translation)
{
	Matrix4 mat = MathUtility::Matrix4Identity();
	mat = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		translation.x,translation.y,translation.z,1 };
	return mat;
}

Matrix4 MyMatrix::MatrixWorld(Matrix4 scale, Matrix4 rotation, Matrix4 translation)
{
	Matrix4 mat = MathUtility::Matrix4Identity();
	mat *= scale *= rotation *= translation;
	return mat;
}

