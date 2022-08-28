#include "Skydome.h"
#include <cassert>

// 初期化
void Skydome::Initialize(Model* model) {
	//nullチェック
	assert(model);

	//メンバ変数に記録
	model_ = model;

	//ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.scale_ = Vector3(100.0f,100.0f,100.0f);
	worldTransform_.matWorld_.m[0][0] = worldTransform_.scale_.x;
	worldTransform_.matWorld_.m[1][1] = worldTransform_.scale_.y;
	worldTransform_.matWorld_.m[2][2] = worldTransform_.scale_.z;
	worldTransform_.TransferMatrix();

};

// 毎フレーム処理
void Skydome::Update(){

};

// 描画
void Skydome::Draw(const ViewProjection& viewProjection){
	model_->Draw(worldTransform_, viewProjection);
};