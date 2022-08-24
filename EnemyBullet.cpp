#include"EnemyBullet.h"
#include <cassert>

EnemyBullet::EnemyBullet(){}
EnemyBullet::~EnemyBullet(){}

///<summary>
///初期化
///<summary>
/// <param name="model">モデル</param>
/// <param name="position">初期座標</param>
void EnemyBullet::Intialize(
	Model* model,
	const Vector3& position) {
	//nullチェック
	assert(model);

	//メンバ変数に記録
	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("enemy_bullet.png");

	//ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}

///<summary>
///更新
///<summary>
void EnemyBullet::Update(){
	//matrix
	static Matrix4 scale;
	static  Matrix4 rot;
	static  Matrix4 translation;

	//行列更新
	scale = myMatrix_.MatrixScale(worldTransform_.scale_);
	rot = myMatrix_.MatrixRotationZ(worldTransform_.rotation_);
	rot *= myMatrix_.MatrixRotationX(worldTransform_.rotation_);
	rot *= myMatrix_.MatrixRotationY(worldTransform_.rotation_);
	translation = myMatrix_.MatrixTranslation(worldTransform_.translation_);

	worldTransform_.matWorld_ = myMatrix_.MatrixWorld(scale, rot, translation);

	//転送
	worldTransform_.TransferMatrix();
}

///<summary>
///描画
///<summary>
/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
void EnemyBullet::Draw(const ViewProjection& viewProjection){
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}