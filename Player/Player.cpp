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
void Player::Initialize(Model* model,Model* modelBullet) {
	//null�`�F�b�N
	assert(model);

	//null�`�F�b�N
	assert(modelBullet);

	//�����o�ϐ��ɋL�^
	model_ = model;
	modelBullet_ = modelBullet;
	textureHandle_ = TextureManager::Load("plane/plane_Map.png");

	//�V���O���g���C���X�^���X
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
	worldTransform_.rotation_.y = XMConvertToRadians(180.0f);
	worldTransform_.translation_.z = 40.0f;
	worldTransform3DReticle_.Initialize();

	//�Փˑ�����ݒ�
	SetCollisionAttribute(kCollisionAttributePlayer);
	//�ՓˑΏۂ������̑����ȊO�ɐݒ�
	SetCollisionMask(~(kCollisionAttributePlayer));
};

///<summary>
///�X�V
///<summary>
void Player::Update() {

#ifdef  _DEBUG
#endif //  _DEBUG

	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {

		return bullet->IsDead();
		});

	Rotate();

	Move();

	Update3DReticle();

	Attack();

	//�e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	Transfer();

	//���L�����f�o�b�O�p�\��
	debugText_->SetPos(50, 150);
	debugText_->Printf(
		"PlaTrans:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);

	debugText_->SetPos(50, 180);
	debugText_->Printf(
		"PlaRota:(%f,%f,%f)", worldTransform_.rotation_.x, worldTransform_.rotation_.y, worldTransform_.rotation_.z);

};

///<summary>
///�`��
///<summary>
void Player::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	modelBullet_->Draw(worldTransform3DReticle_, viewProjection);

	//�e�`��
	for(std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

///<summary>
///�ړ�
///<summary>
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

///<summary>
///����
///<summary>
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

///<summary>
///�]��
///<summary>
void Player::Transfer()
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

	//�e�̃��[���h�s����|���Z
	worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;

	//�]��
	worldTransform_.TransferMatrix();
}

///<summary>
///�U��
///<summary>
void Player::Attack(){
	const float kBulletSpeed = -1.0f;
	Vector3 bulletVelocity_ = Vector3(0, 0, kBulletSpeed);

	bulletVelocity_ = Vector3(
		worldTransform3DReticle_.matWorld_.m[3][0] 
			-worldTransform_.matWorld_.m[3][0],
		worldTransform3DReticle_.matWorld_.m[3][1]
			-worldTransform_.matWorld_.m[3][1],
		worldTransform3DReticle_.matWorld_.m[3][2]
			-worldTransform_.matWorld_.m[3][2]
	);

	bulletVelocity_ = Vector3Normalize(bulletVelocity_)
		* kBulletSpeed;

	Vector3 bulletPosition_ = 
		Vector3(
			worldTransform_.matWorld_.m[3][0],
			worldTransform_.matWorld_.m[3][1],
			worldTransform_.matWorld_.m[3][2]
		);

	if (input_->TriggerKey(DIK_SPACE))
	{
		//���x�x�N�g�������@�̌����ɂ��킹��
		bulletVelocity_ = myMatrix_.CrossVector(bulletVelocity_, worldTransform_.matWorld_);

		
		//�e�𐶐����A������
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Intialize(modelBullet_,bulletPosition_,bulletVelocity_);

		//�e��o�^
		//bullet_ = newBullet;
		/*bullets_.reset(newBullet);*/
		bullets_.push_back(std::move(newBullet));
	}
}

void Player::Update3DReticle() {
	//���@����3D���e�B�N���ւ̋���
	const float kDistancePlayerTo3DRethicle = -50.0f;
	//���@����3D���e�B�N���ւ̃I�t�Z�b�g(Z+����)
	Vector3 offset = { 0,0,1.0f };
	//���@�̃��[���h�s��̉�]�𔽉f
	offset = myMatrix_.CrossVector(offset, worldTransform_.matWorld_);
	
	//�x�N�g���̒����𐮂���
	offset = Vector3Normalize(offset) * kDistancePlayerTo3DRethicle;
	//3D���e�B�N���̍��W���w��
	worldTransform3DReticle_.translation_ = worldTransform_.translation_ + offset;


	//matrix
	static Matrix4 scale;
	static  Matrix4 rota;
	static  Matrix4 translation;

	scale = myMatrix_.MatrixScale(worldTransform3DReticle_.scale_);
	rota = myMatrix_.MatrixRotationZ(worldTransform3DReticle_.rotation_);
	rota *= myMatrix_.MatrixRotationX(worldTransform3DReticle_.rotation_);
	rota *= myMatrix_.MatrixRotationY(worldTransform3DReticle_.rotation_);
	translation = myMatrix_.MatrixTranslation(worldTransform3DReticle_.translation_);

	worldTransform3DReticle_.matWorld_ = myMatrix_.MatrixWorld(scale, rota, translation);
	worldTransform3DReticle_.TransferMatrix();
};

Vector3 Player::GetWorldPosition() {
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
void Player::OnCollision() {

};