#include "Enemy.h"
#include <cassert>

Enemy::Enemy(){}
Enemy::~Enemy(){}

/// <summary>
/// 初期化
/// </summary>
/// <param name="model">モデル</param>
/// <param name="position">初期座標</param>
/// /// <param name="velocity">速度</param>
void Enemy::Intialize(Model* model/*, const Vector3& position,*//* const Vector3& velocity*/) {
	//nullチェック
	assert(model);

	//メンバ変数に記録
	model_ = model;
	textureHandle_ = TextureManager::Load("player.png");

	//ワールド変換の初期化
	worldTransform_.Initialize();

	//worldTransform_.translation_ = position;

};

/// <summary>
/// 更新
/// </summary>
void Enemy:: Update() {
	Transfer(worldTransform_,myMatrix_);
};

/// <summary>
/// 描画
/// </summary>
/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
};

void Enemy::Transfer(WorldTransform worldTransform,MyMatrix myMatrix)
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