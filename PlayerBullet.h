#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "Input.h"
#include "MyMatrix.h"

///<summary>
///���L�����̒e
/// </summary>
class PlayerBullet {
public:
	PlayerBullet();
	~PlayerBullet();

	///<summary>
	///������
	///<summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�������W</param>
	/// <param name="textureHandle">�e�N�X�`���n���h��</param>
	void Intialize(Model* mode, const Vector3& position/*uint32_t textureHandle*/);

	///<summary>
	///�X�V
	///<summary>
	void Update();

	///<summary>
	///�`��
	///<summary>
	/// <param name="viewProjection">�r���[�v���W�F�N�V����(�Q�Ɠn��)</param>
	void Draw(const ViewProjection& viewProjection);

	void Transfer(WorldTransform worldTransform,MyMatrix myMatrix);

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//�C���v�b�g
	Input* input_ = nullptr;
	//�f�o�b�O�e�L�X�g
	DebugText* debugText_ = nullptr;
	////�r���[�v���W�F�N�V����
	//ViewProjection viewProjection_;
	//���[���h�ϊ��s��
	MyMatrix myMatrix_;
};