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
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection viewProjection_;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	/*Vector3 vertex[8] =
	{
		{0.0f,0.0f,0.0f},
		{5.0f,0.0f,0.0f},
		{5.0f,0.0f,5.0f},
		{0.0f,0.0f,5.0f},
		{0.0f,5.0f,0.0f},
		{5.0f,5.0f,0.0f},
		{5.0f,5.0f,5.0f},
		{0.0f,5.0f,5.0f}
	};
	


	int edgeList[12][2] =
	{
		{0,1},
		{1,2},
		{2,3},
		{3,0},
		{0,4},
		{1,5},
		{2,6},
		{3,7},
		{4,5},
		{5,6},
		{6,7},
		{7,4}
	};

	Vector4 vecColor = 
	{
		0x00, 0x00, 0x00, 0xFF
	};

	Vector3 vertexScale[8] =
	{
		{0.0f,0.0f,0.0f},
		{5.0f,0.0f,0.0f},
		{5.0f,0.0f,5.0f},
		{0.0f,0.0f,5.0f},
		{0.0f,5.0f,0.0f},
		{5.0f,5.0f,0.0f},
		{5.0f,5.0f,5.0f},
		{0.0f,5.0f,5.0f}
	};

	Vector3 vertexRote[8] =
	{
		{0.0f,0.0f,0.0f},
		{5.0f,0.0f,0.0f},
		{5.0f,0.0f,5.0f},
		{0.0f,0.0f,5.0f},
		{0.0f,5.0f,0.0f},
		{5.0f,5.0f,0.0f},
		{5.0f,5.0f,5.0f},
		{0.0f,5.0f,5.0f}
	};

	Vector3 vertexTransform[8] =
	{
		{0.0f,0.0f,0.0f},
		{5.0f,0.0f,0.0f},
		{5.0f,0.0f,5.0f},
		{0.0f,0.0f,5.0f},
		{0.0f,5.0f,0.0f},
		{5.0f,5.0f,0.0f},
		{5.0f,5.0f,5.0f},
		{0.0f,5.0f,5.0f}
	};


	Vector4 vecScaleColor = 
	{
		0x00, 0xFF, 0x00, 0xFF
	};

	Vector4 vecRoteColor = 
	{
		0x00, 0x00, 0xFF, 0xFF
	};

	Vector4 vecTransformColor = 
	{
		0xFF, 0x00, 0x00, 0xFF
	};*/

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
