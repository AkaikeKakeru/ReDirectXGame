#include "Skydome.h"
#include <cassert>

// ������
void Skydome::Initialize(Model* model) {
	//null�`�F�b�N
	assert(model);

	//�����o�ϐ��ɋL�^
	model_ = model;

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
	worldTransform_.scale_ = Vector3(100.0f,100.0f,100.0f);
	worldTransform_.matWorld_.m[0][0] = worldTransform_.scale_.x;
	worldTransform_.matWorld_.m[1][1] = worldTransform_.scale_.y;
	worldTransform_.matWorld_.m[2][2] = worldTransform_.scale_.z;
	worldTransform_.TransferMatrix();

};

// ���t���[������
void Skydome::Update(){

};

// �`��
void Skydome::Draw(const ViewProjection& viewProjection){
	model_->Draw(worldTransform_, viewProjection);
};