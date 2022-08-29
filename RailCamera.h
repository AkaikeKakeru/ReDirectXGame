#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"

#include "MyMatrix.h"
#include "MathUtility.h"

#include "DebugText.h"

class RailCamera
{
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(const Vector3& position,
		const Vector3& viewAngle);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	///<summary>
	///�]��
	///<summary>
	void Transfer();

	Vector3 t(const Vector3& v, const Matrix4& m);

	const ViewProjection& GetViewProjection() { return this->viewProjection_; };
private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;

	//���[���h�ϊ��s��
	MyMatrix myMatrix_;

	//�f�o�b�O�e�L�X�g
	DebugText* debugText_ = nullptr;
};