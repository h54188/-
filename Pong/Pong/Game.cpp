#include "Game.h"
using namespace std;

Game::Game() {
	mWindow = nullptr;
	mIsRunnning = true;
	mRenderer = nullptr;
	mPaddleDir = 0;

	//ǽ��
	wall_top = {
		0,	  //���Ϸ�x����
		0,	  //���Ϸ�y����
		window_length,  //����
		thickness  //���
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

	//�����������Ȼ���¼�
	while (SDL_PollEvent(&event)) {

		//����ÿһ���¼�
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunnning = false;
			break;
		}

	}

	//��ȡ������������
	const Uint8* state = SDL_GetKeyboardState(NULL);
	mPaddleDir = 0; //�趨���ķ���

	//���ESC�����£���ֹ��Ϸ
	if (state[SDL_SCANCODE_ESCAPE]) {
		mIsRunnning = false;
	}
	if (state[SDL_SCANCODE_W]) {
		mPaddleDir = -1;
	} //�����ƶ�
	if (state[SDL_SCANCODE_S]) {
		mPaddleDir = 1;
	} //�����ƶ�
	return;
}

void Game::UpdateGame() {
	//ȷ����֮֡��������16ms�ļ��
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));
	//��������ʱ��(ת��Ϊ��)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	//��������ʱ��
	mTicksCount = SDL_GetTicks();

	//ǯ�����ʱ������
	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}//�Ա�֤��Ϸ����������ʱ��ˢ��

	if (mPaddleDir != 0) {
		mPaddlePos.y += mPaddleDir * 450.0f *deltaTime;
		//���´���ȷ�����Ĳ����뿪��Ļ
		if (mPaddlePos.y < (paddle.h / 2.0f)) {
			mPaddlePos.y = paddle.h / 2.0f;
		}
		else if (mPaddlePos.y > (window_width * 1.0 - paddle.h / 2.0f-2*thickness)) {
			mPaddlePos.y = window_width * 1.0 - paddle.h / 2.0f-2*thickness;
		}
		//�����ĵ�λ��
		paddle.y = static_cast<int>(mPaddlePos.y - thickness / 2);
	}

	float diff = paddle.y + paddle.h/2.0 - ball.y > 0 ? paddle.y + paddle.h / 2.0 - ball.y : ball.y - paddle.h / 2.0 - paddle.y;
	//��������ײ
	if (mBallPos.y <= thickness && mBallVel.y < 0.0f) {
		mBallVel.y *= -1;
	}//������ǽ
	if (mBallPos.y >= window_width - thickness && mBallVel.y > 0.0f) {
		mBallVel.y *= -1;
	}//�ײ���ǽ
	if (mBallPos.x >= window_length - thickness && mBallVel.x > 0.0f) {
		mBallVel.x *= -1;
	}
	if (diff <= paddle.h / 1.3f&&mBallPos.x <= 2.0*thickness && mBallPos.x >= 1.0*thickness && mBallVel.x < 0.0f) {
		mBallVel.x *= -1;
	}

	//�������λ��
	mBallPos.x += mBallVel.x *deltaTime;
	mBallPos.y += mBallVel.y *deltaTime;
	ball.x = static_cast<int>(mBallPos.x - thickness / 2);
	ball.y = static_cast<int>(mBallPos.y - thickness / 2);

	return;
}

void Game::GenerateOutput() {

	SDL_SetRenderDrawColor(mRenderer, //���յ���Ⱦ��
		0,     //��ɫֵ
		0,     //��ɫֵ
		0,   //��ɫֵ
		255 ); //��͸����
	SDL_RenderClear(mRenderer); //���������Ϊ�Ϸ�ָ����ɫ
	
	//����ǽ��
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

	SDL_RenderFillRect(mRenderer, &wall_top);
	SDL_RenderFillRect(mRenderer, &wall_bottom);
//	SDL_RenderFillRect(mRenderer, &wall_left);
	SDL_RenderFillRect(mRenderer, &wall_right);

	//�����������
	SDL_RenderFillRect(mRenderer, &paddle);
	SDL_RenderFillRect(mRenderer, &ball);

	SDL_RenderPresent(mRenderer); //����ǰ�������ͺ󻺳���

	return;
}

bool Game::Initialize() {

	//����Ƶ��ϵͳ���г�ʼ��
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);

	//���ʧ��
	if (sdlResult != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	//����һ������
	mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 1)",  //����
		100,														   //�������Ϸ�x����
		100,														   //�������Ϸ�y����
		window_length,														   //���ڿ��
		window_width,														   //���ڸ߶�
		0 );															   //���
	
	//����ʧ��
	if (!mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	//������Ⱦ��
	mRenderer = SDL_CreateRenderer(mWindow, //Ҫ��Ⱦ�Ķ���
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	//��Ⱦ������ʧ��
	if (!mRenderer) {
		SDL_Log("Failed to create renderer: %s",SDL_GetError());
		return false;
	}

	return true;
}

void Game::ShutDown() {

	SDL_DestroyWindow(mWindow); //�رմ���
	SDL_DestroyRenderer(mRenderer);//�ر���Ⱦ��
	SDL_Quit(); //�˳�

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

