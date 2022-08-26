#include"EnemyBullet.h"
#include <cassert>

EnemyBullet::EnemyBullet(){}
EnemyBullet::~EnemyBullet(){}

///<summary>
///������
///<summary>
/// <param name="model">���f��</param>
/// <param name="position">�������W</param>
/// <param name="velocity">���x</param>
void EnemyBullet::Intialize(
	Model* model,
	const Vector3& position,
	const Vector3& velocity) {
	//null�`�F�b�N
	assert(model);

	//�����o�ϐ��ɋL�^
	model_ = model;
	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("enemy_bullet.png");

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
	
	//Z�����ɐL�т��`��
	worldTransform_.scale_.x = 0.5f;
	worldTransform_.scale_.y = 0.5f;
	worldTransform_.scale_.z = 3.0f;

	worldTransform_.translation_ = position;

	//���x������
	velocity_ = velocity;

	//--�e�̏����p�x��ς���--//
	//y���܂�����p�x(��y)
	worldTransform_.rotation_.y = std::atan2(velocity_.x,velocity_.z);

	//velocity_����Y������0�ɂ����x�N�g�������߂�
	Vector3 velocityNotY(velocity_.x, 0.0f, velocity_.z);

	//velocityNotY����A3D�x�N�g���Ƃ��Ă̒��������߂āA
	//velocity_�̉��������Ƃ��Ĉ���
	float velocityXZ = Vector3Length(velocityNotY);

	//x���܂�����p�x(��x)
	worldTransform_.rotation_.x = std::atan2(-velocity_.y,velocityXZ);
	//--�e�̏����p�x�ς��B�����܂�--//
}

///<summary>
///�X�V
///<summary>
void EnemyBullet::Update(){
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
void EnemyBullet::Draw(const ViewProjection& viewProjection){
	//���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void EnemyBullet::Transfer()
{
	//matrix
	static Matrix4 scale;
	static  Matrix4 rot;
	static  Matrix4 translation;

	//�s��X�V
	scale = myMatrix_.MatrixScale(worldTransform_.scale_);
	rot = myMatrix_.MatrixRotationZ(worldTransform_.rotation_);
	rot *= myMatrix_.MatrixRotationX(worldTransform_.rotation_);
	rot *= myMatrix_.MatrixRotationY(worldTransform_.rotation_);
	translation = myMatrix_.MatrixTranslation(worldTransform_.translation_);

	worldTransform_.matWorld_ = myMatrix_.MatrixWorld(scale, rot, translation);

	//�]��
	worldTransform_.TransferMatrix();
}