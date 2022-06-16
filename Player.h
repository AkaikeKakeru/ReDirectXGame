#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "Input.h"
#include "MyMatrix.h"

///<summary>
///���L����
///<summary>
class Player
{
public:
	Player();
	~Player();

	///<summary>
	///������
	///<summary>
	/// <param name="model">���f��</param>
	/// <param name="model">�e�N�X�`���n���h��</param>
	void Intialize(Model*mode,uint32_t textureHandle);

	///<summary>
	///�X�V
	///<summary>
	void Update();

	///<summary>
	///�`��
	///<summary>
	void Draw();

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	MyMatrix myMatrix_;
};