#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "PrimitiveDrawer.h"
#include "AxisIndicator.h"

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

	//startMyLine_ = new Vector3(0.0f, 0.0f,0.0f);
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());


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
	
	Vector3 vertex[8] =
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
