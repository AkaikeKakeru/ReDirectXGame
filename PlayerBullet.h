#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "MyMatrix.h"

///<summary>
///自キャラの弾
/// </summary>
class PlayerBullet {
public:
	PlayerBullet();
	~PlayerBullet();

	///<summary>
	///初期化
	///<summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	void Intialize(Model* mode, const Vector3& position);

	///<summary>
	///更新
	///<summary>
	void Update();

	///<summary>
	///描画
	///<summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(const ViewProjection& viewProjection);

	///<summary>
	///転送
	///<summary>
	void Transfer(WorldTransform worldTransform,MyMatrix myMatrix);

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//ワールド変換行列
	MyMatrix myMatrix_;
};