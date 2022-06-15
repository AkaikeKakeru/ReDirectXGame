#pragma once

#include "Vector3.h"
#include "Vector4.h"
#include "PrimitiveDrawer.h"

struct GridLine
{
	//���_���W
	Vector3 gridCoordinate[44] = {};
	//���X�g
	int gridList[22][2] = {};
	//���̐F
	Vector4 xColor = {};
	Vector4 zColor = {};
	Vector4 lineColor[22] = {};

	//�Q�[���V�[���ŋN��������
	void Intialize();
	void UpDate();
	void Draw();

	//Intialize�ŋN��������
	void SetCoordinate();
	void SetList();
	void SetColor();
};
