#pragma once

#include "Model.h"
#include "WorldTransform.h"
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
	void Intialize(Model* mode, const Vector3& position);

	///<summary>
	///�X�V
	///<summary>
	void Update();

	///<summary>
	///�`��
	///<summary>
	/// <param name="viewProjection">�r���[�v���W�F�N�V����(�Q�Ɠn��)</param>
	void Draw(const ViewProjection& viewProjection);

	///<summary>
	///�]��
	///<summary>
	void Transfer(WorldTransform worldTransform,MyMatrix myMatrix);

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//���[���h�ϊ��s��
	MyMatrix myMatrix_;
};