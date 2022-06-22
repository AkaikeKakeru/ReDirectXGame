#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "Input.h"
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
	/// <param name="model">テクスチャハンドル</param>
	void Intialize(Model* mode, uint32_t textureHandle);

	void Move(/*Matrix4 scale,Matrix4 rot,Matrix4 translation*/);
	void Rotate(/*Matrix4 scale,Matrix4 rot,Matrix4 translation*/);
	void Transfer(WorldTransform worldTransform,MyMatrix myMatrix/*,Matrix4 scale,Matrix4 rot,Matrix4 translation*/);

	///<summary>
	///更新
	///<summary>
	void Update(/*MyMatrix myMatrix*/);

	///<summary>
	///描画
	///<summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(ViewProjection viewProjection);

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
};