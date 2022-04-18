#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

using namespace DirectX;

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

	x = 10.0f;
	y = 10.0f;

	//モデル大きさもともとは（1.0f,1.0f,1.0f）なんだと思う
	//x,y,zの順番で横幅、高さ、奥行
	//下のWorldTransdorm_.Initialize();より上に書かないといけない
	WorldTransdorm_.scale_ = {5.0f, 5.0f, 5.0f};

	//XM_PIはラジアン表記（3.141592は度数法で180度、それを4で割ったら45度。だからしたのは45度y軸回転させてる
	WorldTransdorm_.rotation_ = {XM_PI / 4.0f, XM_PI / 4.0f, 0.0f};
	//度数法の角度で指定したい場合は下のように書く
	//WorldTransdorm_.rotation_ = {0.0f, XMConvertToRadians(45.0f), 0.0f};

	//モデルがワールドのどこにいるのかの指定
	WorldTransdorm_.translation_ = {x, y, 10.0f};

	//モデル周りの初期化
	WorldTransdorm_.Initialize();
	ViewProjection_.Initialize();
	
}

void GameScene::Update() {
	//WASDキー入力で動くようにしたい
	//これでできると思ったけど無理だった多分初期化用のWorldTransdorm_.translation_、とは別に
	//更新用の関数がある。01_02の課題で2D画像を動かす時もそうだったから
	/*if (input_->PushKey(DIK_D)) {
		x++;
	} else if (input_->PushKey(DIK_A)) {
		x--;
	}

	if (input_->PushKey(DIK_W)) {
		y++;
	} else if (input_->PushKey(DIK_S)) {
		y--;
	}

	WorldTransdorm_.translation_ = {x, y, 10.0f};*/
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
	 
	model_->Draw(WorldTransdorm_, ViewProjection_, TextureHandle_);

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
