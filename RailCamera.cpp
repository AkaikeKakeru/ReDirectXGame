#include "RailCamera.h"

// 初期化
void RailCamera::Initialize(const Vector3& position,const Vector3& viewAngle) {
	//ワールドトランスフォームの初期設定
	worldTransform_.scale_ = { 1,1,1 };

	worldTransform_.translation_ = position;

	worldTransform_.rotation_ = viewAngle;

	//ビュープロジェクションの初期化
	viewProjection_.nearZ = 15.0f;

	viewProjection_.farZ = 200.0f;

	viewProjection_.Initialize();

	debugText_ = DebugText::GetInstance();
};

// 更新
void RailCamera::Update(){
	//worldTransform_.translation_.z -= 0.1f;
	//worldTransform_.rotation_.z += 0.001f;
	Transfer();

	viewProjection_.eye = worldTransform_.translation_;
	//ワールド前方ベクトル
	Vector3 forward(0, 0, 1);
	//レールカメラの回転を反映
	forward = -Vector3Transform(forward,worldTransform_.matWorld_);
	
	//視点から前方に適当な距離進んだ位置が注視点
	viewProjection_.target = viewProjection_.eye + forward;

	//	ワールド上方ベクトル
	Vector3 up(0, 1, 0);
	//レールカメラの回転を反映(レールカメラの上方ベクトル)
	viewProjection_.up = Vector3Transform(up,worldTransform_.matWorld_);

	//ビュープロジェクションを更新
	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();

	//デバッグ用表示
	debugText_->SetPos(50, 250);
	debugText_->Printf(
		"eye:(%f,%f,%f)",
		viewProjection_.eye.x,
		viewProjection_.eye.y,
		viewProjection_.eye.z);
};

///<summary>
///転送
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

	//worldTransform_.TransferMatrix();
}

// 座標変換 (ベクトルと行列の掛け算をする)
Vector3 RailCamera::t(const Vector3& v, const Matrix4& m)
{
	float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];

	Vector3 result
	{
		(v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0]) / w,
		(v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1]) / w,
		(v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2]) / w,
	};

	return result;
}