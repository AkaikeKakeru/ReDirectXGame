#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

/// <summary>
/// 天球
/// </summary>
class Skydome {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(Model* model);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	////ビュープロジェクション
	//ViewProjection* viewProjection_;
	//モデル
	Model* model_ = nullptr;
};