#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "Input.h"

#include "PlayerBullet.h"
#include <memory>
#include <List>
#include "MyMatrix.h"

///<summary>
///自キャラ
///<summary>
class Player
{
public:
	Player();
	~Player();

	///<summary>
	///初期化
	///<summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Intialize(Model* model/*, uint32_t textureHandle*/);

	///<summary>
	///更新
	///<summary>
	void Update();

	///<summary>
	///描画
	///<summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(/*const */ViewProjection&  viewProjection);

	///<summary>
	///移動
	///<summary>
	void Move();

	///<summary>
	///旋回
	///<summary>
	void Rotate();

	///<summary>
	///転送
	///<summary>
	void Transfer(WorldTransform worldTransform,MyMatrix myMatrix);

	///<summary>
	///攻撃
	///<summary>
	void Attack();

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//インプット
	Input* input_ = nullptr;
	//デバッグテキスト
	DebugText* debugText_ = nullptr;
	////ビュープロジェクション
	//ViewProjection viewProjection_;
	//ワールド変換行列
	MyMatrix myMatrix_;

	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
};