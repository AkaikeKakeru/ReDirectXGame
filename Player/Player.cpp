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
void Player::Intialize(Model* model) {
	//nullチェック
	assert(model);

	//メンバ変数に記録
	model_ = model;
	textureHandle_ = TextureManager::Load("mario.jpg");

	//シングルトンインスタンス
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();
}

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

	Attack();

	//弾更新
	for(std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	Transfer(worldTransform_,myMatrix_);

	//自キャラデバッグ用表示
	debugText_->SetPos(50, 150);
	debugText_->Printf(
		"PlaTrans:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);

	debugText_->SetPos(50, 180);
	debugText_->Printf(
		"PlaRota:(%f,%f,%f)", worldTransform_.rotation_.x, worldTransform_.rotation_.y, worldTransform_.rotation_.z);
}

///<summary>
///描画
///<summary>
void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

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
void Player::Transfer(WorldTransform worldTransform,MyMatrix myMatrix)
{
	//matrix
	static Matrix4 scale;
	static  Matrix4 rota;
	static  Matrix4 translation;

	scale = myMatrix.MatrixScale(worldTransform.scale_);
	rota = myMatrix.MatrixRotationZ(worldTransform.rotation_);
	rota *= myMatrix.MatrixRotationX(worldTransform.rotation_);
	rota *= myMatrix.MatrixRotationY(worldTransform.rotation_);
	translation = myMatrix.MatrixTranslation(worldTransform.translation_);

	worldTransform_.matWorld_ = myMatrix.MatrixWorld(scale, rota, translation);

	//転送
	worldTransform.TransferMatrix();
}

///<summary>
///攻撃
///<summary>
void Player::Attack(){
	const float kBulletSpeed = 0.1f;
	Vector3 bulletVelocity_ = Vector3(0, 0, kBulletSpeed);

	if (input_->TriggerKey(DIK_SPACE))
	{
		//速度ベクトルを自機の向きにあわせる
		bulletVelocity_ = myMatrix_.CrossVector(bulletVelocity_, worldTransform_.matWorld_);

		//弾を生成し、初期化
 		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Intialize(model_,worldTransform_.translation_,bulletVelocity_);

		//弾を登録
		//bullet_ = newBullet;
		/*bullets_.reset(newBullet);*/
		bullets_.push_back(std::move(newBullet));
	}
}