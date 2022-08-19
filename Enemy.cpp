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
void Enemy::Intialize(Model* model, const Vector3& position, const Vector3& approachVelocity,const Vector3& leaveVelocity) {
	//nullチェック
	assert(model);

	//メンバ変数に記録
	model_ = model;
	textureHandle_ = TextureManager::Load("player.png");

	//シングルトンインスタンス
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	//受けとった速度をメンバ変数に代入
	approachVelocity_ = approachVelocity;
	leaveVelocity_ = leaveVelocity;

	
};

/// <summary>
/// 更新
/// </summary>
void Enemy:: Update() {

	//メンバ関数ポインタに入っている関数を呼び出す
	(this->*spPhaseTable[static_cast <size_t>(phase_)])();

	Transfer(worldTransform_,myMatrix_);

	//キャラデバッグ用表示
	debugText_->SetPos(50, 210);
	debugText_->Printf(
		"EneTrans:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);

};

/// <summary>
/// 描画
/// </summary>
/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
};

/// <summary>
/// 移動
/// </summary>
void Enemy::ApproachMove() {
	//移動(ベクトルを加算)
	worldTransform_.translation_ += approachVelocity_;

	//規定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
};

void Enemy::LeaveMove() {
	//移動(ベクトルを加算)
	worldTransform_.translation_ += leaveVelocity_;
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

//フェーズの関数テーブル
void (Enemy::* Enemy::spPhaseTable[])() = {
	&Enemy::ApproachMove, //接近
	&Enemy::LeaveMove //離脱
};