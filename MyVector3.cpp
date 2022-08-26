#include "MyVector3.h"

//���`���
Vector3 MyVector3::Lerp(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 result;
	//Vector3 a = v2 - v1;
	//Vector3 b = t * a;
	//result = v1 + b;

	//�v�Z
	result = v1 + (t * (v2 - v1));

	return result;
};

//���ʐ��`���
Vector3 MyVector3::Slerp(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 result;
	//v1,P1 = start;
	Vector3 s = v1;
	Vector3Normalize(s);

	//v2,P2 = end;
	Vector3 e = v2;
	Vector3Normalize(e);

	//2�̃x�N�g���Ԃ̊p�x
	float angle = std::acos(Vector3Dot(s, e));

	//sin��
	float sinTheta = std::sin(angle);

	//�⊮�W��
	float P1 = std::sin(angle * (1 - t));
	float P2 = std::sin(angle * t);

	//�v�Z
	result = (P1 * s + P2 * e)/sinTheta;
	result = Vector3Normalize(result);

	return result;
};
