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

	//���a
	kRadius_ = 0.5f;
}

///<summary>
///�X�V
///<summary>
void PlayerBullet::Update(){
	//���W���ړ�������(1�t���[�����̈ړ��ʂ𑫂�����)
	worldTransform_.translation_ += velocity_;

	Transfer();

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

void PlayerBullet::Transfer()
{
	//matrix
	static Matrix4 scale;
	static  Matrix4 rota;
	static  Matrix4 translation;

	scale = myMatrix_.MatrixScale(worldTransform_.scale_);
	rota = myMatrix_.MatrixRotationZ(worldTransform_.rotation_);
	rota *= myMatrix_.MatrixRotationX(worldTransform_.rotation_);
	rota *= myMatrix_.MatrixRotationY(worldTransform_.rotation_);
	translation = myMatrix_.MatrixTranslation(worldTransform_.translation_);

	worldTransform_.matWorld_ = myMatrix_.MatrixWorld(scale, rota, translation);

	//�]��
	worldTransform_.TransferMatrix();
}

/// <summary>
/// �Q�b�^�[
/// </summary>
Vector3 PlayerBullet::GetWorldPosition() {
	//���[���h���W������ϐ�
	Vector3 worldPos;

	//���[���h�s��̕��s�ړ��������擾
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
};

/// <summary>
/// �R�[���o�b�N
/// </summary>
//�Փ˂�����Ăяo�����
void PlayerBullet::OnCollision() {
	isDead_ = true;
};