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

class Enemy; //Enemyクラス前方宣言

class BaseEnemyState 
{
protected:
	Enemy* enemy_;
public:
	 void Update();
};

class EnemyStateApproach : public BaseEnemyState
{
public:
	void Update();
};

class EnemyStateLeave : public BaseEnemyState
{
public:
	void Update();
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
	void Move(Vector3 position,Vector3 velocity);
	void ApproachMove() ;
	void LeaveMove() ;

	/// <summary>
	/// ステート変更
	/// </summary>
	void ChangeState(BaseEnemyState*);

	/// <summary>
	/// 座標のゲッター
	/// </summary>
	Vector3 GetPosition();

private:
	BaseEnemyState* state_;

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

