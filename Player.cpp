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
}

///<summary>
///�`��
///<summary>
void Player::Draw(){
}