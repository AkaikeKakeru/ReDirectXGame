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
}

///<summary>
///描画
///<summary>
void Player::Draw(){
}