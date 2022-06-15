#pragma once

#include "Vector3.h"
#include "Vector4.h"
#include "PrimitiveDrawer.h"

struct GridLine
{
	Vector3 gridCoordinate[44] = {};
	int gridList[22][2] = {};
	Vector4 xColor = {};
	Vector4 zColor = {};
	Vector4 lineColor[22] = {};

	void Intialize();

	void UpDate();

	void Draw();

	void SetCoordinate();
	void SetList();
	void SetColor();
};
