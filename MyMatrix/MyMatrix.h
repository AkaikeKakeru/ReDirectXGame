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
	//�X�P�[���s��
	Matrix4 MatrixScale(Vector3 scale);
	//X����]�s��
	Matrix4 MatrixRotationX(Vector3 rotation);
	//Y����]�s��
	Matrix4 MatrixRotationY(Vector3 rotation);
	//Z����]�s��
	Matrix4 MatrixRotationZ(Vector3 rotation);
	//���s�ړ��s��
	Matrix4 MatrixTranslation(Vector3 translation);
	//�x�N�g���ƍs��̊|���Z
	Vector3 CrossVector(Vector3 vector, Matrix4 matrix);
	//�x�N�g���ƍs��̊|���Z W���Z����
	Vector3 CrossVectorW(Vector3 vector, Matrix4 matrix);
	//���[���h�s��
	Matrix4 MatrixWorld(Matrix4 scale, Matrix4 rotation, Matrix4 translation);
	//�r���[�|�[�g�s��
	Matrix4 MatrixViewPort(float VpWidth,float VpHeight,Vector2 offset);
}MyMatrix;
