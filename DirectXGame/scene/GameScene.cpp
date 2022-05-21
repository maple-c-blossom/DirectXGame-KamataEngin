#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "Vector3.h"
#include "Vector4.h"
GameScene::GameScene() {}

GameScene::~GameScene() 
{
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize(int windowWidth,int windowHeight) {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	textureHandle_ = TextureManager::Load("mario.jpg");
	model_ = Model::Create();
	worldTrasform_.Initialize();
	viewProjection_.Initialize();
	debugCamera_ = new DebugCamera(windowWidth, windowHeight);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

}

void GameScene::Update() 
{
	debugCamera_->Update();
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
	for (int i = 0; i < MAX_LINE_NUM; i++)
	{
			Vector3 startPos = { i * 0.5f , 0, 0};
			Vector3 endPos = {startPos.x,startPos.y,10.5};
			Vector4 RGBA = {0,0,0.5f,1};
			PrimitiveDrawer::GetInstance()->DrawLine3d(startPos,endPos,RGBA);
	}

	for (int i = 0; i < MAX_LINE_NUM; i++)
	{
		Vector3 startPos = { 0 , 0, i * 0.5f };
		Vector3 endPos = { 10.5 ,startPos.y,startPos.z };
		Vector4 RGBA = { 0.5f,0,0,1 };
		PrimitiveDrawer::GetInstance()->DrawLine3d(startPos, endPos, RGBA);
	}
	//model_->Draw(worldTrasform_, debugCamera_->GetViewProjection(), textureHandle_);
	/// </summary>

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
