#include "MyVector3.h"

//線形補間
Vector3 MyVector3::Lerp(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 result;
	//Vector3 a = v2 - v1;
	//Vector3 b = t * a;
	//result = v1 + b;

	//計算
	result = v1 + (t * (v2 - v1));

	return result;
};

//球面線形補間
Vector3 MyVector3::Slerp(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 result;
	//v1,P1 = start;
	Vector3 s = v1;
	Vector3Normalize(s);

	//v2,P2 = end;
	Vector3 e = v2;
	Vector3Normalize(e);

	//2つのベクトル間の角度
	float angle = std::acos(Vector3Dot(s, e));

	//sinθ
	float sinTheta = std::sin(angle);

	//補完係数
	float P1 = std::sin(angle * (1 - t));
	float P2 = std::sin(angle * t);

	//計算
	result = (P1 * s + P2 * e)/sinTheta;
	result = Vector3Normalize(result);

	return result;
};
