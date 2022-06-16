#include <Player.h>
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
void Player::Intialize(Model*model,uint32_t textureHandle){

	//�܂���null����Ȃ����???
	assert(model);

	//�����o�ϐ��ɋL�^
	this->model_ = model;
	this->textureHandle_ = textureHandle;

	//�A�C���O���g���C���X�^���X
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
}

///<summary>
///�X�V
///<summary>
void Player::Update(){
	//�L�����N�^�[�ړ�����
	{
		//�L�����N�^�[�̈ړ��x�N�g��
		Vector3 move = { 0,0,0 };

		if (input_->PushKey(DIK_LEFT)) {
			move.x -= 0.2f;
		}
		if (input_->PushKey(DIK_RIGHT)) {
			move.x += 0.2f;
		}

		if (input_->PushKey(DIK_UP)) {
			move.y -= 0.2f;
		}
		if (input_->PushKey(DIK_DOWN)) {
			move.y += 0.2f;
		}

		worldTransform_.translation_ += move;

		Matrix4 scale = myMatrix_.MatrixScale(worldTransform_.scale_);
		Matrix4 rot = myMatrix_.MatrixRotationZ(worldTransform_.rotation_);
		rot *= myMatrix_.MatrixRotationX(worldTransform_.rotation_);
		rot *= myMatrix_.MatrixRotationY(worldTransform_.rotation_);
		Matrix4 translation = myMatrix_.MatrixTranslation(worldTransform_.translation_);

		worldTransform_.matWorld_ = myMatrix_.MatrixWorld(scale, rot, translation);

		worldTransform_.TransferMatrix();

		//�f�o�b�O�p�\��
		debugText_->SetPos(50, 150);
		debugText_->Printf(
			"Root:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
	}

}

///<summary>
///�`��
///<summary>
void Player::Draw(){
}