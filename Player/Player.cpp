#include "Player.h"
#include <cassert>

Player::Player()
{
}

Player::~Player()
{
}

///<summary>
///������
///<summary>
void Player::Intialize(Model* model, uint32_t textureHandle) {

	//null�`�F�b�N
	assert(model);

	//�����o�ϐ��ɋL�^
	this->model_ = model;
	this->textureHandle_ = textureHandle;

	//�V���O���g���C���X�^���X
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
}

void Player::Move()
{
	//�L�����N�^�[�ړ�����
	//���E�_
	const float kMoveLimitX = 35.5f;
	const float kMoveLimitY = 18.5f;

	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0,0,0 };

	if (input_->PushKey(DIK_LEFT)) {
		move.x -= 0.2f;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		move.x += 0.2f;
	}

	if (input_->PushKey(DIK_UP)) {
		move.y += 0.2f;
	}
	if (input_->PushKey(DIK_DOWN)) {
		move.y -= 0.2f;
	}

	//�ړ��x�N�g�������Z
	worldTransform_.translation_ += move;

	//�ړ����̌��E�_�𒴂����Ȃ�����
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);

	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);
}

void Player::Rotate()
{
#pragma region ���g��]
	//�㔼�g��]

	if (input_->PushKey(DIK_U))
	{
		worldTransform_.rotation_.y -= 0.05f;
	}
	else if (input_->PushKey(DIK_I))
	{
		worldTransform_.rotation_.y += 0.05f;
	}


	//�����g��]

	if (input_->PushKey(DIK_J))
	{
		worldTransform_.rotation_.y -= 0.05f;
	}
	else if (input_->PushKey(DIK_K))
	{
		worldTransform_.rotation_.y += 0.05f;
	}

#pragma endregion
}

void Player::Transfer(WorldTransform worldTransform,MyMatrix myMatrix)
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

///<summary>
///�X�V
///<summary>
void Player::Update() {
	Move();
	Rotate();
	Transfer(worldTransform_,myMatrix_);

	//�f�o�b�O�p�\��
	debugText_->SetPos(50, 150);
	debugText_->Printf(
		"Root:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
}

///<summary>
///�`��
///<summary>
void Player::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}