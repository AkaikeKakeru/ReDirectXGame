﻿#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "PrimitiveDrawer.h"
#include "AxisIndicator.h"

#pragma region Transform関数
/*
Matrix4 GameScene::MatrixScale(Vector3 scale)
{
	Matrix4 mat = MathUtility::Matrix4Identity();
	mat =
	{ scale.x,0,0,0,
		0,scale.y,0,0,
		0,0,scale.z,0,
		0,0,0,1 };
	return mat;
}

Matrix4 GameScene::MatrixRotationX(Vector3 rotation)
{
	Matrix4 mat = MathUtility::Matrix4Identity();
	mat =
	{ 1,0,0,0,
		0,cos(rotation.x),sin(rotation.x),0,
		0,-sin(rotation.x),cos(rotation.x),0,
		0,0,0,1 };
	return mat;
}

Matrix4 GameScene::MatrixRotationY(Vector3 rotation)
{
	Matrix4 mat = MathUtility::Matrix4Identity();
	mat =
	{ cos(rotation.y),0,-sin(rotation.y),0,
		0,1,0,0,
		sin(rotation.y),0,cos(rotation.y),0,
		0,0,0,1 };
	return mat;
}

Matrix4 GameScene::MatrixRotationZ(Vector3 rotation)
{
	Matrix4 mat = MathUtility::Matrix4Identity();
	mat =
	{ cos(rotation.z),sin(rotation.z),0,0,
		-sin(rotation.z),cos(rotation.z),0,0,
		0,0,1,0,
		0,0,0,1 };
	return mat;
}

Matrix4 GameScene::MatrixTranslation(Vector3 translation)
{
	Matrix4 mat = MathUtility::Matrix4Identity();
	mat =
	{ 1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		translation.x,translation.y,translation.z,1 };
	return mat;
}

Matrix4 GameScene::MatrixWorld(Matrix4 scale,Matrix4 rotation, Matrix4 translation)
{
	Matrix4 mat = MathUtility::Matrix4Identity();
	mat *= scale;
	mat *= rotation;
	mat *= translation;
	return mat;
}*/
#pragma endregion

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete debugCamera_;
	delete model_;
	delete player_;
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

#pragma region Player
	//自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	player_->Intialize(model_, textureHandle_);
#pragma endregion

#pragma region カメラ設定

	////カメラ垂直方向視野を設定
	//viewProjection_.fovAngleY = XMConvertToRadians(10.0f);

	////アスペクト比を設定
	//viewProjection_.aspectRatio = 1.0f;

	////ニアクリップ距離を設定
	//viewProjection_.nearZ = 52.0f;

	////ファークリップ距離を設定
	//viewProjection_.farZ = 53.0f;

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
#pragma endregion

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 800);

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_ /*&debugCamera_->GetViewProjection()*/);

	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
}

void GameScene::Update() {
#pragma region デバッグ
#ifdef  _DEBUG
	if (input_->TriggerKey(DIK_R)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif //  _DEBUG

	if (isDebugCameraActive_) {
		//デバッグカメラの更新
		debugCamera_->Update();

		//ビュー行列とプロジェクション行列を取得
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

		//転送
		viewProjection_.TransferMatrix();

		//デバッグ用表示
		debugText_->SetPos(50, 70);
		debugText_->Printf(
			"ON");
	}
	else {
		//再計算と転送
		viewProjection_.UpdateMatrix();
		viewProjection_.TransferMatrix();

		//デバッグ用表示
		debugText_->SetPos(50, 70);
		debugText_->Printf(
			"OFF");
	}

	//デバッグ用表示
	debugText_->SetPos(50, 50);
	debugText_->Printf(
		"Rkey -> ON:OFFswitch");
#pragma endregion

#pragma region 視点処理
	//	{
	//	//視点移動処理
	//	{
	//		//視点ベクトル
	//		Vector3 move = { 0,0,0 };
	//
	//		//視点の移動速さ
	//		const float kEyeSpeed = 0.2f;
	//
	//		//押した方向で移動ベクトルを変更
	//		if (input_->PushKey(DIK_W)) {
	//			move.z += kEyeSpeed;
	//		}else if (input_->PushKey(DIK_S)) {
	//			move.z -= kEyeSpeed;
	//		}
	//
	//		//視点移動(ベクトルの加算)
	//		viewProjection_.eye += move;
	//
	//		//行列の再計算
	//		viewProjection_.UpdateMatrix();
	//
	//		//デバッグ用表示
	//		debugText_->SetPos(50, 50);
	//		debugText_->Printf(
//			"eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
//	}
//
//	//注視点移動処理
//	{
//		//注視点ベクトル
//		Vector3 move = { 0,0,0 };
//
//		//視点の移動速さ
//		const float kTargetSpeed = 0.2f;
//
//		//押した方向で移動ベクトルを変更
//		if (input_->PushKey(DIK_LEFT)) {
//			move.x -= kTargetSpeed;
//		}else if (input_->PushKey(DIK_RIGHT)) {
//			move.x += kTargetSpeed;
//		}
//
//		//視点移動(ベクトルの加算)
//		viewProjection_.target += move;
//
//		//行列の再計算
//		viewProjection_.UpdateMatrix();
//
//		//デバッグ用表示
//		debugText_->SetPos(50, 70);
//		debugText_->Printf(
//			"target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);
//	}
//
//	//上方向回転処理
//	{
//		//上方向の回転速さ[ラジアン/frame]
//		const float kUpSpeed = 0.05f;
//
//		//押した方向で移動ベクトルを変更
//		if (input_->PushKey(DIK_SPACE)) {
//			viewAngle += kUpSpeed;
//			// 2πを超えたら0に戻す
//			viewAngle = fmodf(viewAngle, PI * 2.0f);
//		}
//
//		//上方向ベクトルを計算(半径1の円周上の座標)
//		viewProjection_.up = {cosf(viewAngle), sinf(viewAngle), 0.0f};
//
//		//行列の再計算
//		viewProjection_.UpdateMatrix();
//
//		//デバッグ用表示
//		debugText_->SetPos(50, 90);
//		debugText_->Printf(
//			"up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);
//	}
//	}
#pragma endregion 

#pragma region 視野範囲設定
	//	//FoV変更処理
	//	{
	//		//視野角が広がる
	//		if (input_->PushKey(DIK_W)) {
	//			viewProjection_.fovAngleY += 0.01f;
	//			viewProjection_.fovAngleY = min(viewProjection_.fovAngleY, XM_PI);
	//		//視野角が狭まる
	//		}else if (input_->PushKey(DIK_S)) {
	//			viewProjection_.fovAngleY -= 0.01f;
	//			viewProjection_.fovAngleY = max(viewProjection_.fovAngleY, 0.01f);
	//		}
	//
	//		//行列の再計算
	//		viewProjection_.UpdateMatrix();
	//
	//		//デバッグ用表示
	//		debugText_->SetPos(50, 110);
	//		debugText_->Printf(
	//			"fovAngleY(Degree):%f", XMConvertToDegrees(viewProjection_.fovAngleY));
	//	}
	//
	//	//クリップ距離変更処理
	//	{
	//		//クリップ距離を増減
	//		if (input_->PushKey(DIK_UP)) {
	//			viewProjection_.nearZ += 0.1f;
	//		}else if (input_->PushKey(DIK_DOWN)) {
	//			viewProjection_.nearZ -= 0.1f;
	//		}
	//
	//		//行列の再計算
	//		viewProjection_.UpdateMatrix();
	//
	//		//デバッグ用表示
	//		debugText_->SetPos(50, 130);
	//		debugText_->Printf(
	//			"nearZ:%f", XMConvertToDegrees(viewProjection_.nearZ));
	//	}
#pragma endregion

#pragma region キャラクター

#pragma region Player
	player_->Update();
#pragma endregion

#pragma region 半身回転
	//	//上半身回転
	//	{
	//		if(input_->PushKey(DIK_U))
	//		{
	//			worldTransform[PartId::kChest].rotation_.y -= 0.05f;
	//		}
	//		else if(input_->PushKey(DIK_I))
	//		{
	//			worldTransform[PartId::kChest].rotation_.y += 0.05f;
	//		}
	//	}
	//
	//	//下半身回転
	//	{
	//		if(input_->PushKey(DIK_J))
	//		{
	//			worldTransform[PartId::kHip].rotation_.y -= 0.05f;
	//		}
	//		else if(input_->PushKey(DIK_K))
	//		{
	//			worldTransform[PartId::kHip].rotation_.y += 0.05f;
	//		}
	//	}
	//
	//	for (int i = 0; i < PartId::kNumPartId; i++)
	//	{
	//		PartTransform(&worldTransform[i]);
	//	}
#pragma endregion
#pragma endregion
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

#pragma region キャラクター
	player_->Draw(viewProjection_);
#pragma endregion

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)

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