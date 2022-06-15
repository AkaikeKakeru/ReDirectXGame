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
#include "DebugCamera.h"

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
	/// worldTransform初期化
	/// </summary>
	Matrix4 TransformScale(WorldTransform* worldTransform,Vector3 scale);
	Matrix4 TransformRotation(WorldTransform* worldTransform,Vector3 rotation);
	Matrix4 TransformTranslation(WorldTransform* worldTransform,Vector3 translation);
	void WorldTransformTransfer(WorldTransform* worldTransform,Vector3 scale,Vector3 rotation,Vector3 translation);


	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	//モデル
	Model* model_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransforms_[100];
	//ビュープロジェクション
	ViewProjection viewProjection_;

	//カメラ上方向の角度
	float viewAngle = 0.0f;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
