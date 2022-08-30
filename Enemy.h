#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "MyMatrix.h"
#include "DebugText.h"
#include "Input.h"

#include "EnemyBullet.h"
#include <memory>
#include <List>

#include "BaseEnemyState.h"
#include "EnemyApproach.h"
#include "EnemyLeave.h"

#include "TimedCall.h"
#include "Collider.h"

#include "GameScene.h"
#include "Player.h"

//ゲームシーン前方宣言
class GameScene;

//自機クラスの前方宣言
class Player;

/// <summary>
/// 敵
/// </summary>
class Enemy:public Collider
{
public:
	Enemy();
	~Enemy();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="modelBullet">弾モデル</param> 
	/// <param name="position">初期座標</param>
	///  <param name="approachVelocity">接近速度</param>
	/// <param name="leaveVelocity">離脱速度</param>
	void Intialize(
		Model* model,
		Model* modelBullet,
		const Vector3& position,
		const Vector3& approachVelocity,
		const Vector3& leaveVelocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(const ViewProjection& viewProjection);

	///<summary>
	///転送
	///<summary>
	void Transfer();

	/// <summary>
	/// 移動
	/// </summary>
	/// <param name="position">初期座標</param>
	/// <param name="velocity">速度</param> 
	void Move(Vector3 position,Vector3 velocity);

	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();

	/// <summary>
	/// 弾のタイマーを全削除
	/// </summary>
	void TimerClear();
		
	/// <summary>
	/// ステート変更
	/// </summary>
	/// <param name="newState">新しい状態</param>  
	void ChangeState(BaseEnemyState* newState);

	/// <summary>
	/// ゲッター
	/// </summary>
	//衝突時に呼ばれる関数
	Vector3 GetWorldPosition() override;
	//ワールド座標を取得
	Vector3 GetPosition() {
		return this->worldTransform_.translation_;};
	////弾リストを取得
	//const std::list<std::unique_ptr<EnemyBullet>>& GetBullet() {
	//	return bullets_;}

	float GetRadius() override {
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

	Vector3 GetApproachSpeed(){
		return this->approachVelocity_;}

	Vector3 GetLeaveSpeed(){
		return this->leaveVelocity_;}

	/// <summary>
	/// セッター
	/// </summary> 
	void SetPosition(Vector3 position);
	void SetIsFire(bool isFire);

	void SetGameScene(GameScene* gameScene) {this-> gameScene_ = gameScene; }

	void SetPlayer(Player* player) { this->player_ = player; }

	//衝突属性(自分)を設定
	void SetCollisionAttribute (uint32_t collisionAttribute) { this->collisionAttribute_ = collisionAttribute; };
	//衝突マスク(相手)を設定
	void SetCollisionMask (uint32_t collisionMask) { this->collisionMask_ = collisionMask; };


	bool IsDead() const { return isDead_; }

	/// <summary>
	/// コールバック
	/// </summary>
	/// 弾を発射し、タイマーをリセットする
	void FireAndResetTimer();
	//衝突したら呼び出される
	void OnCollision() override;

public://接近フェーズ
	//接近フェーズ初期化
	void IntializeApproachPhase();
public:
	//発射間隔<frm>
	static const int kFireInterval = 60;

private:
	//状態
	BaseEnemyState* state_ = nullptr;
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

	//ワールド変換行列
	MyMatrix myMatrix_;

	//速度
	Vector3 approachVelocity_;
	Vector3 leaveVelocity_;

	//半径
	float kRadius_ = 0.5f;

	//衝突属性(自分)
	uint32_t collisionAttribute_ = 0xffffffff;
	//衝突マスク(相手)
	uint32_t collisionMask_ = 0xffffffff;

	////弾
	//std::list<std::unique_ptr<EnemyBullet>> bullets_ ;

	//時限発動のリスト
	std::list<std::unique_ptr<TimedCall>> timedCalls_ ;

	//デスフラグ
	bool isDead_ = false;

	//ゲームシーン
	GameScene* gameScene_ = nullptr;

	//自キャラ
	Player* player_ = nullptr;
};