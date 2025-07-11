#include <DxLib.h>
#include "Application.h"
#include "Fader.h"
#include "SceneBase.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "SceneManager.h"

SceneManager* SceneManager::instance_ = nullptr;

SceneManager::SceneManager(void)
{

	sceneId_ = SCENE_ID::NONE;
	waitSceneId_ = SCENE_ID::NONE;

	isSceneChanging_ = false;

}

SceneManager::~SceneManager(void)
{
}

void SceneManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new SceneManager();
	}
	instance_->Init();
}

SceneManager* SceneManager::GetInstance(void)
{
	return instance_;
}

void SceneManager::Init(void)
{

	sceneId_ = SCENE_ID::TITLE;
	waitSceneId_ = SCENE_ID::NONE;

	fader_ = new Fader();
	fader_->Init();

	isSceneChanging_ = false;

	// 初期シーンの設定
	DoChangeScene(SCENE_ID::TITLE);

}

void SceneManager::Update(void)
{

	// フェード更新
	fader_->Update();
	if (isSceneChanging_)
	{
		Fade();
	}
	else
	{
		// 更新
		scene_->Update();
	}

}

void SceneManager::Draw(void)
{

	// 描画先グラフィック領域の指定
	// (３Ｄ描画で使用するカメラの設定などがリセットされる)
	SetDrawScreen(DX_SCREEN_BACK);

	// 画面を初期化
	ClearDrawScreen();

	// 描画
	scene_->Draw();

	// 最後
	fader_->Draw();

}

void SceneManager::Destroy(void)
{
	scene_->Release();
	delete scene_;
	delete fader_;
	delete instance_;
}

void SceneManager::ChangeScene(SCENE_ID nextId)
{

	// フェード処理が終わってからシーンを変える場合もあるため、
	// 遷移先シーンをメンバ変数に保持
	waitSceneId_ = nextId;

	// フェードアウト(暗転)を開始する
	fader_->SetFade(Fader::STATE::FADE_OUT, 0xff0000);
	isSceneChanging_ = true;

}

void SceneManager::DoChangeScene(SCENE_ID sceneId)
{

	// 現在のシーンを解放
	if (scene_ != nullptr)
	{
		scene_->Release();
		delete scene_;
	}

	// シーンを変更する
	sceneId_ = sceneId;

	switch (sceneId_)
	{
	case SCENE_ID::TITLE:
		scene_ = new TitleScene();
		break;
	case SCENE_ID::GAME:
		scene_ = new GameScene();
		break;
	case SCENE_ID::GAMEOVER:
		break;
	}

	// シーンの初期化
	scene_->Init();

	waitSceneId_ = SCENE_ID::NONE;

}

void SceneManager::Fade(void)
{

	Fader::STATE fState = fader_->GetState();
	switch (fState)
	{
	case Fader::STATE::FADE_OUT:
		if (fader_->IsEnd())
		{
			// 暗転後、シーン変更
			DoChangeScene(waitSceneId_);
			fader_->SetFade(Fader::STATE::FADE_IN, 0xff0000);
		}
		break;
	case Fader::STATE::FADE_IN:
		if (fader_->IsEnd())
		{
			// 明転後、シーン遷移終了
			fader_->SetFade(Fader::STATE::NONE, 0xff0000);
			isSceneChanging_ = false;
		}
		break;
	}

}
