#include "Enemy.h"
#include <cassert>

Enemy::Enemy() {
	state_ = new EnemyStateApproach();
}
Enemy::~Enemy() {
	delete state_;
}

/// <summary>
/// ������
/// </summary>
void Enemy::Intialize(
	Model* model,
	const Vector3& position,
	const Vector3& approachVelocity,
	const Vector3& leaveVelocity) {
	//null�`�F�b�N
	assert(model);

	//�����o�ϐ��ɋL�^
	model_ = model;
	textureHandle_ = TextureManager::Load("enemy.png");

	//�V���O���g���C���X�^���X
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	//�󂯂Ƃ������x�������o�ϐ��ɑ��
	approachVelocity_ = approachVelocity;
	leaveVelocity_ = leaveVelocity;
};

/// <summary>
/// �X�V
/// </summary>
void Enemy::Update() {

	//���݂̃t�F�[�Y�ňړ��������s��
	state_->Update(this);

	//���[���h���W�̍X�V�Ɠ]��
	Transfer(worldTransform_, myMatrix_);

	//�U��
	Attack();

	//�e�X�V
	if (bullet_) {
		bullet_->Update();
	}

	//�L�����f�o�b�O�p�\��
	debugText_->SetPos(50, 210);
	debugText_->Printf(
		"EneTrans:(%f,%f,%f)",
		worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);
};

/// <summary>
/// �`��
/// </summary>
void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//�e�`��
	if (bullet_) {
		bullet_->Draw(viewProjection);
	}
};

/// <summary>
/// �ړ�
/// </summary>
void Enemy::Move(Vector3 position, Vector3 velocity) {
	//�ړ�(�x�N�g�������Z)
	position += velocity;

	//�ʒu���W���㏑��
	SetPosition(position);
};

void Enemy::Attack() {
	if(isFire_ != true){
		Fire();
		isFire_ = true;
	}
};

void Enemy::Fire() {
	const float kBulletSpeed = -0.5f;
	Vector3 bulletVelocity_ = Vector3(0, 0, kBulletSpeed);

	//�e����
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Intialize(model_, worldTransform_.translation_,bulletVelocity_);
	
	//�e��o�^
	bullet_.reset(newBullet);
};

void Enemy::Transfer(WorldTransform worldTransform, MyMatrix myMatrix) {
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
/// �X�e�[�g�ύX
/// </summary>
void Enemy::ChangeState(BaseEnemyState* newState) {
	delete state_;
	state_ = newState;
};

/// <summary>
/// �Z�b�^�[
/// </summary>
void Enemy::SetPosition(Vector3 position) {
	this->worldTransform_.translation_ = position;
};