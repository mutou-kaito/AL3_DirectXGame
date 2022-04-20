#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include<random>

using namespace DirectX;
using namespace std;

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete model_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//追加分の代入
	TextureHandle_ = TextureManager::Load("AL3_01_02\\mario.jpg");
	model_ = Model::Create();
	
	//乱数シードの生成器
	random_device seed_gen;
	//メルセンヌ・ツイスター
	mt19937_64 engine(seed_gen());
	//乱数範囲（回転角用(最少値、最大値
	uniform_real_distribution<float> rotDist(0.0f, XM_2PI);
	//乱数範囲（座標用
	uniform_real_distribution<float> posDist(-10.0f, 10.0f);

	for (int i = 0; i < 100; i++) {
		//モデル大きさもともとは（1.0f,1.0f,1.0f）なんだと思う
		// x,y,zの順番で横幅、高さ、奥行
		//下のWorldTransdorm_.Initialize();より上に書かないといけない
		WorldTransdorm_[i].scale_ = {1.0f, 1.0f, 1.0f};

		// XM_PIはラジアン表記（3.141592は度数法で180度、それを4で割ったら45度。だからしたのは45度y軸回転させてる
		WorldTransdorm_[i].rotation_ = {rotDist(engine), rotDist(engine), rotDist(engine)};
		//度数法の角度で指定したい場合は下のように書く
		// WorldTransdorm_.rotation_ = {0.0f, XMConvertToRadians(45.0f), 0.0f};

		//モデルがワールドのどこにいるのかの指定
		WorldTransdorm_[i].translation_ = {posDist(engine), posDist(engine), posDist(engine)};

		//モデル周りの初期化
		WorldTransdorm_[i].Initialize();
	}
	//カメラの場所
	ViewProjection_.eye = {0, 0, -10};
	//カメラの見てる場所（ターゲットの場所
	ViewProjection_.target = {10, 0, 0};
	//カメラの上方向ベクトル
	ViewProjection_.up = {cosf(XM_PI / 4.0f), sinf(XM_PI / 4.0f), 0.0f};

	ViewProjection_.Initialize();

}

void GameScene::Update() {
	//カメラ移動速度用の変数の宣言
	XMFLOAT3 move = {0, 0, 0};
	//移動速度の初期化
	const float kEyeSpeed = 0.2f;
	//シフト押してるときはカメラのターゲット移動
	if (input_->PushKey(DIK_LSHIFT)) {
		//キー入力
		if (input_->PushKey(DIK_D)) {
			move = {kEyeSpeed, 0, 0};
		} else if (input_->PushKey(DIK_A)) {
			move = {-kEyeSpeed, 0, 0};
		}

		if (input_->PushKey(DIK_W)) {
			move = {0, kEyeSpeed, 0};
		} else if (input_->PushKey(DIK_S)) {
			move = {0, -kEyeSpeed, 0};
		}

		if (input_->PushKey(DIK_R)) {
			move = {0, 0, kEyeSpeed};
		} else if (input_->PushKey(DIK_F)) {
			move = {0, 0, -kEyeSpeed};
		}
		//移動速度の加算
		ViewProjection_.target.x += move.x;
		ViewProjection_.target.y += move.y;
		ViewProjection_.target.z += move.z;
	} 
	//シフト押してないときはカメラの場所移動
	else {
		//キー入力
		if (input_->PushKey(DIK_D)) {
			move = {kEyeSpeed, 0, 0};
		} else if (input_->PushKey(DIK_A)) {
			move = {-kEyeSpeed, 0, 0};
		}

		if (input_->PushKey(DIK_W)) {
			move = {0, kEyeSpeed, 0};
		} else if (input_->PushKey(DIK_S)) {
			move = {0, -kEyeSpeed, 0};
		}

		if (input_->PushKey(DIK_R)) {
			move = {0, 0, kEyeSpeed};
		} else if (input_->PushKey(DIK_F)) {
			move = {0, 0, -kEyeSpeed};
		}
		//移動速度の加算
		ViewProjection_.eye.x += move.x;
		ViewProjection_.eye.y += move.y;
		ViewProjection_.eye.z += move.z;
	}

	//上方向の回転速度の速さ
	const float kUpRotSpeed = 0.05f;

	if (input_->PushKey(DIK_SPACE)) {
		ViewAngle += kUpRotSpeed;
		//2PIを超えたら0に戻す
		ViewAngle = fmodf(ViewAngle, XM_2PI);
	}

	//上方向ベクトルを計算（半径1の円周上の座標
	ViewProjection_.up = {cosf(ViewAngle), sinf(ViewAngle), 0.0f};

	//更新
	ViewProjection_.UpdateMatrix();

	debugText_->SetPos(50, 50);
	debugText_->Printf(
	  "eye:(%f,%f,%f)\ntarget:(%f,%f,%f)", ViewProjection_.eye.x, ViewProjection_.eye.y, ViewProjection_.eye.z,
		ViewProjection_.target.x, ViewProjection_.target.y, ViewProjection_.target.z);
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
	 
	for (int i = 0; i < 100; i++) {
		model_->Draw(WorldTransdorm_[i], ViewProjection_, TextureHandle_);
	}
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
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
