#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "MyMatrix.h"
#include "DebugText.h"

#include "Collider.h"

///<summary>
///自キャラの弾
/// </summary>
class PlayerBullet:public Collider {
public:
	PlayerBullet();
	~PlayerBullet();

	///<summary>
	///初期化
	///<summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	/// /// <param name="velocity">速度</param>
	void Intialize(Model* model, const Vector3& position, const Vector3& velocity);

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
	void Transfer();

	bool IsDead() const { return isDead_; }

	/// <summary>
	/// コールバック
	/// </summary>
	//衝突したら呼び出される
	void OnCollision() override;

	/// <summary>
	/// ゲッター
	/// </summary>
	/// <returns></returns>
	//衝突時に呼ばれる関数
	Vector3 GetWorldPosition() override;
	float GetRadius() override{
		return kRadius_;
	};

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//ワールド変換行列
	MyMatrix myMatrix_;

	//デバッグテキスト
	DebugText* debugText_ = nullptr;

	//速度
	Vector3 velocity_;

	//半径
	float kRadius_ = 0.5f;

	//寿命<frm>
	static const int32_t kLifeTime = 60 * 5;

	//デスタイマー
	int32_t deathtimer_ = kLifeTime;

	//デスフラグ
	bool isDead_ = false;
};