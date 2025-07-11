#include <DxLib.h>
#include "InputManager.h"
#include "SceneManager.h"
#include "Application.h"
#include "FrameRate.h"

Application* Application::instance_ = nullptr;

Application::Application(void)
{
	frameRate_ = new FrameRate;
	isError_ = false;
}

Application::Application(const Application& ins)
{
}

Application::~Application()
{
}

void Application::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Application();
	}
	instance_->Init();
}

Application* Application::GetInstance(void)
{
	return instance_;
}

void Application::Init(void)
{

	// アプリケーションの初期設定
	SetWindowText("AsouRockman");

	// ウィンドウサイズ
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	ChangeWindowMode(true);

	// 垂直同期 無効化
	SetWaitVSyncFlag(0);

	if (DxLib_Init() == -1)
	{
		isError_ = true;
		return;
	}

	// 入力制御初期化
	SetUseDirectInputFlag(true);
	InputManager::CreateInstance();
	InputManager::GetInstance()->Init();

	// シーン管理初期化
	SceneManager::CreateInstance();

}

void Application::Run(void)
{

	// ゲームループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		frameRate_->Update(); // フレームレート更新

		if (frameRate_->GetLimitFrameRate() == false)
		{
			frameRate_->SetFrameRate();

			InputManager::GetInstance()->Update();
			SceneManager::GetInstance()->Update();
			SceneManager::GetInstance()->Draw();

			frameRate_->Draw();
			ScreenFlip();
		}
	}
}

void Application::Destroy(void)
{
	// フレームレート破棄
	delete frameRate_;

	// キー制御破棄
	InputManager::GetInstance()->DeleteInstance();

	// シーン管理破棄
	SceneManager::GetInstance()->Destroy();

	if (DxLib_End() == -1)
	{
		isError_ = true;
		return;
	}

	delete instance_;

}

bool Application::IsError(void)
{
	return isError_;
}
