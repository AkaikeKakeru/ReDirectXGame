#include "PlayerBullet.h"
#include <cassert>

PlayerBullet::PlayerBullet() {}
PlayerBullet::~PlayerBullet(){}

///<summary>
///������
///<summary>
/// <param name="model">���f��</param>
/// <param name="position">�������W</param>
/// <param name="textureHandle">�e�N�X�`���n���h��</param>
void PlayerBullet::Intialize(Model* model, const Vector3& position/*uint32_t textureHandle*/){
	//null�`�F�b�N
	assert(model);

	//�����o�ϐ��ɋL�^
	model_ = model;
	textureHandle_ = TextureManager::Load("player_bullet.png");
	//textureHandle_ = textureHandle;

	////�V���O���g���C���X�^���X
	//input_ = Input::GetInstance();
	//debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	worldTransform_.translation_ = position;
}

///<summary>
///�X�V
///<summary>
void PlayerBullet::Update(){
	Transfer(worldTransform_,myMatrix_);

	//�f�o�b�O�p�\��
	debugText_->SetPos(50, 150);
	debugText_->Printf(
		"Root:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);

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