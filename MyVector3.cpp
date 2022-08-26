#include "MyVector3.h"

Vector3 MyVector3::Lerp(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 result;
	//Vector3 a = v2 - v1;
	//Vector3 b = t * a;
	//result = v1 + b;

	result = v1 + (t * (v2 - v1));

	return result;
};