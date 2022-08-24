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

/// <summary>
/// 敵
/// </summary>
class Enemy
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
	///  <param name="worldTransform">ワールドトランスフォーム</param>
	/// <param name="myMatrix">座標変換のまとめ</param>
	void Transfer(WorldTransform worldTransform,MyMatrix myMatrix);

	/// <summary>
	/// 移動
	/// </summary>
	/// <param name="position">初期座標</param>
	/// <param name="velocity">速度</param> 
	void Move(Vector3 position,Vector3 velocity);

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();

	/// <summary>
	/// ステート変更
	/// </summary>
	/// <param name="newState">新しい状態</param>  
	void ChangeState(BaseEnemyState* newState);

	/// <summary>
	/// ゲッター
	/// </summary>
	Vector3 GetPosition() {
		return this->worldTransform_.translation_;};

	Vector3 GetApproachSpeed(){
		return this->approachVelocity_;}

	Vector3 GetLeaveSpeed(){
		return this->leaveVelocity_;}

	/// <summary>
	/// セッター
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(Vector3 position);

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

	//弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_ ;
};