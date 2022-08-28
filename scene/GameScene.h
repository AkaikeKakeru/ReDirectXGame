﻿#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"

#include "Player.h"
#include "Enemy.h"

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
	//Player* player_ = nullptr;
	std::unique_ptr<Player> player_; /*= nullptr;*/

	//敵キャラ
	//Enemy* enemy_ = nullptr;
	std::unique_ptr<Enemy> enemy_; /*= nullptr;*/

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//カメラ上方向の角度
	float viewAngle = 0.0f;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	//Vector3による特殊な計算の関数まとめ
	MyVector3 myVector3_;

	//衝突マネージャー
	std::unique_ptr<CollisionManager> collisionManager_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};