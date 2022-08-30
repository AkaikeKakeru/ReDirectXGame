#include "RailCamera.h"

// ������
void RailCamera::Initialize(const Vector3& position,const Vector3& viewAngle) {
	//���[���h�g�����X�t�H�[���̏����ݒ�
	worldTransform_.scale_ = { 1,1,1 };

	worldTransform_.translation_ = position;

	worldTransform_.rotation_ = viewAngle;

	//�r���[�v���W�F�N�V�����̏�����
	viewProjection_.nearZ = 15.0f;

	viewProjection_.farZ = 500.0f;

	viewProjection_.Initialize();

	debugText_ = DebugText::GetInstance();
};

// �X�V
void RailCamera::Update(){
	worldTransform_.translation_.z -= 0.1f;
	Transfer();

	viewProjection_.eye = worldTransform_.translation_;
	//���[���h�O���x�N�g��
	Vector3 forward(0, 0, 1);
	//���[���J�����̉�]�𔽉f
	forward = -Vector3Transform(forward,worldTransform_.matWorld_);
	
	//���_����O���ɓK���ȋ����i�񂾈ʒu�������_
	viewProjection_.target = viewProjection_.eye + forward;

	//	���[���h����x�N�g��
	Vector3 up(0, 1, 0);
	//���[���J�����̉�]�𔽉f(���[���J�����̏���x�N�g��)
	viewProjection_.up = Vector3Transform(up,worldTransform_.matWorld_);

	//�r���[�v���W�F�N�V�������X�V
	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();

	//�f�o�b�O�p�\��
	debugText_->SetPos(50, 250);
	debugText_->Printf(
		"eye:(%f,%f,%f)",
		viewProjection_.eye.x,
		viewProjection_.eye.y,
		viewProjection_.eye.z);
};

///<summary>
///�]��
///<summary>
void RailCamera::Transfer()
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
};