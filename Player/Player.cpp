#include "Player.h"
#include <cassert>

Player::Player()
{
}

Player::~Player()
{

}

///<summary>
///初期化
///<summary>
void Player::Initialize(Model* model,Model* modelBullet) {
	//nullチェック
	assert(model);

	//nullチェック
	assert(modelBullet);

	//メンバ変数に記録
	model_ = model;
	modelBullet_ = modelBullet;
	textureHandle_ = TextureManager::Load("plane/plane_Map.png");

	//シングルトンインスタンス
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.rotation_.y = XMConvertToRadians(180.0f);
	worldTransform_.translation_.z = 40.0f;
	worldTransform3DReticle_.Initialize();

	//衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	//衝突対象を自分の属性以外に設定
	SetCollisionMask(~(kCollisionAttributePlayer));
};

///<summary>
///更新
///<summary>
void Player::Update() {

#ifdef  _DEBUG
#endif //  _DEBUG

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {

		return bullet->IsDead();
		});

	Rotate();

	Move();

	Update3DReticle();

	Attack();

	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	Transfer();

	//自キャラデバッグ用表示
	debugText_->SetPos(50, 150);
	debugText_->Printf(
		"PlaTrans:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);

	debugText_->SetPos(50, 180);
	debugText_->Printf(
		"PlaRota:(%f,%f,%f)", worldTransform_.rotation_.x, worldTransform_.rotation_.y, worldTransform_.rotation_.z);

};

///<summary>
///描画
///<summary>
void Player::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	modelBullet_->Draw(worldTransform3DReticle_, viewProjection);

	//弾描画
	for(std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

///<summary>
///移動
///<summary>
void Player::Move()
{
	//キャラクター移動処理
	//限界点
	const float kMoveLimitX = 35.5f;
	const float kMoveLimitY = 18.5f;

	//キャラクターの移動ベクトル
	Vector3 move = { 0,0,0 };

	if (input_->PushKey(DIK_LEFT)) {
		move.x -= 0.2f;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		move.x += 0.2f;
	}

	if (input_->PushKey(DIK_UP)) {
		move.y += 0.2f;
	}
	if (input_->PushKey(DIK_DOWN)) {
		move.y -= 0.2f;
	}

	//移動ベクトルを加算
	worldTransform_.translation_ += move;

	//移動時の限界点を超えられなくする
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);

	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);
}

///<summary>
///旋回
///<summary>
void Player::Rotate()
{


#pragma region 半身回転
	//上半身回転
	if (input_->PushKey(DIK_U))
	{
		worldTransform_.rotation_.y -= 0.05f;
	}
	else if (input_->PushKey(DIK_I))
	{
		worldTransform_.rotation_.y += 0.05f;
	}

	//下半身回転
	if (input_->PushKey(DIK_J))
	{
		worldTransform_.rotation_.y -= 0.05f;
	}
	else if (input_->PushKey(DIK_K))
	{
		worldTransform_.rotation_.y += 0.05f;
	}
#pragma endregion

}

///<summary>
///転送
///<summary>
void Player::Transfer()
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

	//親のワールド行列を掛け算
	worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;

	//転送
	worldTransform_.TransferMatrix();
}

///<summary>
///攻撃
///<summary>
void Player::Attack(){
	const float kBulletSpeed = -1.0f;
	Vector3 bulletVelocity_ = Vector3(0, 0, kBulletSpeed);

	bulletVelocity_ = Vector3(
		worldTransform3DReticle_.matWorld_.m[3][0] 
			-worldTransform_.matWorld_.m[3][0],
		worldTransform3DReticle_.matWorld_.m[3][1]
			-worldTransform_.matWorld_.m[3][1],
		worldTransform3DReticle_.matWorld_.m[3][2]
			-worldTransform_.matWorld_.m[3][2]
	);

	bulletVelocity_ = Vector3Normalize(bulletVelocity_)
		* kBulletSpeed;

	Vector3 bulletPosition_ = 
		Vector3(
			worldTransform_.matWorld_.m[3][0],
			worldTransform_.matWorld_.m[3][1],
			worldTransform_.matWorld_.m[3][2]
		);

	if (input_->TriggerKey(DIK_SPACE))
	{
		//速度ベクトルを自機の向きにあわせる
		bulletVelocity_ = myMatrix_.CrossVector(bulletVelocity_, worldTransform_.matWorld_);

		
		//弾を生成し、初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Intialize(modelBullet_,bulletPosition_,bulletVelocity_);

		//弾を登録
		//bullet_ = newBullet;
		/*bullets_.reset(newBullet);*/
		bullets_.push_back(std::move(newBullet));
	}
}

void Player::Update3DReticle() {
	//自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DRethicle = -50.0f;
	//自機から3Dレティクルへのオフセット(Z+向き)
	Vector3 offset = { 0,0,1.0f };
	//自機のワールド行列の回転を反映
	offset = myMatrix_.CrossVector(offset, worldTransform_.matWorld_);
	
	//ベクトルの長さを整える
	offset = Vector3Normalize(offset) * kDistancePlayerTo3DRethicle;
	//3Dレティクルの座標を指定
	worldTransform3DReticle_.translation_ = worldTransform_.translation_ + offset;


	//matrix
	static Matrix4 scale;
	static  Matrix4 rota;
	static  Matrix4 translation;

	scale = myMatrix_.MatrixScale(worldTransform3DReticle_.scale_);
	rota = myMatrix_.MatrixRotationZ(worldTransform3DReticle_.rotation_);
	rota *= myMatrix_.MatrixRotationX(worldTransform3DReticle_.rotation_);
	rota *= myMatrix_.MatrixRotationY(worldTransform3DReticle_.rotation_);
	translation = myMatrix_.MatrixTranslation(worldTransform3DReticle_.translation_);

	worldTransform3DReticle_.matWorld_ = myMatrix_.MatrixWorld(scale, rota, translation);
	worldTransform3DReticle_.TransferMatrix();
};

Vector3 Player::GetWorldPosition() {
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
//衝突したら呼び出される
void Player::OnCollision() {

};