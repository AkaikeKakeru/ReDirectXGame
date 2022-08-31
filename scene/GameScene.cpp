#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "PrimitiveDrawer.h"
#include "AxisIndicator.h"
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete debugCamera_;
	delete railCamera_;

	delete model_;
	delete modelSkydome_;
	delete modelPlayer_;
	delete modelEnemy_;
}

void GameScene::Initialize() {

	//レティクルのテクスチャ
	TextureManager::Load("cursor.png");

	loadEnemyPopData();

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//レールカメラの生成
	railCamera_ = new RailCamera();

	railCamera_->Initialize(Vector3(0, 0, -40),
		Vector3(
			XMConvertToRadians(0.0f),
			XMConvertToRadians(0.0f),
			XMConvertToRadians(0.0f)));

	//モデルの生成
	model_ = Model::Create();

#pragma region Skydome
	//天球モデル生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	//天球の生成
	skydome_ = std::make_unique<Skydome>();

	//天球の初期化
	skydome_->Initialize(modelSkydome_);
#pragma endregion

#pragma region Player
	//自キャラモデル生成
	modelPlayer_ = Model::CreateFromOBJ("plane", true);

	//自キャラの生成
	player_ = std::make_unique<Player>();

	//自キャラの初期化
	player_->Initialize(modelPlayer_,model_);

	player_->SetWorldParent(&railCamera_->GetWorldTransform());

#pragma endregion

#pragma region Enemy
	//敵キャラモデル生成
	modelEnemy_ = Model::CreateFromOBJ("plane", true);

	//敵キャラの生成
	PopEnemy(Vector3(2.0f, 5.0f, 50.0f));

	for (const std::unique_ptr<Enemy>& enemy
	: enemys_) {
	enemy->SetGameScene(this);
	}


	//敵キャラに自キャラのアドレスを渡す
	//enemy_->SetPlayer(player_.get());
	for (const std::unique_ptr<Enemy>& enemy
		: enemys_) {
		enemy->SetPlayer(player_.get());
	}
#pragma endregion

	//衝突マネージャー
	collisionManager_ = std::make_unique<CollisionManager>();

#pragma region カメラ設定

	////カメラ垂直方向視野を設定
	//viewProjection_.fovAngleY = XMConvertToRadians(10.0f);

	////アスペクト比を設定
	//viewProjection_.aspectRatio = 1.0f;

	////ニアクリップ距離を設定
	//viewProjection_.nearZ = 10.0f;

	////ファークリップ距離を設定
	//viewProjection_.farZ = 20.0f;

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
#pragma endregion

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 800);
	;

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_ /*&debugCamera_->GetViewProjection()*/);

	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_/*&debugCamera_->GetViewProjection()*/);
}

void GameScene::Update() {
#pragma region デバッグ
#ifdef  _DEBUG
	//if (input_->TriggerKey(DIK_R)) {
	//	isDebugCameraActive_ = !isDebugCameraActive_;
	//}

	//if (isDebugCameraActive_) {
	//	//デバッグ用表示
	//	debugText_->SetPos(50, 70);
	//	debugText_->Printf(
	//		"ON");
	//}
	//else {
	//	//デバッグ用表示
	//	debugText_->SetPos(50, 70);
	//	debugText_->Printf(
	//		"OFF");
	//}
	////デバッグ用表示
	//debugText_->SetPos(50, 50);
	//debugText_->Printf(
	//	"Rkey -> ON:OFFswitch");
#endif //  _DEBUG

	//デバッグ用表示
	debugText_->SetPos(50, 300);
	debugText_->Printf(
		"ARROWkey -> Move");

	//debugText_->SetPos(50, 320);
	//debugText_->Printf(
	//	"UorJkey -> leftRota");

	//debugText_->SetPos(50, 340);
	//debugText_->Printf(
	//	"IorKkey -> RightRota");

	debugText_->SetPos(50, 320);
	debugText_->Printf(
		"SPACEkey -> Shot");

	//if (isDebugCameraActive_) {
	//	//デバッグカメラの更新
	//	debugCamera_->Update();

	//	//ビュー行列とプロジェクション行列を取得
	//	viewProjection_.matView = debugCamera_->GetViewProjection().matView;
	//	viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

	//	//転送
	//	viewProjection_.TransferMatrix();
	//}
	//else {
	//	//再計算と転送
	//	viewProjection_.UpdateMatrix();
	//	viewProjection_.TransferMatrix();
	//}
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

	//レールカメラ
	railCamera_->Update();

	this->viewProjection_ = railCamera_->GetViewProjection();
#pragma region Skydome
	skydome_->Update();
#pragma endregion

#pragma region キャラクター

#pragma region Player
	player_->Update();
#pragma endregion

#pragma region Enemy
	//デスフラグの立った弾を削除
	enemyBullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
		});

	//デスフラグの立った敵を削除
	enemys_.remove_if([](std::unique_ptr<Enemy>& enemy) {
		return enemy->IsDead();
		});

	//敵更新
	for (const std::unique_ptr<Enemy>& enemy
		: enemys_) {
		UpdateEnemypopCommands();

		enemy->Update();
	}
	//enemy_->Update();

	//弾更新
	for (std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullets_) {
		enemyBullet->SetPlayer(player_.get());
		enemyBullet->Update();
	}
#pragma endregion

	//衝突マネージャーを動かす
	RunCollisionManager();

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

#pragma region Skydome
	skydome_->Draw(viewProjection_);
#pragma endregion

#pragma region キャラクター
	player_->Draw(viewProjection_);

	//敵描画
	for (const std::unique_ptr<Enemy>& enemy
		: enemys_) {
		enemy->Draw(viewProjection_);
	}
	//enemy_->Draw(viewProjection_);

	//弾描画
	for (std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullets_) {
		enemyBullet->Draw(viewProjection_);
	}
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
	player_->DrawUI();


	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::PopEnemy(Vector3 position) {
	//敵キャラの生成
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();

	newEnemy->Intialize(modelEnemy_,
		model_,
		position,
		Vector3(0, 0, -0.1f),
		Vector3(-0.1f, 0.1f, 0));

	enemys_.push_back(std::move(newEnemy));

	for (const std::unique_ptr<Enemy>& enemy
		: enemys_) {
		enemy->SetGameScene(this);
	}


	//敵キャラに自キャラのアドレスを渡す
	//enemy_->SetPlayer(player_.get());
	for (const std::unique_ptr<Enemy>& enemy
		: enemys_) {
		enemy->SetPlayer(player_.get());
	}
};

void GameScene::AddEnemyBullet(std::unique_ptr<EnemyBullet> enemyBullet) {
	//リストに追加
	enemyBullets_.push_back(std::move(enemyBullet));
};

void GameScene::RunCollisionManager() {
	collisionManager_->ClearColliderList();

	//コライダーをリストに登録
	collisionManager_->SetCollider(player_.get());

	//敵全てについて
	for (const std::unique_ptr<Enemy>& enemy
		: enemys_) {
		collisionManager_->SetCollider(enemy.get());
	}
	

	//自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>&
		playerBullets = player_->GetBullet();
	//敵弾リストの取得
	//const std::list<std::unique_ptr<EnemyBullet>>&
	//	enemyBullets = enemy_->GetBullet();
	//const std::list<std::unique_ptr<EnemyBullet>>&
	//	enemyBullets = enemy_->GetBullet();


	//自弾全てについて
	for (const std::unique_ptr<PlayerBullet>& playerBullet
		: playerBullets) {
		collisionManager_->SetCollider(playerBullet.get());
	}
	//敵弾全てについて
	for (const std::unique_ptr<EnemyBullet>& enemyBullet
		: enemyBullets_) {
		collisionManager_->SetCollider(enemyBullet.get());
	}

	collisionManager_->CheckAllCollision();
};

void GameScene::loadEnemyPopData() {
	//ファイルを開く
	std::ifstream file;

	file.open("C:/KamataEngine/DirectXGame/Resources/enemyPop.csv");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	//ファイルを閉じる
	file.close();
};

void GameScene::UpdateEnemypopCommands() {
	//待機処理
	if (isWait_) {
		waitTimer_--;
		if (waitTimer_ <= 0) {
			//待機完了
			isWait_ = false;
		}
		return;
	}


	//1行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		//"//"から始まるコメント
		if (word.find("//") == 0) {
			//コメント行は飛ばす
			continue;
		}

		//POPコマンド
		if (word.find("POP") == 0) {
			//x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());
	
			//y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());
		
			//z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			//敵を発生
			PopEnemy(Vector3(x, y, z));
		}
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			//待ち時間
			int32_t waitTime = atoi(word.c_str());

			//待機時間
			isWait_ = true;
			waitTimer_ = waitTime;

			//コマンドループから抜ける
			break;
		}
	}
};