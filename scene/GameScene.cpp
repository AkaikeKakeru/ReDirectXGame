#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "PrimitiveDrawer.h"
#include "AxisIndicator.h"

#include <math.h>

#define PI 3.141592

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete debugCamera_;
	delete model_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	//モデルの生成
	model_ = Model::Create();

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 800);

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	//scale
	//X,Y,Z方向のスケーリングを設定
	worldTransform_.scale_ = { 5.0f,1.0f,5.0f };

	////スケーリング行列を宣言
	Matrix4 matScale;

	matScale =
	{ worldTransform_.scale_.x,0,0,0,
		0,worldTransform_.scale_.y,0,0,
		0,0,worldTransform_.scale_.z,0,
		0,0,0,1 };


	//Rote
	float radian = 45 * PI / 180.0;

	//X,Y,Z方向の回転を設定
	worldTransform_.rotation_ = { radian,radian,radian };

	//合成用回転行列を宣言
	Matrix4 matRot;

	//各軸用回転行列を宣言
	Matrix4 matRotX, matRotY, matRotZ;

	matRotX =
	{ 1,0,0,0,
		0,cos(worldTransform_.rotation_.x),sin(worldTransform_.rotation_.x),0,
		0,-sin(worldTransform_.rotation_.x),cos(worldTransform_.rotation_.x),0,
		0,0,0,1 };

	matRotY =
	{ cos(worldTransform_.rotation_.y),0,-sin(worldTransform_.rotation_.y),0,
		0,1,0,0,
		sin(worldTransform_.rotation_.y),0,cos(worldTransform_.rotation_.y),0,
		0,0,0,1 };

	matRotZ =
	{ cos(worldTransform_.rotation_.z),sin(worldTransform_.rotation_.z),0,0,
		-sin(worldTransform_.rotation_.z),cos(worldTransform_.rotation_.z),0,0,
		0,0,1,0,
		0,0,0,1 };

	//各軸の回転行列を合成
	matRot = matRotZ *= matRotX *= matRotY;


	//translation
	//X,Y,Z方向の平行移動を設定
	worldTransform_.translation_ = { 10.0f,10.0f,10.0f };

	//平行移動行列を宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	matTrans =
	{ 1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		worldTransform_.translation_.x,worldTransform_.translation_.y,worldTransform_.translation_.z,1 };

	//行列の合成
	//ワールドトランスフォーム行列
	worldTransform_.matWorld_ =
	{ 1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1 };

	worldTransform_.matWorld_ *= matScale;
	worldTransform_.matWorld_ *= matRot;
	worldTransform_.matWorld_ *= matTrans;

	//行列の転送
	worldTransform_.TransferMatrix();
}

void GameScene::Update() {
	//デバッグカメラの更新
	debugCamera_->Update();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)

	for (int i = 0; i < 12; i++)
	{
		PrimitiveDrawer::GetInstance()->DrawLine3d(vertex[edgeList[i][0]], vertex[edgeList[i][1]], vecColor);
	}

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
