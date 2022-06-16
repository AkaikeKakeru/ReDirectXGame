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
void Player::Intialize(Model*model,uint32_t textureHandle){

	//まさかnullじゃないよね???
	assert(model);

	//メンバ変数に記録
	this->model_ = model;
	this->textureHandle_ = textureHandle;

	//アイングルトンインスタンス
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();
}

///<summary>
///更新
///<summary>
void Player::Update(){
	//キャラクター移動処理
	{
		//キャラクターの移動ベクトル
		Vector3 move = { 0,0,0 };

		if (input_->PushKey(DIK_LEFT)) {
			move.x -= 0.2f;
		}
		if (input_->PushKey(DIK_RIGHT)) {
			move.x += 0.2f;
		}

		if (input_->PushKey(DIK_UP)) {
			move.y -= 0.2f;
		}
		if (input_->PushKey(DIK_DOWN)) {
			move.y += 0.2f;
		}

		worldTransform_.translation_ += move;

		Matrix4 scale = myMatrix_.MatrixScale(worldTransform_.scale_);
		Matrix4 rot = myMatrix_.MatrixRotationZ(worldTransform_.rotation_);
		rot *= myMatrix_.MatrixRotationX(worldTransform_.rotation_);
		rot *= myMatrix_.MatrixRotationY(worldTransform_.rotation_);
		Matrix4 translation = myMatrix_.MatrixTranslation(worldTransform_.translation_);

		worldTransform_.matWorld_ = myMatrix_.MatrixWorld(scale, rot, translation);

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
void Player::Draw(){
}