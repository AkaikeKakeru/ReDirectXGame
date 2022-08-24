#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "MyMatrix.h"

class EnemyBullet{
public:
	EnemyBullet();
	~EnemyBullet();

	///<summary>
	///初期化
	///<summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	/// <param name="velocity">速度</param>
	void Intialize(Model* mode, const Vector3& position, const Vector3& velocity);

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
	///  <param name="worldTransform">ワールドトランスフォーム</param>
	/// <param name="myMatrix">座標変換のまとめ</param>
	void Transfer(WorldTransform worldTransform, MyMatrix myMatrix);

	bool IsDead() const { return isDead_; }

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//ワールド変換行列
	MyMatrix myMatrix_;
	//速度
	Vector3 velocity_;

	//寿命<frm>
	static const int32_t kLifeTime = 60 * 5;
	//デスタイマー
	int32_t deathtimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;
};