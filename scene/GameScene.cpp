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

#pragma region ワールドトランスフォーム設定

	

	//範囲forで全てのワールドトランスフォームを順に処理する
	/*for(WorldTransform& worldTransform : worldTransforms_)
	{*/
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
		//親(0番)
		worldTransforms_[0].Initialize();

		//子(1番)
		worldTransforms_[1].Initialize();
		worldTransforms_[1].translation_ = { 0, 4.5f, 0 };
		worldTransforms_[1].parent_ = &worldTransforms_[0];

		////scale
		////X,Y,Z方向のスケーリングを設定
		//worldTransform.scale_ = { 1.0f,1.0f,1.0f };

		////スケーリング行列を宣言
		//Matrix4 matScale;

		//matScale =
		//{
		//	worldTransform.scale_.x,0,0,0,
		//	0,worldTransform.scale_.y,0,0,
		//	0,0,worldTransform.scale_.z,0,
		//	0,0,0,1 
		//};


		////Rote
		////float radian = 45 * PI / 180.0;
		//Vector3 radian =
		//{
		//	static_cast<float>(randomRote.x * PI / 180.0f),
		//	static_cast<float>(randomRote.y * PI / 180.0f),
		//	static_cast<float>(randomRote.z * PI / 180.0f),
		//};

		////X,Y,Z方向の回転を設定
		//worldTransform.rotation_ = { radian.x,radian.y,radian.z };

		////合成用回転行列を宣言
		//Matrix4 matRot;

		////各軸用回転行列を宣言
		//Matrix4 matRotX, matRotY, matRotZ;

		//matRotX =
		//{ 
		//	1,0,0,0,
		//	0,cos(worldTransform.rotation_.x),sin(worldTransform.rotation_.x),0,
		//	0,-sin(worldTransform.rotation_.x),cos(worldTransform.rotation_.x),0,
		//	0,0,0,1 
		//};

		//matRotY =
		//{ 
		//	cos(worldTransform.rotation_.y),0,-sin(worldTransform.rotation_.y),0,
		//	0,1,0,0,
		//	sin(worldTransform.rotation_.y),0,cos(worldTransform.rotation_.y),0,
		//	0,0,0,1 
		//};

		//matRotZ =
		//{ 
		//	cos(worldTransform.rotation_.z),sin(worldTransform.rotation_.z),0,0,
		//	-sin(worldTransform.rotation_.z),cos(worldTransform.rotation_.z),0,0,
		//	0,0,1,0,
		//	0,0,0,1 
		//};

		////各軸の回転行列を合成
		//matRot = matRotZ *= matRotX *= matRotY;


		////translation
		////X,Y,Z方向の平行移動を設定
		//worldTransform.translation_ = 
		//{ 
		//	randomTranslation.x,
		//	randomTranslation.y,
		//	randomTranslation.z,
		//};

		////平行移動行列を宣言
		//Matrix4 matTrans = MathUtility::Matrix4Identity();

		//matTrans =
		//{ 
		//	1,0,0,0,
		//	0,1,0,0,
		//	0,0,1,0,
		//	worldTransform.translation_.x,worldTransform.translation_.y,worldTransform.translation_.z,1 
		//};

		////行列の合成
		////ワールドトランスフォーム行列

		//worldTransform.matWorld_ =
		//{
		//	1,0,0,0,
		//	0,1,0,0,
		//	0,0,1,0,
		//	0,0,0,1,
		//};

		//worldTransform.matWorld_ *= matScale;
		//worldTransform.matWorld_ *= matRot;
		//worldTransform.matWorld_ *= matTrans;

		////行列の転送
		//worldTransform.TransferMatrix();
	//}
#pragma endregion

#pragma region カメラ設定

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
			move.x += 0.2f;
		}else if (input_->PushKey(DIK_RIGHT)) {
			move.x -= 0.2f;
		}
	
		worldTransforms_[0].translation_ += move;


			//scale
			//X,Y,Z方向のスケーリングを設定
			worldTransforms_[0].scale_ = {1.0f,1.0f,1.0f};

			//スケーリング行列を宣言
			Matrix4 matScale;

			matScale =
			{
				worldTransforms_[0].scale_.x,0,0,0,
				0,worldTransforms_[0].scale_.y,0,0,
				0,0,worldTransforms_[0].scale_.z,0,
				0,0,0,1 
			};

			//Rote
			float radian = 45 * PI / 180.0;

			//X,Y,Z方向の回転を設定
			worldTransforms_[0].rotation_ = {radian,radian,radian};

			//合成用回転行列を宣言
			Matrix4 matRot;

			//各軸用回転行列を宣言
			Matrix4 matRotX, matRotY, matRotZ;

			matRotX =
			{ 
				1,0,0,0,
				0,cos(worldTransforms_[0].rotation_.x),sin(worldTransforms_[0].rotation_.x),0,
				0,-sin(worldTransforms_[0].rotation_.x),cos(worldTransforms_[0].rotation_.x),0,
				0,0,0,1 
			};

			matRotY =
			{ 
				cos(worldTransforms_[0].rotation_.y),0,-sin(worldTransforms_[0].rotation_.y),0,
				0,1,0,0,
				sin(worldTransforms_[0].rotation_.y),0,cos(worldTransforms_[0].rotation_.y),0,
				0,0,0,1 
			};

			matRotZ =
			{ 
				cos(worldTransforms_[0].rotation_.z),sin(worldTransforms_[0].rotation_.z),0,0,
				-sin(worldTransforms_[0].rotation_.z),cos(worldTransforms_[0].rotation_.z),0,0,
				0,0,1,0,
				0,0,0,1 
			};

			//各軸の回転行列を合成
			matRot = matRotZ *= matRotX *= matRotY;


		//平行移動行列を宣言
			Matrix4 matTrans = MathUtility::Matrix4Identity();

			matTrans =
			{ 
				1,0,0,0,
				0,1,0,0,
				0,0,1,0,
				worldTransforms_[0].translation_.x,worldTransforms_[0].translation_.y,worldTransforms_[0].translation_.z,1 
			};

		worldTransforms_[0].matWorld_ = MathUtility::Matrix4Identity();

		worldTransforms_[0].matWorld_ *= matScale;
		worldTransforms_[0].matWorld_ *= matRot;
		worldTransforms_[0].matWorld_ *= matTrans;
		
		worldTransforms_[0].TransferMatrix();

					//デバッグ用表示
					debugText_->SetPos(50, 150);
			debugText_->Printf(
			"Root:(%f,%f,%f)", worldTransforms_[0].translation_.x, worldTransforms_[0].translation_.y, worldTransforms_[0].translation_.z);
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

	//範囲forですべてのワールドトランスフォームを順に処理する
	//for (WorldTransform& worldTransform : worldTransforms_)
	//{
	//	model_->Draw(worldTransform, viewProjection_ /*debugCamera_->GetViewProjection()*/, textureHandle_);
	//}

	model_->Draw(worldTransforms_[0], viewProjection_, textureHandle_);
	//model_->Draw(worldTransforms_[1], viewProjection_, textureHandle_);

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
