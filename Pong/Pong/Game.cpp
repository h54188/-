#include "Game.h"
using namespace std;

Game::Game() {
	mWindow = nullptr;
	mIsRunnning = true;
	mRenderer = nullptr;
	mPaddleDir = 0;

	//墙壁
	wall_top = {
		0,	  //左上方x坐标
		0,	  //左上方y坐标
		window_length,  //长度
		thickness  //宽度
	};
	wall_bottom = { 0,window_width - thickness,window_length,thickness },
	//wall_left = { 0,0,thickness,window_width },
	wall_right = { window_length - thickness,0,thickness,window_width };

	mPaddlePos = { thickness+window_length / window_width,window_width / 2 };
	mBallPos = { window_length / 2,window_width / 2 };
	mBallVel = { -200.0f,235.0f};

	ball = {
		static_cast<int>(mBallPos.x - thickness/2),
		static_cast<int>(mBallPos.y - thickness/2),
		thickness,
		thickness
	};
	paddle = {
		static_cast<int>(mPaddlePos.x - thickness / 2),
		static_cast<int>(mPaddlePos.y - thickness / 2),
		thickness,
		static_cast<int>(thickness * (window_length / window_width + 3.5))
	};
}

void Game::ProgressInput() {
	SDL_Event event;

	//如果队列里仍然有事件
	while (SDL_PollEvent(&event)) {

		//处理每一种事件
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunnning = false;
			break;
		}

	}

	//获取键盘输入数组
	const Uint8* state = SDL_GetKeyboardState(NULL);
	mPaddleDir = 0; //设定球拍方向

	//如果ESC被按下，终止游戏
	if (state[SDL_SCANCODE_ESCAPE]) {
		mIsRunnning = false;
	}
	if (state[SDL_SCANCODE_W]) {
		mPaddleDir = -1;
	} //向上移动
	if (state[SDL_SCANCODE_S]) {
		mPaddleDir = 1;
	} //向下移动
	return;
}

void Game::UpdateGame() {
	//确保两帧之间至少有16ms的间隔
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));
	//计算增量时间(转换为秒)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	//更新流逝时间
	mTicksCount = SDL_GetTicks();

	//钳制最大时间增量
	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}//以保证游戏世界以增量时间刷新

	if (mPaddleDir != 0) {
		mPaddlePos.y += mPaddleDir * 450.0f *deltaTime;
		//以下代码确保球拍不会离开屏幕
		if (mPaddlePos.y < (paddle.h / 2.0f)) {
			mPaddlePos.y = paddle.h / 2.0f;
		}
		else if (mPaddlePos.y > (window_width * 1.0 - paddle.h / 2.0f-2*thickness)) {
			mPaddlePos.y = window_width * 1.0 - paddle.h / 2.0f-2*thickness;
		}
		//更新拍的位置
		paddle.y = static_cast<int>(mPaddlePos.y - thickness / 2);
	}

	float diff = paddle.y + paddle.h/2.0 - ball.y > 0 ? paddle.y + paddle.h / 2.0 - ball.y : ball.y - paddle.h / 2.0 - paddle.y;
	//检测球的碰撞
	if (mBallPos.y <= thickness && mBallVel.y < 0.0f) {
		mBallVel.y *= -1;
	}//顶部的墙
	if (mBallPos.y >= window_width - thickness && mBallVel.y > 0.0f) {
		mBallVel.y *= -1;
	}//底部的墙
	if (mBallPos.x >= window_length - thickness && mBallVel.x > 0.0f) {
		mBallVel.x *= -1;
	}
	if (diff <= paddle.h / 1.3f&&mBallPos.x <= 2.0*thickness && mBallPos.x >= 1.0*thickness && mBallVel.x < 0.0f) {
		mBallVel.x *= -1;
	}

	//更新球的位置
	mBallPos.x += mBallVel.x *deltaTime;
	mBallPos.y += mBallVel.y *deltaTime;
	ball.x = static_cast<int>(mBallPos.x - thickness / 2);
	ball.y = static_cast<int>(mBallPos.y - thickness / 2);

	return;
}

void Game::GenerateOutput() {

	SDL_SetRenderDrawColor(mRenderer, //接收的渲染器
		0,     //红色值
		0,     //绿色值
		0,   //蓝色值
		255 ); //不透明度
	SDL_RenderClear(mRenderer); //清除缓冲区为上方指定颜色
	
	//绘制墙壁
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

	SDL_RenderFillRect(mRenderer, &wall_top);
	SDL_RenderFillRect(mRenderer, &wall_bottom);
//	SDL_RenderFillRect(mRenderer, &wall_left);
	SDL_RenderFillRect(mRenderer, &wall_right);

	//绘制球和球拍
	SDL_RenderFillRect(mRenderer, &paddle);
	SDL_RenderFillRect(mRenderer, &ball);

	SDL_RenderPresent(mRenderer); //交换前缓冲区和后缓冲区

	return;
}

bool Game::Initialize() {

	//对视频子系统进行初始化
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);

	//如果失败
	if (sdlResult != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	//创建一个窗口
	mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 1)",  //标题
		100,														   //窗口左上方x坐标
		100,														   //窗口左上方y坐标
		window_length,														   //窗口宽度
		window_width,														   //窗口高度
		0 );															   //标记
	
	//创建失败
	if (!mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	//创建渲染器
	mRenderer = SDL_CreateRenderer(mWindow, //要渲染的对象
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	//渲染器创建失败
	if (!mRenderer) {
		SDL_Log("Failed to create renderer: %s",SDL_GetError());
		return false;
	}

	return true;
}

void Game::ShutDown() {

	SDL_DestroyWindow(mWindow); //关闭窗口
	SDL_DestroyRenderer(mRenderer);//关闭渲染器
	SDL_Quit(); //退出

	return;
}

void Game::RunLoop() {

	Game::GenerateOutput();

	while (mIsRunnning) {

		Game::ProgressInput();
		Game::UpdateGame();
		Game::GenerateOutput();

	}

	return;
}

