#pragma once
#include "SDL.h"
const int thickness = 15,window_length = 800,window_width = 600;

struct vector2
{
	float x;
	float y;
};

class Game {
public:
	Game();

	bool Initialize();
	//��ʼ����Ϸ

	void RunLoop();
	//ˢ����Ϸֱ����Ϸ����

	void ShutDown();
	//������Ϸ
private:
	//��Ϸˢ�¹����еİ������� : �������롢ˢ����Ϸ���������
	void ProgressInput();
	void UpdateGame();
	void GenerateOutput();

	//��SDL�����Ĵ���
	SDL_Window *mWindow;

	//��ϷӦ�ü�������
	bool mIsRunnning;

	//������Ⱦ��
	SDL_Renderer *mRenderer;

	//����ʱ��
	Uint32 mTicksCount;

	//����λ��,���λ��,����ٶ�
	vector2 mPaddlePos, mBallPos,mBallVel;
	//����ǽ���Լ��������
	SDL_Rect wall_top, wall_right, wall_left, wall_bottom;
	SDL_Rect ball, paddle;

	//���ĵ��˶�����
	int mPaddleDir;
};