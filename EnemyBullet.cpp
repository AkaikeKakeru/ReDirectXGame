#include"EnemyBullet.h"
#include <cassert>

EnemyBullet::EnemyBullet(){}
EnemyBullet::~EnemyBullet(){}

///<summary>
///初期化
///<summary>
/// <param name="model">モデル</param>
/// <param name="position">初期座標</param>
/// <param name="velocity">速度</param>
void EnemyBullet::Intialize(
	Model* model,
	const Vector3& position,
	const Vector3& velocity) {
	//nullチェック
	assert(model);

	//メンバ変数に記録
	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("enemy_bullet.png");

	//ワールド変換の初期化
	worldTransform_.Initialize();
	
	//Z方向に伸びた形状
	worldTransform_.scale_.x = 0.5f;
	worldTransform_.scale_.y = 0.5f;
	worldTransform_.scale_.z = 3.0f;

	worldTransform_.translation_ = position;

	//速度初期化
	velocity_ = velocity;
}

///<summary>
///更新
///<summary>
void EnemyBullet::Update(){
	//座標を移動させる(1フレーム分の移動量を足しこむ)
	worldTransform_.translation_ += velocity_;

	Transfer(worldTransform_, myMatrix_);

	//時間経過で朽ちる
	if (--deathtimer_ <= 0) {
		isDead_ = true;
	}
}

///<summary>
///描画
///<summary>
/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
void EnemyBullet::Draw(const ViewProjection& viewProjection){
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void EnemyBullet::Transfer(WorldTransform worldTransform,MyMatrix myMatrix)
{
	//matrix
	static Matrix4 scale;
	static  Matrix4 rot;
	static  Matrix4 translation;

	//行列更新
	scale = myMatrix.MatrixScale(worldTransform.scale_);
	rot = myMatrix.MatrixRotationZ(worldTransform.rotation_);
	rot *= myMatrix.MatrixRotationX(worldTransform.rotation_);
	rot *= myMatrix.MatrixRotationY(worldTransform.rotation_);
	translation = myMatrix.MatrixTranslation(worldTransform.translation_);

	worldTransform.matWorld_ = myMatrix.MatrixWorld(scale, rot, translation);

	//転送
	worldTransform.TransferMatrix();
}