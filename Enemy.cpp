#include "Enemy.h"
#include <cassert>

Enemy::Enemy() {
	state_ = new EnemyStateApproach();
}
Enemy::~Enemy() {
	delete state_;
}

/// <summary>
/// 初期化
/// </summary>
void Enemy::Intialize(
	Model* model,
	const Vector3& position,
	const Vector3& approachVelocity,
	const Vector3& leaveVelocity) {
	//nullチェック
	assert(model);

	//メンバ変数に記録
	model_ = model;
	textureHandle_ = TextureManager::Load("enemy.png");

	//シングルトンインスタンス
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	//受けとった速度をメンバ変数に代入
	approachVelocity_ = approachVelocity;
	leaveVelocity_ = leaveVelocity;

	//接近フェーズ初期化
	IntializeApproachPhase();
};

/// <summary>
/// 更新
/// </summary>
void Enemy::Update() {

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
		});

	//完了フラグの立ったタイマーを削除
	timedCalls_.remove_if([](std::unique_ptr<TimedCall>& timedCall) {
		return timedCall->IsFinished();
		});

	//タイマー更新
	for (std::unique_ptr<TimedCall>& timedCall : timedCalls_) {
		timedCall->Update();
	}

	//現在のフェーズで移動処理を行う
	state_->Update(this);

	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}

	//キャラデバッグ用表示
	debugText_->SetPos(50, 210);
	debugText_->Printf(
		"EneTrans:(%f,%f,%f)",
		worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);
};

/// <summary>
/// 描画
/// </summary>
void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//弾描画
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
};

/// <summary>
/// 移動
/// </summary>
void Enemy::Move(Vector3 position, Vector3 velocity) {
	//移動(ベクトルを加算)
	position += velocity;

	//位置座標を上書き
	SetPosition(position);

	//ワールド座標の更新と転送
	Transfer(worldTransform_, myMatrix_);
};

/// <summary>
/// 発射
/// </summary>
void Enemy::Fire() {
	const float kBulletSpeed = -0.5f;
	Vector3 bulletVelocity_ = Vector3(0, 0, kBulletSpeed);

	//弾生成
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();

	newBullet->Intialize(model_,worldTransform_.translation_, bulletVelocity_);

	//弾を登録
	bullets_.push_back(std::move(newBullet));
};

/// <summary>
/// 発射とタイマ－のリセット
/// </summary>
void Enemy::FireAndResetTimer() {
	//弾を発射
	Fire();

	//発射タイマーをセット
	timedCalls_.push_back(std::make_unique<TimedCall>(std::bind(&Enemy::FireAndResetTimer, this), kFireTimer_));
};

/// <summary>
/// 転送
/// </summary>
void Enemy::Transfer(WorldTransform worldTransform, MyMatrix myMatrix) {
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
/// ステート変更
/// </summary>
void Enemy::ChangeState(BaseEnemyState* newState) {
	delete state_;
	state_ = newState;
};

//接近フェーズ
void Enemy::IntializeApproachPhase() {
	//発射タイマーを初期化
	kFireTimer_ = kFireInterval;
};

void Enemy::UpdateApproachPhase(){

	//発射タイマーカウントダウン
	kFireTimer_--;

	//指定時間に達した
	if (kFireTimer_ <= 0) {
		//弾を発射
		//Fire();
		//発射タイマーを初期化
		//kFireTimer_ = kFireInterval;

		FireAndResetTimer();
	}
};

/// <summary>
/// セッター
/// </summary>
void Enemy::SetPosition(Vector3 position) {
	this->worldTransform_.translation_ = position;
};

void Enemy::SetFireTimer(int32_t timer) {
	this->kFireTimer_ = timer;
};