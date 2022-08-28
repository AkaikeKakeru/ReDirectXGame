#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "MyMatrix.h"
#include "DebugText.h"

#include "Collider.h"

///<summary>
///���L�����̒e
/// </summary>
class PlayerBullet:public Collider {
public:
	PlayerBullet();
	~PlayerBullet();

	///<summary>
	///������
	///<summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�������W</param>
	/// /// <param name="velocity">���x</param>
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
	/// �R�[���o�b�N
	/// </summary>
	//�Փ˂�����Ăяo�����
	void OnCollision() override;

	/// <summary>
	/// �Q�b�^�[
	/// </summary>
	/// <returns></returns>
	//�Փˎ��ɌĂ΂��֐�
	Vector3 GetWorldPosition() override;
	float GetRadius() override{
		return kRadius_;
	};

	//�Փˑ���(����)���擾
	uint32_t GetCollisionAttribute() override{
		return collisionAttribute_;
	};
	//�Փ˃}�X�N(����)���擾
	uint32_t GetCollisionMask() override{
		return collisionMask_;
	};

	/// <summary>
	/// �Q�b�^�[
	/// </summary>
	//�Փˑ���(����)��ݒ�
	void SetCollisionAttribute (uint32_t collisionAttribute) { this->collisionAttribute_ = collisionAttribute; };
	//�Փ˃}�X�N(����)��ݒ�
	void SetCollisionMask (uint32_t collisionMask) { this->collisionMask_ = collisionMask; };

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//���[���h�ϊ��s��
	MyMatrix myMatrix_;

	//�f�o�b�O�e�L�X�g
	DebugText* debugText_ = nullptr;

	//���x
	Vector3 velocity_;

	//���a
	float kRadius_ = 0.5f;

	//�Փˑ���(����)
	uint32_t collisionAttribute_ = 0xffffffff;
	//�Փ˃}�X�N(����)
	uint32_t collisionMask_ = 0xffffffff;

	//����<frm>
	static const int32_t kLifeTime = 60 * 5;

	//�f�X�^�C�}�[
	int32_t deathtimer_ = kLifeTime;

	//�f�X�t���O
	bool isDead_ = false;
};