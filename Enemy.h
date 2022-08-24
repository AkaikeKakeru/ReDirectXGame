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

/// <summary>
/// �G
/// </summary>
class Enemy
{
public:
	Enemy();
	~Enemy();
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�������W</param>
	///  <param name="approachVelocity">�ڋߑ��x</param>
	/// <param name="leaveVelocity">���E���x</param>
	void Intialize(
		Model* model,
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
	///  <param name="worldTransform">���[���h�g�����X�t�H�[��</param>
	/// <param name="myMatrix">���W�ϊ��̂܂Ƃ�</param>
	void Transfer(WorldTransform worldTransform,MyMatrix myMatrix);

	/// <summary>
	/// �ړ�
	/// </summary>
	/// <param name="position">�������W</param>
	/// <param name="velocity">���x</param> 
	void Move(Vector3 position,Vector3 velocity);

	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

	/// <summary>
	/// �e����
	/// </summary>
	void Fire();

	/// <summary>
	/// �X�e�[�g�ύX
	/// </summary>
	/// <param name="newState">�V�������</param>  
	void ChangeState(BaseEnemyState* newState);

	/// <summary>
	/// �Q�b�^�[
	/// </summary>
	Vector3 GetPosition() {
		return this->worldTransform_.translation_;};

	Vector3 GetApproachSpeed(){
		return this->approachVelocity_;}

	Vector3 GetLeaveSpeed(){
		return this->leaveVelocity_;}

	/// <summary>
	/// �Z�b�^�[
	/// </summary>
	/// <param name="position">���W</param>
	void SetPosition(Vector3 position);

private:
	//���
	BaseEnemyState* state_ = nullptr;
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

	//���[���h�ϊ��s��
	MyMatrix myMatrix_;

	//���x
	Vector3 approachVelocity_;
	Vector3 leaveVelocity_;

	//�e
	std::list<std::unique_ptr<EnemyBullet>> bullets_ ;
};