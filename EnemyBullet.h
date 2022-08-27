#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "MyMatrix.h"
#include "MyVector3.h"
#include "Collider.h"

#include "Player.h"

class Player;

class EnemyBullet:public Collider {
public:
	EnemyBullet();
	~EnemyBullet();

	///<summary>
	///初期化
	///<summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	/// <param name="velocity">速度</param>
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

	/// <summary>
	/// 弾の見た目の進行方向を回転
	/// </summary>
	void MatchRotationAppearance();

	/// <summary>
	/// ホーミング
	/// </summary>
	void Homing();

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

	//衝突属性(自分)を取得
	uint32_t GetCollisionAttribute() override{
		return collisionAttribute_;
	};
	//衝突マスク(相手)を取得
	uint32_t GetCollisionMask() override{
		return collisionMask_;
	};

	/// <summary>
	/// セッター
	/// </summary>
	void SetPlayer(Player* player) { this->player_ = player; }

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//ワールド変換行列
	MyMatrix myMatrix_;
	//Vector3による特殊な計算の関数まとめ
	MyVector3 myVector3_;

	//半径
	float kRadius_ = 0.5f;

	//衝突属性(自分)
	uint32_t collisionAttribute_ = 0xffffffff;
	//衝突マスク(相手)
	uint32_t collisionMask_ = 0xffffffff;

	//速度
	Vector3 velocity_;

	//自機ポインタ
	Player* player_ = nullptr;

	//寿命<frm>
	static const int32_t kLifeTime = 60 * 5;
	//デスタイマー
	int32_t deathtimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;
};