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
	
	//僕のイメージ的にモデルの中心、原点は腰の骨盤と背骨の接合点？あたりか、みぞおちあたり
	// 今回は原点を腰にする
	//親　原点
	WorldTransform_[0].Initialize();
	//子　背骨
	WorldTransform_[1].translation_ = {0, 3.0f, 0};
	WorldTransform_[1].parent_ = &WorldTransform_[0];
	WorldTransform_[1].Initialize();

	//背骨の子
	//胸
	WorldTransform_[2].parent_ = &WorldTransform_[1];
	WorldTransform_[2].Initialize();
	//胸の子
	//頭
	WorldTransform_[3].translation_ = {0, 3.0f, 0};
	WorldTransform_[3].parent_ = &WorldTransform_[2];
	WorldTransform_[3].Initialize();
	//左手
	WorldTransform_[4].translation_ = {3.0f, 0, 0};
	WorldTransform_[4].parent_ = &WorldTransform_[2];
	WorldTransform_[4].Initialize();
	//右手
	WorldTransform_[5].translation_ = {-3.0f, 0, 0};
	WorldTransform_[5].parent_ = &WorldTransform_[2];
	WorldTransform_[5].Initialize();

	//原点の子
	//お尻
	WorldTransform_[6].translation_ = {0, 0, 0};
	WorldTransform_[6].parent_ = &WorldTransform_[0];
	WorldTransform_[6].Initialize();
	//お尻の子
	//左足
	WorldTransform_[7].translation_ = {3.0f, -3.0f, 0};
	WorldTransform_[7].parent_ = &WorldTransform_[6];
	WorldTransform_[7].Initialize();
	//右足
	WorldTransform_[8].translation_ = {-3.0f, -3.0f, 0};
	WorldTransform_[8].parent_ = &WorldTransform_[6];
	WorldTransform_[8].Initialize();



	ViewProjection_.Initialize();

}

void GameScene::Update() { 
	//キャラクターの移動用ベクトル
	XMFLOAT3 move = {0, 0, 0};

	//キャラクターの移動速度
	const float kCaracterSpeed = 0.2f;

	//押した方向で移動ベクトル
	if (input_->PushKey(DIK_LEFT)) {
		move = {-kCaracterSpeed, 0, 0};
	} else if (input_->PushKey(DIK_RIGHT)) {
		move = {kCaracterSpeed, 0, 0};
	}

	//ワールドトランスフォームの移動
	WorldTransform_[PartId::Root].translation_.x += move.x;
	WorldTransform_[PartId::Root].translation_.y += move.y;
	WorldTransform_[PartId::Root].translation_.z += move.z;

	//回転
	const float kRotSpeed = 0.05f;
	//上半身
	if (input_->PushKey(DIK_U)) {
	WorldTransform_[PartId::Chest].rotation_.y -= kRotSpeed;
	
	//Rootのほうを回転させたら斜め移動するようになると思うから試してみる
	//ならんかった、つまんね
	//WorldTransform_[PartId::Root].rotation_.y -= kRotSpeed;
	}else if (input_->PushKey(DIK_I)) {	
	WorldTransform_[PartId::Chest].rotation_.y += kRotSpeed;
	
	// Rootのほうを回転させたら斜め移動するようになると思うから試してみる
	//WorldTransform_[PartId::Root].rotation_.y += kRotSpeed;
	}
	//下半身
	if (input_->PushKey(DIK_J)) {	
	WorldTransform_[PartId::Hip].rotation_.y -= kRotSpeed;
	}else if (input_->PushKey(DIK_K)) {	
	WorldTransform_[PartId::Hip].rotation_.y += kRotSpeed;
	}


	//デバッグ用の表示
	debugText_->SetPos(50, 150);
	debugText_->Printf(
	  "root:(%f,%f,%f)", 
		WorldTransform_[PartId::Root].translation_.x,
		WorldTransform_[PartId::Root].translation_.y, 
		WorldTransform_[PartId::Root].translation_.z);



	//ワールドトランスフォームの更新
	for (int i = 0; i < 9; i++) {
		WorldTransform_[i].UpdateMatrix();
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
	 
	for (int i = 2; i < 9; i++) {
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
