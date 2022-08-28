#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

/// <summary>
/// �V��
/// </summary>
class Skydome {
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	void Initialize(Model* model);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(const ViewProjection& viewProjection);
private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	////�r���[�v���W�F�N�V����
	//ViewProjection* viewProjection_;
	//���f��
	Model* model_ = nullptr;
};