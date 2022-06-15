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

#pragma region Trasform
Matrix4 GameScene::TransformScale(WorldTransform* worldTransform,Vector3 scale)
{
	worldTransform->scale_ = scale;

	//拡縮
	static Matrix4 matScale = MathUtility::Matrix4Identity();

	matScale = 
	{ worldTransform->scale_.x,0,0,0,
		0,worldTransform->scale_.y,0,0,
		0,0,worldTransform->scale_.z,0,
		0,0,0,1
	};
	return matScale;
}

Matrix4 GameScene::TransformRotation(WorldTransform* worldTransform,Vector3 rotation)
{
	worldTransform->rotation_ = rotation;

	//z回転
	static Matrix4 matZ = MathUtility::Matrix4Identity();
	matZ =
	{ cos(worldTransform->rotation_.z),sin(worldTransform->rotation_.z),0,0,
		-sin(worldTransform->rotation_.z),cos(worldTransform->rotation_.z),0,0,
		0,0,1,0,
		0,0,0,1 };

	//x回転
	static Matrix4 matX = MathUtility::Matrix4Identity();
	matX =
	{ 1,0,0,0,
		0,cos(worldTransform->rotation_.x),sin(worldTransform->rotation_.x),0,
		0,-sin(worldTransform->rotation_.x),cos(worldTransform->rotation_.x),0,
		0,0,0,1 };

	//y回転
	static Matrix4 matY = MathUtility::Matrix4Identity();
	matY =
	{ cos(worldTransform->rotation_.y),0,-sin(worldTransform->rotation_.y),0,
		0,1,0,0,
		sin(worldTransform->rotation_.y),0,cos(worldTransform->rotation_.y),0,
		0,0,0,1 };

	static Matrix4 matRota = MathUtility::Matrix4Identity();

	matRota *= matZ *= matX *= matY;

	return matRota;
}

Matrix4 GameScene::TransformTranslation(WorldTransform* worldTransform,Vector3 translation)
{
	worldTransform->translation_ = translation;

	//平行移動行列を宣言
	static Matrix4 matTrans = MathUtility::Matrix4Identity();

	matTrans =
	{ 
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		worldTransform->translation_.x,worldTransform->translation_.y,worldTransform->translation_.z,1 
	};
	return matTrans;
}
#pragma endregion

#pragma region WorldTransformIntialize
void GameScene::WorldTransformTransfer(WorldTransform* worldTransform,Vector3 scale,Vector3 rotation,Vector3 translation)
{
	worldTransform->matWorld_ 
		*= TransformScale(worldTransform, scale)
		*= TransformRotation(worldTransform, rotation)
		*= TransformTranslation(worldTransform, translation);

	worldTransform->TransferMatrix();

}
#pragma endregion

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete debugCamera_;
	delete model_;
}

void GameScene::Initialize() {

	srand(time(NULL));

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	//モデルの生成
	model_ = Model::Create();

	//範囲forで全てのワールドトランスフォームを順に処理する
	for(WorldTransform& worldTransform : worldTransforms_)
	{
		Vector3 randomRote = 
		{
			static_cast<float>(rand() % 360),//(314 / 100) - PI;// / 180;
			static_cast<float>(rand() % 360),
			static_cast<float>(rand() % 360),
		};

		Vector3 randomTranslation = 
		{
			static_cast<float>(rand() % 20 - 10),
			static_cast<float>(rand() % 20 - 10),
			static_cast<float>(rand() % 20 - 10),
		};

		//ワールドトランスフォームの初期化
		worldTransform.Initialize();

		//scale
		Vector3 scale = { 1.0f,1.0f,1.0f };

		//Rote
		Vector3 radian =
		{
			static_cast<float>(randomRote.x * PI / 180.0f),
			static_cast<float>(randomRote.y * PI / 180.0f),
			static_cast<float>(randomRote.z * PI / 180.0f),
		};

		//X,Y,Z方向の回転を設定
		Vector3 rotation = { radian.x,radian.y,radian.z };

		WorldTransformTransfer(&worldTransform, scale, rotation, randomTranslation);
	}

	//カメラ垂直方向視野を設定
	viewProjection_.fovAngleY = XMConvertToRadians(10.0f);

	//アスペクト比を設定
	viewProjection_.aspectRatio = 1.0f;

	//ニアクリップ距離を設定
	viewProjection_.nearZ = 52.0f;

	//ファークリップ距離を設定
	viewProjection_.farZ = 53.0f;

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 800);

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_ );

	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
}

void GameScene::Update() {
	//デバッグカメラの更新
	debugCamera_->Update();

	//FoV変更処理
	{
		//視野角が広がる
		if (input_->PushKey(DIK_W)) {
			viewProjection_.fovAngleY += 0.01f;
			viewProjection_.fovAngleY = min(viewProjection_.fovAngleY, XM_PI);
		//視野角が狭まる
		}else if (input_->PushKey(DIK_S)) {
			viewProjection_.fovAngleY -= 0.01f;
			viewProjection_.fovAngleY = max(viewProjection_.fovAngleY, 0.01f);
		}

		//行列の再計算
		viewProjection_.UpdateMatrix();

		//デバッグ用表示
		debugText_->SetPos(50, 110);
		debugText_->Printf(
			"fovAngleY(Degree):%f", XMConvertToDegrees(viewProjection_.fovAngleY));
	}

	//クリップ距離変更処理
	{
		//クリップ距離を増減
		if (input_->PushKey(DIK_UP)) {
			viewProjection_.nearZ += 0.1f;
		}else if (input_->PushKey(DIK_DOWN)) {
			viewProjection_.nearZ -= 0.1f;
		}

		//行列の再計算
		viewProjection_.UpdateMatrix();

		//デバッグ用表示
		debugText_->SetPos(50, 130);
		debugText_->Printf(
			"nearZ:%f", XMConvertToDegrees(viewProjection_.nearZ));
	}
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

	//範囲forですべてのワールドトランスフォームを順に処理する
	for (WorldTransform& worldTransform : worldTransforms_)
	{
		model_->Draw(worldTransform, viewProjection_ , textureHandle_);
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
