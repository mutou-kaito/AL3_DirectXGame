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
	

	for (int i = 0; i < 10; i++) {
		WorldTransform_[i].scale_ = {1.0f, 1.0f, 1.0f};
		WorldTransform_[i + 10].scale_ = {1.0f, 1.0f, 1.0f};

		//モデルがワールドのどこにいるのかの指定
		float num = -5 + i*2;

		WorldTransform_[i].translation_ = {num, -4.0f, 0.0f};
		WorldTransform_[i + 10].translation_ = {num, 4.0f, 0.0f};

		//モデル周りの初期化
		WorldTransform_[i].Initialize();
		WorldTransform_[i + 10].Initialize();
	}
	//カメラの場所
	ViewProjection_.eye = {0, 0, 10};
	//カメラの見てる場所（ターゲットの場所
	ViewProjection_.target = {0, 0, 0};
	//カメラの上方向ベクトル
	//ViewProjection_.up = {1.0f, 1.0f, 1.0f};

	ViewProjection_.Initialize();

}

void GameScene::Update() {
	//カメラ移動速度用の変数の宣言
	XMFLOAT3 move = {0, 0, 0};
	//移動速度の初期化
	const float kEyeSpeed = 0.2f;
	
	//↓↓課題部分
	//視点移動
	if (input_->PushKey(DIK_W)) {
		move = {0, kEyeSpeed, 0};
	} else if (input_->PushKey(DIK_S)) {
		move = {0, -kEyeSpeed, 0};
	}

	//注点移動
	if (input_->PushKey(DIK_RIGHT)) {
		move = {kEyeSpeed, 0, 0};
	} else if (input_->PushKey(DIK_LEFT)) {
		move = {-kEyeSpeed, 0, 0};
	}

	//代入
	ViewProjection_.target.x += move.x;
	ViewProjection_.target.y += move.y;
	ViewProjection_.target.z += move.z;

	ViewProjection_.eye.x += move.x;
	ViewProjection_.eye.y += move.y;
	ViewProjection_.eye.z += move.z;

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
	 
	for (int i = 0; i < 20; i++) {
		model_->Draw(WorldTransform_[i], ViewProjection_, TextureHandle_);
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
