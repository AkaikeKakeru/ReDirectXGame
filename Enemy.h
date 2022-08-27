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

#include "Player.h"

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
	/// <param name="position">初期座標</param>
	///  <param name="approachVelocity">接近速度</param>
	/// <param name="leaveVelocity">離脱速度</param>
	void Intialize(
		Model* model,
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
	//弾リストを取得
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullet() {
		return bullets_;}

	Vector3 GetApproachSpeed(){
		return this->approachVelocity_;}

	Vector3 GetLeaveSpeed(){
		return this->leaveVelocity_;}

	/// <summary>
	/// セッター
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(Vector3 position);
	void SetIsFire(bool isFire);

	void SetPlayer(Player* player) { this->player_ = player; }

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
	static float kRadius_;

	//弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_ ;

	//時限発動のリスト
	std::list<std::unique_ptr<TimedCall>> timedCalls_ ;

	//自キャラ
	Player* player_ = nullptr;
};