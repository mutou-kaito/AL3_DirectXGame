#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

using namespace DirectX;
using namespace std;

GameScene::GameScene() {}

GameScene::~GameScene() {
	// Spriteクラスのsprite_のデストラクタの呼び出し
	delete sprite_;
	delete model_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//初期化追加部分
	//画像の情報を代入？
	Graph_ = TextureManager::Load("AL3_01_02\\mario.jpg");
	//上でGraphに代入した情報を描画するための画像データとして保存？
	sprite_ = Sprite::Create(Graph_, {100, 50});

	// 3dモデルの生成
	model_ = Model::Create();
	//座標の代入
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	//サウンドデータの代入
	soundDataHandle_ = audio_->LoadWave("AL3_01_02\\se_sad03.wav");
	audio_->PlayWave(soundDataHandle_, true);
}

void GameScene::Update() {
	// XMFLOAT2の2ってことは3もあるはず
	XMFLOAT2 position = sprite_->GetPosition();
	position.x += 2;
	position.y += 1;
	sprite_->SetPosition(position);

	//	キー入力でBGM停止
	if (input_->TriggerKey(DIK_SPACE)) {
		audio_->StopWave(soundDataHandle_);
	}
	if (input_->TriggerKey(DIK_RETURN)) {
		audio_->PlayWave(soundDataHandle_);
	}

	// Value
	Value_++;
	//値を含んだ文字列
	string strDebug = string("Value:") + to_string(Value_);
	// debugテキストの表示
	debugText_->Print(strDebug, 50, 50, 1.0f);

	string Graph_X = string("Graph_X:") + to_string(position.x);
	string Graph_Y = string("Graph_Y:") + to_string(position.y);
	debugText_->Print(Graph_X, 50, 100, 1.0f);
	debugText_->Print(Graph_Y, 50, 150, 1.0f);
	//テキスト
	string Menu = "SPACE = BGM STOP : ENTER = SINGLE PLAY";
	debugText_->Print(Menu, 50, 200, 1.0f);
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

	model_->Draw(worldTransform_, viewProjection_, Graph_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	sprite_->Draw();

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
