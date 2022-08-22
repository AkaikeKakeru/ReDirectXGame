#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "MyMatrix.h"
#include "DebugText.h"
#include "Input.h"

//�s���t�F�[�Y
enum class Phase {
	Approach, //�ڋ߂���
	Leave, //���E����
};

class Enemy; //Enemy�N���X�O���錾

class BaseEnemyState 
{
protected:
	Enemy* enemy_;
public:
	 void Update();
};

class EnemyStateApproach : public BaseEnemyState
{
public:
	void Update();
};

class EnemyStateLeave : public BaseEnemyState
{
public:
	void Update();
};


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
	/// <param name="velocity">���x</param>
	void Intialize(Model* model, const Vector3& position, const Vector3& ApproachVelocity,const Vector3& LeaveVelocity);

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
	void Transfer(WorldTransform worldTransform,MyMatrix myMatrix);

	/// <summary>
	/// �ړ�
	/// </summary>
	void Move(Vector3 position,Vector3 velocity);
	void ApproachMove() ;
	void LeaveMove() ;

	/// <summary>
	/// �X�e�[�g�ύX
	/// </summary>
	void ChangeState(BaseEnemyState*);

	/// <summary>
	/// ���W�̃Q�b�^�[
	/// </summary>
	Vector3 GetPosition();

private:
	BaseEnemyState* state_;

	//�����o�֐��|�C���^�̃e�[�u��
	static void (Enemy::* spPhaseTable[])();

	//�t�F�[�Y
	Phase phase_ =  Phase::Approach;
	
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
};

