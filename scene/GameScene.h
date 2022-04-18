#pragma once

#include "Audio.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <DirectXMath.h>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	//画像を管理するためのん変数の宣言
	//アンファインドint型の略　32は32bit = 4byte
	//アンダーバーはメンバ変数ってこと
	uint32_t Graph_ = 0;
	// Spriteクラスのsprite_を宣言、nullptrはパターン無しだろうけどなんのパターン無しなのかよくわからん
	Sprite* sprite_ = nullptr;
	//上の=0 とか　nullptrは無くてもいいけどあったほうがいいらしい

	// 3dモデル
	Model* model_ = nullptr;
	//モデルの座標
	WorldTransform worldTransform_;
	//多分モデルを映すカメラの場所？
	ViewProjection viewProjection_;

	//サウンド
	uint32_t soundDataHandle_ = 0;

	//プログラム開始から何ループ経過したかを表示する
	uint32_t Value_ = 0;
};