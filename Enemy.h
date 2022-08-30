#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "MyMatrix.h"
#include "DebugText.h"
#include "Input.h"

#include "EnemyBullet.h"
#include <memory>
#include <List>

#include "BaseEnemyState.h"
#include "EnemyApproach.h"
#include "EnemyLeave.h"

#include "TimedCall.h"
#include "Collider.h"

#include "GameScene.h"
#include "Player.h"

//�Q�[���V�[���O���錾
class GameScene;

//���@�N���X�̑O���錾
class Player;

/// <summary>
/// �G
/// </summary>
class Enemy:public Collider
{
public:
	Enemy();
	~Enemy();
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="modelBullet">�e���f��</param> 
	/// <param name="position">�������W</param>
	///  <param name="approachVelocity">�ڋߑ��x</param>
	/// <param name="leaveVelocity">���E���x</param>
	void Intialize(
		Model* model,
		Model* modelBullet,
		const Vector3& position,
		const Vector3& approachVelocity,
		const Vector3& leaveVelocity);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="viewProjection">�r���[�v���W�F�N�V����(�Q�Ɠn��)</param>
	void Draw(const ViewProjection& viewProjection);

	///<summary>
	///�]��
	///<summary>
	void Transfer();

	/// <summary>
	/// �ړ�
	/// </summary>
	/// <param name="position">�������W</param>
	/// <param name="velocity">���x</param> 
	void Move(Vector3 position,Vector3 velocity);

	/// <summary>
	/// �e����
	/// </summary>
	void Fire();

	/// <summary>
	/// �e�̃^�C�}�[��S�폜
	/// </summary>
	void TimerClear();
		
	/// <summary>
	/// �X�e�[�g�ύX
	/// </summary>
	/// <param name="newState">�V�������</param>  
	void ChangeState(BaseEnemyState* newState);

	/// <summary>
	/// �Q�b�^�[
	/// </summary>
	//�Փˎ��ɌĂ΂��֐�
	Vector3 GetWorldPosition() override;
	//���[���h���W���擾
	Vector3 GetPosition() {
		return this->worldTransform_.translation_;};
	////�e���X�g���擾
	//const std::list<std::unique_ptr<EnemyBullet>>& GetBullet() {
	//	return bullets_;}

	float GetRadius() override {
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

	Vector3 GetApproachSpeed(){
		return this->approachVelocity_;}

	Vector3 GetLeaveSpeed(){
		return this->leaveVelocity_;}

	/// <summary>
	/// �Z�b�^�[
	/// </summary> 
	void SetPosition(Vector3 position);
	void SetIsFire(bool isFire);

	void SetGameScene(GameScene* gameScene) {this-> gameScene_ = gameScene; }

	void SetPlayer(Player* player) { this->player_ = player; }

	//�Փˑ���(����)��ݒ�
	void SetCollisionAttribute (uint32_t collisionAttribute) { this->collisionAttribute_ = collisionAttribute; };
	//�Փ˃}�X�N(����)��ݒ�
	void SetCollisionMask (uint32_t collisionMask) { this->collisionMask_ = collisionMask; };


	bool IsDead() const { return isDead_; }

	/// <summary>
	/// �R�[���o�b�N
	/// </summary>
	/// �e�𔭎˂��A�^�C�}�[�����Z�b�g����
	void FireAndResetTimer();
	//�Փ˂�����Ăяo�����
	void OnCollision() override;

public://�ڋ߃t�F�[�Y
	//�ڋ߃t�F�[�Y������
	void IntializeApproachPhase();
public:
	//���ˊԊu<frm>
	static const int kFireInterval = 60;

private:
	//���
	BaseEnemyState* state_ = nullptr;
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	Model* modelBullet_ = nullptr;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	//�C���v�b�g
	Input* input_ = nullptr;
	//�f�o�b�O�e�L�X�g
	DebugText* debugText_ = nullptr;

	//���[���h�ϊ��s��
	MyMatrix myMatrix_;

	//���x
	Vector3 approachVelocity_;
	Vector3 leaveVelocity_;

	//���a
	float kRadius_ = 0.5f;

	//�Փˑ���(����)
	uint32_t collisionAttribute_ = 0xffffffff;
	//�Փ˃}�X�N(����)
	uint32_t collisionMask_ = 0xffffffff;

	////�e
	//std::list<std::unique_ptr<EnemyBullet>> bullets_ ;

	//���������̃��X�g
	std::list<std::unique_ptr<TimedCall>> timedCalls_ ;

	//�f�X�t���O
	bool isDead_ = false;

	//�Q�[���V�[��
	GameScene* gameScene_ = nullptr;

	//���L����
	Player* player_ = nullptr;
};