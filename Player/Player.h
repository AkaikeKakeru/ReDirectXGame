#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "Input.h"

#include "PlayerBullet.h"
#include <memory>
#include <List>
#include "MyMatrix.h"

#include "Collider.h"

///<summary>
///自キャラ
///<summary>
class Player:public Collider
{
public:
	Player();
	~Player();

	///<summary>
	///初期化
	///<summary>
	/// <param name="model">モデル</param>
	/// <param name="modelBullet">弾モデル</param> 
	/// <param name="cameraWorld">カメラのワールド行列</param>
	void Initialize(Model* model,Model* modelBullet);

	///<summary>
	///更新
	///<summary>
	void Update();

	///<summary>
	///描画
	///<summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(const ViewProjection&  viewProjection);

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
	void Transfer(/*MyMatrix myMatrix*/);

	///<summary>
	///攻撃
	///<summary>
	void Attack();

	/// <summary>
	/// ゲッター
	/// </summary>
	//ワールド座標を取得
	//衝突時に呼ばれる関数
	Vector3 GetWorldPosition() override;
	//弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullet() {
		return bullets_;}

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
	//衝突属性(自分)を設定
	void SetCollisionAttribute (uint32_t collisionAttribute) { this->collisionAttribute_ = collisionAttribute; };
	//衝突マスク(相手)を設定
	void SetCollisionMask (uint32_t collisionMask) { this->collisionMask_ = collisionMask; };

	void SetWorldParent(WorldTransform* worldTransform) {
		this->worldTransform_.parent_ = worldTransform; };

	void SetCamera(WorldTransform* worldCamera) {
		this->worldTransform_.parent_ = worldCamera; };

	/// <summary>
	/// コールバック
	/// </summary>
	//衝突したら呼び出される
	void OnCollision() override;

private:
	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;
	Model* modelBullet_ = nullptr;

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

	//半径
	float kRadius_ = 0.5f;

	//衝突属性(自分)
	uint32_t collisionAttribute_ = 0xffffffff;
	//衝突マスク(相手)
	uint32_t collisionMask_ = 0xffffffff;
};