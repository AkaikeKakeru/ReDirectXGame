#pragma once

class EnemyBullet{
public:
	EnemyBullet();
	~EnemyBullet();

	///<summary>
	///初期化
	///<summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	void Intialize(Model* mode, const Vector3& position);

	///<summary>
	///更新
	///<summary>
	void Update();

	///<summary>
	///描画
	///<summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(const ViewProjection& viewProjection);

private:
};