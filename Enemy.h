#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "MyMatrix.h"
#include "DebugText.h"
#include "Input.h"

//行動フェーズ
enum class Phase {
	Approach, //接近する
	Leave, //離脱する
};

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
	/// <param name="velocity">速度</param>
	void Intialize(Model* model, const Vector3& position, const Vector3& ApproachVelocity,const Vector3& LeaveVelocity);

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
	void Transfer(WorldTransform worldTransform,MyMatrix myMatrix);

	/// <summary>
	/// 移動
	/// </summary>
	void ApproachMove() ;
	void LeaveMove() ;

private:
	//メンバ関数ポインタのテーブル
	static void (Enemy::* spPhaseTable[])();

	//フェーズ
	Phase phase_ =  Phase::Approach;
	

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
};

