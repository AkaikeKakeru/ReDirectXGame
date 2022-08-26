#include "PlayerBullet.h"
#include <cassert>

PlayerBullet::PlayerBullet() {}
PlayerBullet::~PlayerBullet(){}

///<summary>
///������
///<summary>
/// <param name="model">���f��</param>
/// <param name="position">�������W</param>
void PlayerBullet::Intialize(Model* model, const Vector3& position, const Vector3& velocity){
	//null�`�F�b�N
	assert(model);

	//�����o�ϐ��ɋL�^
	model_ = model;
	textureHandle_ = TextureManager::Load("player_bullet.png");

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	//�󂯂Ƃ������x�������o�ϐ��ɑ��
	velocity_ = velocity;
}

///<summary>
///�X�V
///<summary>
void PlayerBullet::Update(){
	//���W���ړ�������(1�t���[�����̈ړ��ʂ𑫂�����)
	worldTransform_.translation_ += velocity_;

	Transfer(worldTransform_,myMatrix_);

	//���Ԍo�߂ŋ�����
	if (--deathtimer_ <= 0) {
		isDead_ = true;
	}
}

///<summary>
///�`��
///<summary>
/// <param name="viewProjection">�r���[�v���W�F�N�V����(�Q�Ɠn��)</param>
void PlayerBullet::Draw(const ViewProjection& viewProjection){
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void PlayerBullet::Transfer(WorldTransform worldTransform,MyMatrix myMatrix)
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

/// <summary>
/// �R�[���o�b�N
/// </summary>
//�Փ˂�����Ăяo�����
void PlayerBullet::OnCollision() {
	isDead_ = true;
};