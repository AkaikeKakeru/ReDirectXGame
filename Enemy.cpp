#include "Enemy.h"
#include <cassert>

Enemy::Enemy(){}
Enemy::~Enemy(){}

/// <summary>
/// ������
/// </summary>
/// <param name="model">���f��</param>
/// <param name="position">�������W</param>
/// /// <param name="velocity">���x</param>
void Enemy::Intialize(Model* model/*, const Vector3& position,*//* const Vector3& velocity*/) {
	//null�`�F�b�N
	assert(model);

	//�����o�ϐ��ɋL�^
	model_ = model;
	textureHandle_ = TextureManager::Load("player.png");

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	//worldTransform_.translation_ = position;

};

/// <summary>
/// �X�V
/// </summary>
void Enemy:: Update() {
	Transfer(worldTransform_,myMatrix_);
};

/// <summary>
/// �`��
/// </summary>
/// <param name="viewProjection">�r���[�v���W�F�N�V����(�Q�Ɠn��)</param>
void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
};

void Enemy::Transfer(WorldTransform worldTransform,MyMatrix myMatrix)
{
	//matrix
	static Matrix4 scale;
	static  Matrix4 rot;
	static  Matrix4 translation;

	//�s��X�V
	scale = myMatrix.MatrixScale(worldTransform.scale_);
	rot = myMatrix.MatrixRotationZ(worldTransform.rotation_);
	rot *= myMatrix.MatrixRotationX(worldTransform.rotation_);
	rot *= myMatrix.MatrixRotationY(worldTransform.rotation_);
	translation = myMatrix.MatrixTranslation(worldTransform.translation_);

	worldTransform.matWorld_ = myMatrix.MatrixWorld(scale, rot, translation);

	//�]��
	worldTransform.TransferMatrix();
}