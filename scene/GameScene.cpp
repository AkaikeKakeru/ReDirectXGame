﻿#include "GameScene.h"
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

	//モデルの生成
	model_ = Model::Create();
#pragma region Player
	//自キャラの生成
	player_ = std::make_unique<Player>();

	//自キャラの初期化
	player_->Intialize(model_);
#pragma endregion

#pragma region Enemy
	//敵キャラの生成
	enemy_ = std::make_unique<Enemy>();

	Vector3 enePos(2.0f, 5.0f, 50.0f);

	//敵キャラの初期化
	enemy_->Intialize(model_,
		enePos,
		Vector3(0, 0, -0.1f),
		Vector3(-0.1f, 0.1f, 0));

	//敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_.get());
#pragma endregion

	//衝突マネージャー
	collisionManager_ = std::make_unique<CollisionManager>();

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

	if (isDebugCameraActive_) {
		//デバッグ用表示
		debugText_->SetPos(50, 70);
		debugText_->Printf(
			"ON");
	}
	else {
		//デバッグ用表示
		debugText_->SetPos(50, 70);
		debugText_->Printf(
			"OFF");
	}
	//デバッグ用表示
	debugText_->SetPos(50, 50);
	debugText_->Printf(
		"Rkey -> ON:OFFswitch");
#endif //  _DEBUG

	//デバッグ用表示
	debugText_->SetPos(50, 300);
	debugText_->Printf(
		"ARROWkey -> Move");

	debugText_->SetPos(50, 320);
	debugText_->Printf(
		"UorJkey -> leftRota");

	debugText_->SetPos(50, 340);
	debugText_->Printf(
		"IorKkey -> RightRota");

	debugText_->SetPos(50, 360);
	debugText_->Printf(
		"SPACEkey -> Shot");

	if (isDebugCameraActive_) {
		//デバッグカメラの更新
		debugCamera_->Update();

		//ビュー行列とプロジェクション行列を取得
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

		//転送
		viewProjection_.TransferMatrix();
	}
	else {
		//再計算と転送
		viewProjection_.UpdateMatrix();
		viewProjection_.TransferMatrix();
	}
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

	CheckAllCollision();
#pragma region Player
	player_->Update();
#pragma endregion

#pragma region Enemy
	enemy_->Update();
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

	enemy_->Draw(viewProjection_);
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


// 衝突判定と応答
void GameScene::CheckAllCollision() {
	//衝突対象AとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>&
		playerBullets = player_->GetBullet();
	//敵弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>&
		enemyBullets = enemy_->GetBullet();

#pragma region 自キャラと敵弾の当たり判定
	//自キャラと敵弾全ての当たり判定
	//for (const std::unique_ptr<EnemyBullet>& enemyBullet 
	//	: enemyBullets){
	//	//ペアの衝突判定
	//	CheckCollisionPair(player_,enemyBullet.get());
	//}
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	//for (const std::unique_ptr<PlayerBullet>&playerBullet 
	//	: playerBullets){
	//	//ペアの衝突判定
	//	CheckCollisionPair(enemy_,playerBullet.get());
	//}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	//自弾と敵弾全ての当たり判定
	//for (const std::unique_ptr<PlayerBullet>& playerBullet
	//	: playerBullets) {
	//	for (const std::unique_ptr<EnemyBullet>& enemyBullet
	//		: enemyBullets) {
	//		//ペアの衝突判定
	//		CheckCollisionPair(playerBullet.get(),enemyBullet.get());
	//	}
	//}
#pragma endregion

	//コライダー
	std::list<Collider*> colliders_;
	//コライダーをリストに登録
	colliders_.push_back(player_.get());
	colliders_.push_back(enemy_.get());
	//自弾全てについて
	for (const std::unique_ptr<PlayerBullet>& playerBullet
		: playerBullets) {
		colliders_.push_back(playerBullet.get());
	}
	//敵弾全てについて
	for (const std::unique_ptr<EnemyBullet>& enemyBullet
		: enemyBullets) {
		colliders_.push_back(enemyBullet.get());
	}

	//リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA){
		Collider* colA = *itrA;

		//イテレータBはイテレータAの次の要素から回す(重複判定を回避)
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != colliders_.end(); ++itrB){
			Collider* colB = *itrB;

			//ペアの衝突判定
			CheckCollisionPair(colA,colB);
		}
	}
};

// コライダー2つの衝突判定と応答
void GameScene::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	//衝突フィルタリング
	if (
		(colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) == 0 ||
		(colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask()) == 0
		){
		return;
	}
	
	//コライダーのワールド座標を取得
	Vector3 posA = colliderA->GetWorldPosition();
	Vector3 posB = colliderB->GetWorldPosition();

	float rA = colliderA->GetRadius();
	float rB = colliderB->GetRadius();

	if (myVector3_.CollisionAlgorithm(posA, rA, posB, rB) == true) {
	
		//自弾の衝突時コールバックを呼び出す
		colliderA->OnCollision();
		//敵弾の衝突時コールバックを呼び出す
		colliderB->OnCollision();
	}
};
