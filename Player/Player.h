#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "Input.h"

#include "PlayerBullet.h"
#include <memory>
#include <List>
#include "MyMatrix.h"

#include "Collider.h"

///<summary>
///���L����
///<summary>
class Player:public Collider
{
public:
	Player();
	~Player();

	///<summary>
	///������
	///<summary>
	/// <param name="model">���f��</param>
	/// <param name="textureHandle">�e�N�X�`���n���h��</param>
	void Intialize(Model* model/*, uint32_t textureHandle*/);

	///<summary>
	///�X�V
	///<summary>
	void Update();

	///<summary>
	///�`��
	///<summary>
	/// <param name="viewProjection">�r���[�v���W�F�N�V����(�Q�Ɠn��)</param>
	void Draw(const ViewProjection&  viewProjection);

	///<summary>
	///�ړ�
	///<summary>
	void Move();

	///<summary>
	///����
	///<summary>
	void Rotate();

	///<summary>
	///�]��
	///<summary>
	void Transfer(/*MyMatrix myMatrix*/);

	///<summary>
	///�U��
	///<summary>
	void Attack();

	/// <summary>
	/// �Q�b�^�[
	/// </summary>
	//���[���h���W���擾
	//�Փˎ��ɌĂ΂��֐�
	Vector3 GetWorldPosition() override;
	//�e���X�g���擾
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullet() {
		return bullets_;}

	/// <summary>
	/// �R�[���o�b�N
	/// </summary>
	//�Փ˂�����Ăяo�����
	void OnCollision() override;

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

	//�e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
};