#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "MyMatrix.h"

#include "Player.h"

class Player;

class EnemyBullet{
public:
	EnemyBullet();
	~EnemyBullet();

	///<summary>
	///������
	///<summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�������W</param>
	/// <param name="velocity">���x</param>
	void Intialize(Model* model, const Vector3& position, const Vector3& velocity);

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
	void Transfer();

	bool IsDead() const { return isDead_; }

	/// <summary>
	/// �Z�b�^�[
	/// </summary>
	void SetPlayer(Player* player) { player_ = player; }

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//���[���h�ϊ��s��
	MyMatrix myMatrix_;
	//���x
	Vector3 velocity_;

	//���@�|�C���^
	Player* player_ = nullptr;

	//����<frm>
	static const int32_t kLifeTime = 60 * 5;
	//�f�X�^�C�}�[
	int32_t deathtimer_ = kLifeTime;
	//�f�X�t���O
	bool isDead_ = false;
};