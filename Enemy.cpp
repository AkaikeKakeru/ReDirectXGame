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
	Model* modelBullet,
	const Vector3& position,
	const Vector3& approachVelocity,
	const Vector3& leaveVelocity) {
	//nullチェック
	assert(model);

	//nullチェック
	assert(modelBullet);

	//メンバ変数に記録
	model_ = model;
	modelBullet_ = modelBullet;
	textureHandle_ = TextureManager::Load("plane/enemy_plane_Map.png");

	//シングルトンインスタンス
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	//受けとった速度をメンバ変数に代入
	approachVelocity_ = approachVelocity;
	leaveVelocity_ = leaveVelocity;

	//衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeEnemy);
	//衝突対象を自分の属性以外に設定
	SetCollisionMask(~(kCollisionAttributeEnemy));

	//接近フェーズ初期化
	IntializeApproachPhase();
};

/// <summary>
/// 更新
/// </summary>
void Enemy::Update() {

	//デスフラグの立った弾を削除
	//bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
	//	return bullet->IsDead();
	//	});

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

	////弾更新
	//for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
	//	bullet->SetPlayer(player_);
	//	bullet->Update();
	//}

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

	////弾描画
	//for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
	//	bullet->Draw(viewProjection);
	//}
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
	Transfer();
};

/// <summary>
/// 発射
/// </summary>
void Enemy::Fire() {
	assert(player_);

	//弾速度
	const float kBulletSpeed = 1.0f;
	//弾のベクトル
	Vector3 bulletVelocity_;

#pragma region 自機狙い
	//自機と敵の位置座標を取得
	Vector3 plPos = player_->GetWorldPosition();
	Vector3 enPos = GetWorldPosition();

	//移動方向(差分ベクトル)を出す
	bulletVelocity_ = plPos - enPos;

	//移動方向を正規化(１にする)
	Vector3Normalize(bulletVelocity_);

	//移動方向を1フレーム当たりの速さにする
	bulletVelocity_ *= kBulletSpeed;
#pragma endregion

	//弾生成
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();

	newBullet->Intialize(modelBullet_,worldTransform_.translation_, bulletVelocity_);

	//弾を登録
	gameScene_->AddEnemyBullet(std::move(newBullet));
	//bullets_.push_back(std::move(newBullet));
};

/// 弾のタイマーを全削除
void Enemy::TimerClear() {
	timedCalls_.clear();
};

/// <summary>
/// 転送
/// </summary>
void Enemy::Transfer() {
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

/// <summary>
/// ステート変更
/// </summary>
void Enemy::ChangeState(BaseEnemyState* newState) {
	delete state_;
	state_ = newState;
};

//接近フェーズ初期化
void Enemy::IntializeApproachPhase() {
	//発射タイマーをセット
	timedCalls_.push_back(std::make_unique<TimedCall>(
		std::bind(&Enemy::FireAndResetTimer, this),
		kFireInterval));
};

/// <summary>
/// セッター
/// </summary>
void Enemy::SetPosition(Vector3 position) {
	this->worldTransform_.translation_ = position;
};

/// <summary>
/// ゲッター
/// </summary>
Vector3 Enemy::GetWorldPosition() {
	//ワールド座標を入れる変数
	Vector3 worldPos;

	//ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
};

/// <summary>
/// コールバック
/// </summary>
// 発射とタイマ－のリセット
void Enemy::FireAndResetTimer() {
	//弾を発射
	Fire();

	//発射タイマーをセット
	timedCalls_.push_back(std::make_unique<TimedCall>(
		std::bind(&Enemy::FireAndResetTimer, this),
		kFireInterval));
};
//衝突したら呼び出される
void Enemy::OnCollision() {

};