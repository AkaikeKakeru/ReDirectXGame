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

	//�U��
	Attack();
};

/// <summary>
/// �X�V
/// </summary>
void Enemy::Update() {

	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
		});

	//���݂̃t�F�[�Y�ňړ��������s��
	state_->Update(this);

	//���[���h���W�̍X�V�Ɠ]��
	Transfer(worldTransform_, myMatrix_);

	

	//�e�X�V
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
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
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
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
		Fire();
};

void Enemy::Fire() {
	const float kBulletSpeed = -0.2f;
	Vector3 bulletVelocity_ = Vector3(0, 0, kBulletSpeed);

	//�e����
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique< EnemyBullet>();
	newBullet->Intialize(model_, worldTransform_.translation_, bulletVelocity_);

	//�e��o�^
	bullets_.push_back(std::move(newBullet));
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

void Enemy::IntializeApproachPhase() {
	//���˃^�C�}�[��������
	kFireTimer_ = kFireInterval;
}

/// <summary>
/// �Z�b�^�[
/// </summary>
void Enemy::SetPosition(Vector3 position) {
	this->worldTransform_.translation_ = position;
};