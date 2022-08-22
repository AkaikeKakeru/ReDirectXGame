#include "Enemy.h"
#include <cassert>

Enemy::Enemy(){
	state_ = new EnemyStateApproach();
}
Enemy::~Enemy(){
	delete state_;
}



/// <summary>
/// ������
/// </summary>
/// <param name="model">���f��</param>
/// <param name="position">�������W</param>
/// /// <param name="velocity">���x</param>
void Enemy::Intialize(Model* model, const Vector3& position, const Vector3& approachVelocity,const Vector3& leaveVelocity) {
	//null�`�F�b�N
	assert(model);

	//�����o�ϐ��ɋL�^
	model_ = model;
	textureHandle_ = TextureManager::Load("player.png");

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
void Enemy:: Update() {

	//�����o�֐��|�C���^�ɓ����Ă���֐����Ăяo��
	(this->*spPhaseTable[static_cast <size_t>(phase_)])();

	Transfer(worldTransform_,myMatrix_);

	//�L�����f�o�b�O�p�\��
	debugText_->SetPos(50, 210);
	debugText_->Printf(
		"EneTrans:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);

};

/// <summary>
/// �`��
/// </summary>
/// <param name="viewProjection">�r���[�v���W�F�N�V����(�Q�Ɠn��)</param>
void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
};

/// <summary>
/// �ړ�
/// </summary>
void Enemy::Move(Vector3 position,Vector3 velocity) {
	position += velocity;
};

void Enemy::ApproachMove() {
	////�ړ�(�x�N�g�������Z)
	//worldTransform_.translation_ += approachVelocity_;

	////�K��̈ʒu�ɓ��B�����痣�E
	//if (worldTransform_.translation_.z < 0.0f) {
	//	phase_ = Phase::Leave;
	//}
	state_->Update();
};

void Enemy::LeaveMove() {
	////�ړ�(�x�N�g�������Z)
	//worldTransform_.translation_ += leaveVelocity_;

	state_->Update();
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

/// <summary>
/// �X�e�[�g�ύX
/// </summary>
void Enemy::ChangeState(BaseEnemyState*)
{

};

Vector3 Enemy::GetPosition() {
	return this->worldTransform_.translation_;
};

//�t�F�[�Y�̊֐��e�[�u��
void (Enemy::* Enemy::spPhaseTable[])() = {
	&Enemy::ApproachMove, //�ڋ�
	&Enemy::LeaveMove //���E
};


//----�q�N���X�̎���
void BaseEnemyState(){

}

void EnemyStateApproach::Update() {
	//�ړ�(�x�N�g�������Z)
	Enemy::Move();
//	//�K��̈ʒu�ɓ��B�����痣�E
//	if (worldTransform_.translation_.z < 0.0f) {
//		phase_ = Leave;
//	}
}
