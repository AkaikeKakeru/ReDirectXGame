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

	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

}

void GameScene::Update() {
	//デバッグカメラの更新
	debugCamera_->Update();

	//Mesh CubePosition = model_.;

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
	

	Vector3 grid[44]
		=
	{
		/*{-5.0f, 0.0f, -5.0f},
		{-4.0f, 0.0f, -5.0f},
		{-3.0f, 0.0f, -5.0f},
		{-2.0f, 0.0f, -5.0f},
		{-1.0f, 0.0f, -5.0f},
		{ 0.0f, 0.0f, -5.0f},
		{ 1.0f, 0.0f, -5.0f},
		{ 2.0f, 0.0f, -5.0f},
		{ 3.0f, 0.0f, -5.0f},
		{ 4.0f, 0.0f, -5.0f},
		{ 5.0f, 0.0f, -5.0f},

		{-5.0f, 0.0f, 5.0f},
		{-4.0f, 0.0f, 5.0f},
		{-3.0f, 0.0f, 5.0f},
		{-2.0f, 0.0f, 5.0f},
		{-1.0f, 0.0f, 5.0f},
		{ 0.0f, 0.0f, 5.0f},
		{ 1.0f, 0.0f, 5.0f},
		{ 2.0f, 0.0f, 5.0f},
		{ 3.0f, 0.0f, 5.0f},
		{ 4.0f, 0.0f, 5.0f},
		{ 5.0f, 0.0f, 5.0f},


		{-5.0f, 0.0f, -5.0f},
		{-5.0f, 0.0f, -4.0f},
		{-5.0f, 0.0f, -3.0f},
		{-5.0f, 0.0f, -2.0f},
		{-5.0f, 0.0f, -1.0f},
		{-5.0f, 0.0f, 0.0f},
		{-5.0f, 0.0f, 1.0f},
		{-5.0f, 0.0f, 2.0f},
		{-5.0f, 0.0f, 3.0f},
		{-5.0f, 0.0f, 4.0f},
		{-5.0f ,0.0f, 5.0f},

		{ 5.0f, 0.0f, -5.0f},
		{ 5.0f, 0.0f, -4.0f},
		{ 5.0f, 0.0f, -3.0f},
		{ 5.0f, 0.0f, -2.0f},
		{ 5.0f, 0.0f, -1.0f},
		{ 5.0f, 0.0f, 0.0f},
		{ 5.0f, 0.0f, 1.0f},
		{ 5.0f, 0.0f, 2.0f},
		{ 5.0f, 0.0f, 3.0f},
		{ 5.0f, 0.0f, 4.0f},
		{ 5.0f ,0.0f, 5.0f},*/

		//{-5.0f, 0.0f, -5.0f},
		//{-5.0f, 0.0f, 5.0f},

		//{-4.0f, 0.0f, -5.0f},
		//{-4.0f, 0.0f, 5.0f},

		//{-3.0f, 0.0f, -5.0f},
		//{-3.0f, 0.0f, 5.0f},

		//{-2.0f, 0.0f, -5.0f},
		//{-2.0f, 0.0f, 5.0f},

		//{-1.0f, 0.0f, -5.0f},
		//{-1.0f, 0.0f, 5.0f},

		//{ 0.0f, 0.0f, -5.0f},
		//{ 0.0f, 0.0f, 5.0f},

		//{ 1.0f, 0.0f, -5.0f},
		//{ 1.0f, 0.0f, 5.0f},

		//{ 2.0f, 0.0f, -5.0f},
		//{ 2.0f, 0.0f, 5.0f},

		//{ 3.0f, 0.0f, -5.0f},
		//{ 3.0f, 0.0f, 5.0f},

		//{ 4.0f, 0.0f, -5.0f},
		//{ 4.0f, 0.0f, 5.0f},

		//{ 5.0f, 0.0f, -5.0f},
		//{ 5.0f, 0.0f, 5.0f},


		//{-5.0f, 0.0f, -5.0f},
		//{ 5.0f, 0.0f, -5.0f},

		//{-5.0f, 0.0f, -4.0f},
		//{ 5.0f, 0.0f, -4.0f},

		//{-5.0f, 0.0f, -3.0f},
		//{ 5.0f, 0.0f, -3.0f},

		//{-5.0f, 0.0f, -2.0f},
		//{ 5.0f, 0.0f, -2.0f},

		//{-5.0f, 0.0f, -1.0f},
		//{ 5.0f, 0.0f, -1.0f},

		//{-5.0f, 0.0f, 0.0f},
		//{ 5.0f, 0.0f, 0.0f},

		//{-5.0f, 0.0f, 1.0f},
		//{ 5.0f, 0.0f, 1.0f},

		//{-5.0f, 0.0f, 2.0f},
		//{ 5.0f, 0.0f, 2.0f},

		//{-5.0f, 0.0f, 3.0f},
		//{ 5.0f, 0.0f, 3.0f},

		//{-5.0f, 0.0f, 4.0f},
		//{ 5.0f, 0.0f, 4.0f},

		//{-5.0f ,0.0f, 5.0f},
		//{ 5.0f ,0.0f, 5.0f},
	};


	for (int i = 0; i < 44; i++)
	{
		if(i < 22)
		{
			if(i < 11)
			{
				grid[i].x = -5.0f;
			}
			else
			{
				grid[i].x = 5.0f;
			}

			/*for (int j = -5; j < 5; j++)
			{
				grid[i].z = static_cast<float>(j);
			}*/
		}
		else
		{
			if(i < 33)
			{
				grid[i].z = -5.0f;
			}
			else
			{
				grid[i].z = 5.0f;
			}

			/*for (int j = -5; j < 5; j++)
			{
				grid[i].x = static_cast<float>(j);
			}*/
		} 
		grid[i].y = 0.0f;
	}

	int gridList[22][2] =
	{
		/*{0,11},
		{1,12},
		{2,13},
		{3,14},
		{4,15},
		{5,16},
		{6,17},
		{7,18},
		{8,19},
		{9,20},
		{10,21},

		{22,33},
		{23,34},
		{24,35},
		{25,36},
		{26,37},
		{27,38},
		{28,39},
		{29,40},
		{30,41},
		{31,42},
		{32,43},*/


		/*{0,1},
		{2,3},
		{4,5},
		{6,7},
		{8,9},
		{10,11},
		{12,13},
		{14,15},
		{16,17},
		{18,19},
		{20,21},
		{22,23},
		{24,25},
		{26,27},
		{28,29},
		{30,31},
		{32,33},
		{34,35},
		{36,37},
		{38,39},
		{40,41},
		{42,43},*/

	};

	for (int i = 0; i < 22; i++)
	{
		if (i < 11)
		{
			gridList[i][0] = i;
		}
		else 
		{
			gridList[i][0] = i + 11;
		}

		gridList[i][1] = gridList[i][0] + 11;
	}

	Vector4 xColor =
	{
		0xFF, 0x00, 0x00, 0xFF
	};

	Vector4 zColor =
	{
		0x00, 0x00, 0xFF, 0xFF
	};

	/// </summary>

	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);
		
	// 3Dオブジェクト描画後処理
	Model::PostDraw();

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)

	for (int i = 0; i < 22; i++)
	{
		if (i < 11)
			PrimitiveDrawer::GetInstance()->DrawLine3d(grid[gridList[i][0]], grid[gridList[i][1]], zColor);

		else
			PrimitiveDrawer::GetInstance()->DrawLine3d(grid[gridList[i][0]], grid[gridList[i][1]], xColor);
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
