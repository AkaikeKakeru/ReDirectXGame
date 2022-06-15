#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "PrimitiveDrawer.h"
#include "AxisIndicator.h"

#include <math.h>
#include <random>
#include <time.h>

#define PI 3.141592

#pragma region Trasform
void GameScene::TransformScale(WorldTransform* worldTransform,Vector3 scale)
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

	worldTransform->matWorld_ *= matScale;
}

void GameScene::TransformRotation(WorldTransform* worldTransform,Vector3 rotation)
{
	worldTransform->rotation_ = rotation;

	//z回転
	static Matrix4 matZ =
	{ cos(worldTransform->rotation_.z),sin(worldTransform->rotation_.z),0,0,
		-sin(worldTransform->rotation_.z),cos(worldTransform->rotation_.z),0,0,
		0,0,1,0,
		0,0,0,1 };

	//x回転
	static Matrix4 matX =
	{ 1,0,0,0,
		0,cos(worldTransform->rotation_.x),sin(worldTransform->rotation_.x),0,
		0,-sin(worldTransform->rotation_.x),cos(worldTransform->rotation_.x),0,
		0,0,0,1 };

	//y回転
	static Matrix4 matY =
	{ cos(worldTransform->rotation_.y),0,-sin(worldTransform->rotation_.y),0,
		0,1,0,0,
		sin(worldTransform->rotation_.y),0,cos(worldTransform->rotation_.y),0,
		0,0,0,1 };

	static Matrix4 matRota = MathUtility::Matrix4Identity();

	matRota *= matZ *= matX *= matY;

	worldTransform->matWorld_ *= matRota;
}

void GameScene::TransformTranslation(WorldTransform* worldTransform,Vector3 translation)
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

	worldTransform->matWorld_ *= matTrans;
}
#pragma endregion

#pragma region WorldTransformIntialize
void GameScene::WorldTransformTransfer(WorldTransform* worldTransform,Vector3 scale,Vector3 rotation,Vector3 translation)
{
	TransformScale(worldTransform, scale);
	TransformRotation(worldTransform, rotation);
	TransformTranslation(worldTransform, translation);

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
		//X,Y,Z方向のスケーリングを設定
		//worldTransform.scale_ = { 1.0f,1.0f,1.0f };
		Vector3 scale = { 1.0f,1.0f,1.0f };

		//Rote
		//float radian = 45 * PI / 180.0;
		Vector3 radian =
		{
			static_cast<float>(randomRote.x * PI / 180.0f),
			static_cast<float>(randomRote.y * PI / 180.0f),
			static_cast<float>(randomRote.z * PI / 180.0f),
		};

		//X,Y,Z方向の回転を設定
		//worldTransform.rotation_ = { radian.x,radian.y,radian.z };
		Vector3 rotation = { radian.x,radian.y,radian.z };

		WorldTransformTransfer(&worldTransform, scale, rotation, randomTranslation);
	}

	//カメラ視点座標を設定
	//viewProjection_.eye = {0,0,-10};

	//カメラ注視点座標を設定
	viewProjection_.target = {10,0,0};

	//カメラ上方向ベクトルを設定 (右上45度)
	viewProjection_.up = { cosf(PI / 4.0f), sinf(PI / 4.0f), 0.0f };

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

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

	//視点移動処理
	{
		//視点ベクトル
		Vector3 move = { 0,0,0 };

		//視点の移動速さ
		const float kEyeSpeed = 0.2f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_W)) {
			move.z += kEyeSpeed;
		}else if (input_->PushKey(DIK_S)) {
			move.z -= kEyeSpeed;
		}

		//視点移動(ベクトルの加算)
		viewProjection_.eye += move;

		//行列の再計算
		viewProjection_.UpdateMatrix();

		//デバッグ用表示
		debugText_->SetPos(50, 50);
		debugText_->Printf(
			"eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	}

	//注視点移動処理
	{
		//注視点ベクトル
		Vector3 move = { 0,0,0 };

		//視点の移動速さ
		const float kTargetSpeed = 0.2f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_LEFT)) {
			move.x -= kTargetSpeed;
		}else if (input_->PushKey(DIK_RIGHT)) {
			move.x += kTargetSpeed;
		}

		//視点移動(ベクトルの加算)
		viewProjection_.target += move;

		//行列の再計算
		viewProjection_.UpdateMatrix();

		//デバッグ用表示
		debugText_->SetPos(50, 70);
		debugText_->Printf(
			"target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);
	}

	//上方向回転処理
	{
		//上方向の回転速さ[ラジアン/frame]
		const float kUpSpeed = 0.05f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_SPACE)) {
			viewAngle += kUpSpeed;
			// 2πを超えたら0に戻す
			viewAngle = fmodf(viewAngle, PI * 2.0f);
		}

		//上方向ベクトルを計算(半径1の円周上の座標)
		viewProjection_.up = {cosf(viewAngle), sinf(viewAngle), 0.0f};

		//行列の再計算
		viewProjection_.UpdateMatrix();

		//デバッグ用表示
		debugText_->SetPos(50, 90);
		debugText_->Printf(
			"up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);
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
		model_->Draw(worldTransform, viewProjection_ /*debugCamera_->GetViewProjection()*/, textureHandle_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)

	//for (int i = 0; i < 12; i++)
	//{
	//	PrimitiveDrawer::GetInstance()->DrawLine3d(vertex[edgeList[i][0]], vertex[edgeList[i][1]], vecColor);
	//}

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
