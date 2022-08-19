#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "MyMatrix.h"
#include "DebugText.h"

//�s���t�F�[�Y
enum class Phase {
	Approach, //�ڋ߂���
	Leave, //���E����
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
	void ApproachMove() ;
	void LeaveMove() ;

private:
	
	//�t�F�[�Y
	Phase phase_ =  Phase::Approach;

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
	Vector3 approachVelocity_;
	Vector3 leaveVelocity_;
};

