#include <Player.h>
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
void Player::Intialize(Model* model, uint32_t textureHandle) {

	//nullチェック
	assert(model);

	//メンバ変数に記録
	this->model_ = model;
	this->textureHandle_ = textureHandle;

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
	//キャラクター移動処理
	{
		//matrix
		static Matrix4 scale;
		static  Matrix4 rot;
		static  Matrix4 translation;

		//limit
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

		//行列更新
		scale = myMatrix_.MatrixScale(worldTransform_.scale_);
		rot = myMatrix_.MatrixRotationZ(worldTransform_.rotation_);
		rot *= myMatrix_.MatrixRotationX(worldTransform_.rotation_);
		rot *= myMatrix_.MatrixRotationY(worldTransform_.rotation_);
		translation = myMatrix_.MatrixTranslation(worldTransform_.translation_);

		worldTransform_.matWorld_ = myMatrix_.MatrixWorld(scale, rot, translation);

		//転送
		worldTransform_.TransferMatrix();

		//デバッグ用表示
		debugText_->SetPos(50, 150);
		debugText_->Printf(
			"Root:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
	}
}

///<summary>
///描画
///<summary>
void Player::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}