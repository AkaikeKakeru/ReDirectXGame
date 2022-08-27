#include"EnemyBullet.h"
#include <cassert>

EnemyBullet::EnemyBullet(){}
EnemyBullet::~EnemyBullet(){}

///<summary>
///初期化
///<summary>
/// <param name="model">モデル</param>
/// <param name="position">初期座標</param>
/// <param name="velocity">速度</param>
void EnemyBullet::Intialize(
	Model* model,
	const Vector3& position,
	const Vector3& velocity) {
	//nullチェック
	assert(model);

	//メンバ変数に記録
	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("enemy_bullet.png");

	//ワールド変換の初期化
	worldTransform_.Initialize();
	
	//Z方向に伸びた形状
	worldTransform_.scale_.x = 0.5f;
	worldTransform_.scale_.y = 0.5f;
	worldTransform_.scale_.z = 3.0f;

	worldTransform_.translation_ = position;

	//速度初期化
	velocity_ = velocity;

	//弾の初期角度を変える
	MatchRotationAppearance();
}

///<summary>
///更新
///<summary>
void EnemyBullet::Update(){
	//ホーミング
	//Homing();

	//座標を移動させる(1フレーム分の移動量を足しこむ)
	worldTransform_.translation_ += velocity_;

	//転送
	Transfer();

	//時間経過で朽ちる
	if (--deathtimer_ <= 0) {
		isDead_ = true;
	}
}

///<summary>
///描画
///<summary>
/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
void EnemyBullet::Draw(const ViewProjection& viewProjection){
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

// 弾の見た目の進行方向を回転
void EnemyBullet::MatchRotationAppearance() {
	//y軸まわり周り角度(θy)
	worldTransform_.rotation_.y = std::atan2(velocity_.x,velocity_.z);

	//velocity_からY成分を0にしたベクトルを求める
	Vector3 velocityNotY(velocity_.x, 0.0f, velocity_.z);

	//velocityNotYから、3Dベクトルとしての長さを求めて、
	//velocity_の横軸方向として扱う
	float velocityXZ = Vector3Length(velocityNotY);

	//x軸まわり周り角度(θx)
	worldTransform_.rotation_.x = std::atan2(-velocity_.y,velocityXZ);
};

//ホーミング
void EnemyBullet::Homing() {
	//弾速さ
	float kBulletSpeed = 1.0f;
	//弾のホーミング精度
	float kHomingAccuracy = 0.06f;

	//自機と敵弾の位置座標を取得
	Vector3 plPos = player_->GetWorldPosition();
	Vector3 blPos = GetWorldPosition();

	//敵弾から自キャラへのベクトルを計算
	Vector3 toPlayer = plPos - blPos;

	//ベクトルを正規化する
	Vector3Normalize(toPlayer);
	Vector3Normalize(velocity_);

	//球面線形補間により、今の速度と自キャラへのベクトルを内挿し、新たな速度とする。
	velocity_ = myVector3_.Slerp(velocity_, toPlayer, kHomingAccuracy) * kBulletSpeed;

	//進行方向に見た目の回転を合わせる。
	MatchRotationAppearance();
};

//転送
void EnemyBullet::Transfer()
{
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
/// ゲッター
/// </summary>
Vector3 EnemyBullet::GetWorldPosition() {
	//ワールド座標を入れる変数
	Vector3 worldPos;

	//ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
};

float EnemyBullet::GetRadius() {
	return kRadius_;
};

/// <summary>
/// コールバック
/// </summary>
//衝突したら呼び出される
void EnemyBullet::OnCollision() {
	isDead_ = true;
};