#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "PrimitiveDrawer.h"
#include "AxisIndicator.h"

#include "directxmath.h"
#include <math.h>
#include <random>
#include <time.h>
using namespace DirectX;

#define PI 3.141592

#pragma region Transform
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
	mat *= scale *= rotation *= translation;
	return mat;
}
#pragma endregion

void GameScene::PartTransform(WorldTransform* worldTransform)
{
	//scale
	//X,Y,Z方向のスケーリングを設定
	Matrix4 matScale = MatrixScale(worldTransform->scale_);

	//Rote
	//X,Y,Z方向の回転を設定
	Matrix4 matRotation = MatrixRotationZ(worldTransform->rotation_);
	matRotation *= MatrixRotationX(worldTransform->rotation_);
	matRotation *= MatrixRotationY(worldTransform->rotation_);

	//translation
	//X,Y,Z方向の平行移動を設定
	Matrix4 matTranslation = MatrixTranslation(worldTransform->translation_);

	worldTransform->matWorld_ = MatrixWorld(matScale, matRotation, matTranslation);
	if(worldTransform->parent_ != nullptr)
		worldTransform->matWorld_ *= worldTransform->parent_->matWorld_;

	worldTransform->TransferMatrix();
}

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

#pragma region ワールドトランスフォーム設定
		//ワールドトランスフォームの初期化
		//大元
		worldTransforms_[PartId::kRoot].Initialize();

		//脊椎
		worldTransforms_[PartId::kSpine].Initialize();
		worldTransforms_[PartId::kSpine].parent_ = &worldTransforms_[PartId::kRoot];
		worldTransforms_[PartId::kSpine].translation_ = { 0,4.5f, 0 };

		//上半身
		//胸
		worldTransforms_[PartId::kChest].Initialize();
		worldTransforms_[PartId::kChest].parent_ = &worldTransforms_[PartId::kSpine];
		worldTransforms_[PartId::kChest].translation_ = { 0, -2.0f, 0 };
		//頭
		worldTransforms_[PartId::kHead].Initialize();
		worldTransforms_[PartId::kHead].parent_ = &worldTransforms_[PartId::kChest];
		worldTransforms_[PartId::kHead].translation_ = { 0, 3.0f, 0 };

		//左腕
		worldTransforms_[PartId::kArmL].Initialize();
		worldTransforms_[PartId::kArmL].parent_ = &worldTransforms_[PartId::kChest];
		worldTransforms_[PartId::kArmL].translation_ = { -2.5f, 0, 0 };
		//右腕
		worldTransforms_[PartId::kArmR].Initialize();
		worldTransforms_[PartId::kArmR].parent_ = &worldTransforms_[PartId::kChest];
		worldTransforms_[PartId::kArmR].translation_ = { 2.5f, 0, 0 };


		//下半身
		//尻
		worldTransforms_[PartId::kHip].Initialize();
		worldTransforms_[PartId::kHip].translation_ = { 0, -4.5f, 0 };
		worldTransforms_[PartId::kHip].parent_ = &worldTransforms_[PartId::kSpine];

		//左脚
		worldTransforms_[PartId::kLegL].Initialize();
		worldTransforms_[PartId::kLegL].parent_ = &worldTransforms_[PartId::kHip];
		worldTransforms_[PartId::kLegL].translation_ = { -2.5f, -2.5f, 0 };
		//右脚
		worldTransforms_[PartId::kLegR].Initialize();
		worldTransforms_[PartId::kLegR].parent_ = &worldTransforms_[PartId::kHip];
		worldTransforms_[PartId::kLegR].translation_ = { 2.5f, -2.5f, 0 };
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
	//デバッグカメラの更新
	debugCamera_->Update();
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

	//キャラクター移動処理
	{
		//キャラクターの移動ベクトル
		Vector3 move = { 0,0,0 };
	
		if (input_->PushKey(DIK_LEFT)) {
			move.x -= 0.2f;
		}
		if (input_->PushKey(DIK_RIGHT)) {
			move.x += 0.2f;
		}
	
		worldTransforms_[PartId::kRoot].translation_ += move;

		//デバッグ用表示
		debugText_->SetPos(50, 150);
		debugText_->Printf(
			"Root:(%f,%f,%f)", worldTransforms_[PartId::kRoot].translation_.x, worldTransforms_[PartId::kRoot].translation_.y, worldTransforms_[PartId::kRoot].translation_.z);
	}

	//上半身回転
	{
		if(input_->PushKey(DIK_U))
		{
			worldTransforms_[PartId::kChest].rotation_.y -= 0.05f;
		}
		else if(input_->PushKey(DIK_I))
		{
			worldTransforms_[PartId::kChest].rotation_.y += 0.05f;
		}
	}

	//下半身回転
	{
		if(input_->PushKey(DIK_J))
		{
			worldTransforms_[PartId::kHip].rotation_.y -= 0.05f;
		}
		else if(input_->PushKey(DIK_K))
		{
			worldTransforms_[PartId::kHip].rotation_.y += 0.05f;
		}
	}

	for (int i = 0; i < PartId::kNumPartId; i++)
	{
		PartTransform(&worldTransforms_[i]);
	}
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

	for (int i = 0; i < PartId::kNumPartId; i++)
	{
		if(i < 2)
		{
			continue;
		}
		model_->Draw(worldTransforms_[i], viewProjection_, textureHandle_);
	}

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
