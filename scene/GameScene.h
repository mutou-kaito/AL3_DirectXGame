#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
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
	
	//textureの宣言
	uint32_t TextureHandle_ = 0;
	//モデルの宣言
	Model* model_ = nullptr;

	//モデルがワールド内のどこにいるかの宣言用
	WorldTransform WorldTransform_[9];
	//カメラの座標
	ViewProjection ViewProjection_;



	public:
	//パーツID
		enum PartId {
			Root,
			Spine,
			Chest,
			Head,
			ArmL,
			ArmR,
			Hip,
			LegL,
			LegR
	  };
	
};
