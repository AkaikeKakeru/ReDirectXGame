#pragma once

#include "Vector3.h"
#include "Vector4.h"
#include "PrimitiveDrawer.h"

struct GridLine
{
	//頂点座標
	Vector3 gridCoordinate[44] = {};
	//リスト
	int gridList[22][2] = {};
	//軸の色
	Vector4 xColor = {};
	Vector4 zColor = {};
	Vector4 lineColor[22] = {};

	//ゲームシーンで起動させる
	void Intialize();
	void UpDate();
	void Draw();

	//Intializeで起動させる
	void SetCoordinate();
	void SetList();
	void SetColor();
};
