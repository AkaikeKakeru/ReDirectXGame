#pragma once

/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider {
public:
	//衝突時に呼ばれる関数
	virtual void OnCollision();

	//ワールド座標を取得
	virtual Vector3 GetWorldPosition() = 0;
	//半径を取得
	float GetRadius();

	//半径を設定
	void SetRadius(float r) {this->radius_ = r; };
private:
	//衝突半径
	float radius_ = 0.5f;
};