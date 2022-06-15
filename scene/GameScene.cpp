#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "PrimitiveDrawer.h"
#include "AxisIndicator.h"

#pragma region WorldTransformIntialize
void GameScene::WorldTransformIntialize(WorldTransform* worldTransform,Vector3 scale,Vector3 rotation,Vector3 translation)
{
	worldTransform->scale_ = scale;
	worldTransform->rotation_ = rotation;
	worldTransform->translation_ = translation;

	Matrix4 Mat[] =
	{
		//拡縮
		{ scale.x,0,0,0,
		0,scale.y,0,0,
		0,0,scale.z,0,
		0,0,0,1 } ,

		//z回転
		{ cos(rotation.z),sin(rotation.z),0,0,
		-sin(rotation.z),cos(rotation.z),0,0,
		0,0,1,0,
		0,0,0,1 },
		//x回転
		{ 1,0,0,0,
		0,cos(rotation.x),sin(rotation.x),0,
		0,-sin(rotation.x),cos(rotation.x),0,
		0,0,0,1 },
		//y回転
		{ cos(rotation.y),0,-sin(rotation.y),0,
		0,1,0,0,
		sin(rotation.y),0,cos(rotation.y),0,
		0,0,0,1 },

		//平行
		{ 1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		translation.x,translation.y,translation.z,1 },
	};

	for (int i = 0; i < 5; i++)
	{
		worldTransform->matWorld_ *= Mat[i];
	}
	worldTransform->TransferMatrix();
}
#pragma endregion

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

	//gridを宣言初期化
	Vector3 grid[44] = {};

	for (int i = 0; i < 44; i++)
	{
		//5～-5を代入する
		static int j = 5;

		//iは22未満か
		if (i < 22)
		{
			//11未満か
			if (i < 11)
			{
				//z座標を-5に
				grid[i].z = -5.0f;
			}
			else
			{
				//z座標を5に
				grid[i].z = 5.0f;
			}
			//x座標は5～-5に
			grid[i].x = static_cast<float>(j);
		}
		//上と処理はほぼ同じ
		else
		{
			if (i < 33)
			{
				grid[i].x = -5.0f;
			}
			else
			{
				grid[i].x = 5.0f;
			}
			grid[i].z = static_cast<float>(j);
		}
		//y座標は一律0に
		grid[i].y = 0.0f;

		//jは-5を下回ったか
		if (j <= -5)
		{
			//5に戻す
			j = 5;
		}
		else
		{
			//jを1減らす
			j--;
		}
	}

	//gridListを宣言初期化
	int gridList[22][2] = {};

	for (int i = 0; i < 22; i++)
	{
		//iは11未満か
		if (i < 11)
		{
			//始点はgridのi番目の頂点
			gridList[i][0] = i;
		}
		else
		{
			//始点はgridの(i + 11)番目の頂点
			gridList[i][0] = i + 11;
		}
		//終点はgridの(始点 + 11)番目の頂点
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
