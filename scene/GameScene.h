#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <sstream>

#include "DebugCamera.h"
#include "RailCamera.h"

#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"

#include "directxmath.h"
#include <random>
#include "MyVector3.h"
#include <memory>

#include "Collider.h"
#include "CollisionManager.h"

using namespace DirectX;

#define PI = XM_PI

class Collider;

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {
public: // メンバ関数
  /// <summary>
  /// コンストクラタ
  /// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//弾リストを取得
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullet() {
		return enemyBullets_;}

	/// <summary>
	/// 敵弾を追加
	/// </summary>
	/// <param name="enemyBullet">敵弾</param>
	void AddEnemyBullet(std::unique_ptr<EnemyBullet> enemyBullet);

private: // メンバ関数

		 //衝突マネージャーを動かす
	void RunCollisionManager();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	//モデル
	Model* model_ = nullptr;

	//自キャラ
	std::unique_ptr<Player> player_;
	Model* modelPlayer_ = nullptr;

	//敵キャラ
	//std::unique_ptr<Enemy> enemy_;
	std::list<std::unique_ptr<Enemy>> enemys_;
	Model* modelEnemy_ = nullptr;

	//敵弾
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets_;

	//敵発生コマンド
	std::stringstream enemyPopCommands;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//カメラ上方向の角度
	float viewAngle_ = 0.0f;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	//レールカメラ
	RailCamera* railCamera_ = nullptr;

	//Vector3による特殊な計算の関数まとめ
	MyVector3 myVector3_;

	//衝突マネージャー
	std::unique_ptr<CollisionManager> collisionManager_;

	//天球
	std::unique_ptr<Skydome> skydome_;
	//天球用モデル
	Model* modelSkydome_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};