#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"

#include "MyMatrix.h"
#include "MathUtility.h"

#include "DebugText.h"

class RailCamera
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& position,
		const Vector3& viewAngle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	///<summary>
	///転送
	///<summary>
	void Transfer();

	Vector3 t(const Vector3& v, const Matrix4& m);

	const ViewProjection& GetViewProjection() { return this->viewProjection_; };
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection viewProjection_;

	//ワールド変換行列
	MyMatrix myMatrix_;

	//デバッグテキスト
	DebugText* debugText_ = nullptr;
};