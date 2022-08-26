#include "PlayerBullet.h"
#include <cassert>

PlayerBullet::PlayerBullet() {}
PlayerBullet::~PlayerBullet(){}

///<summary>
///初期化
///<summary>
/// <param name="model">モデル</param>
/// <param name="position">初期座標</param>
void PlayerBullet::Intialize(Model* model, const Vector3& position, const Vector3& velocity){
	//nullチェック
	assert(model);

	//メンバ変数に記録
	model_ = model;
	textureHandle_ = TextureManager::Load("player_bullet.png");

	//ワールド変換の初期化
	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	//受けとった速度をメンバ変数に代入
	velocity_ = velocity;
}

///<summary>
///更新
///<summary>
void PlayerBullet::Update(){
	//座標を移動させる(1フレーム分の移動量を足しこむ)
	worldTransform_.translation_ += velocity_;

	Transfer(worldTransform_,myMatrix_);

	//時間経過で朽ちる
	if (--deathtimer_ <= 0) {
		isDead_ = true;
	}
}

///<summary>
///描画
///<summary>
/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
void PlayerBullet::Draw(const ViewProjection& viewProjection){
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void PlayerBullet::Transfer(WorldTransform worldTransform,MyMatrix myMatrix)
{
	//matrix
	static Matrix4 scale;
	static  Matrix4 rot;
	static  Matrix4 translation;

	//行列更新
	scale = myMatrix.MatrixScale(worldTransform.scale_);
	rot = myMatrix.MatrixRotationZ(worldTransform.rotation_);
	rot *= myMatrix.MatrixRotationX(worldTransform.rotation_);
	rot *= myMatrix.MatrixRotationY(worldTransform.rotation_);
	translation = myMatrix.MatrixTranslation(worldTransform.translation_);

	worldTransform.matWorld_ = myMatrix.MatrixWorld(scale, rot, translation);

	//転送
	worldTransform.TransferMatrix();
}

/// <summary>
/// コールバック
/// </summary>
//衝突したら呼び出される
void PlayerBullet::OnCollision() {
	isDead_ = true;
};