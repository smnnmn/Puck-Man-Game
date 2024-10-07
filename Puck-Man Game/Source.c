#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <stdbool.h>

#define WIDTH 21
#define HEIGHT 21

#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80

#define BUFFERSIZE 10000

bool isInsMonster1 = false; // 몬스터1 추가생성 확인
bool isInsMonster2 = false; // 몬스터2 추가생성 확인
bool isInsMonster3 = false; // 몬스터3 추가생성 확인

bool isInsPower = false;// 파워업  생성 확인

bool isSpeedUp1 = false;// 속도 1증가 확인
bool isSpeedUp2 = false;// 속도 2증가 확인
bool isSpeedUp3 = false;// 속도 3증가 확인

bool isInsTelpo1 = false; // 통로1 생성
bool isInsTelpo2 = false; // 통로2 생성

bool isChangeExit = false;// 출구생성 확인

bool isGameOver = false; // 게임 오버 확인
bool isGameClear = false; // 게임 클리어 확인

bool isPowerUp = false; // 파워업 상태 확인

int randomDir = 0;

int exitX = 0;
int exitY = 0;

int enemysDir[4] = { 0,0,0,0 }; // 적의 이동 방향을 저장
bool enemysFind[4] = { false,false,false,false }; // 적의 플레이어 찾음의 유무

int powerTime = 0;



typedef struct Character
{
	int x;
	int y;
	const char* shape;
}Character;

// 플레이어 캐릭터 생성
Character character = { 20,15,"●" };

// 적 캐릭터 생성
Character enemy1 = { 20,8,"△" };
Character enemy2 = { 18,11,"▽" };
Character enemy3 = { 20,11,"◁" };
Character enemy4 = { 22,11,"▷" };

Character* enemys[] = { &enemy1,&enemy2, &enemy3, &enemy4 };

int coins = 0; // 플레이어가 먹은 코인의 개수
int nextcoins = 0; // 플레이어의 다음 코인 목표

char nextAction[]; // 플레이어의 다음 생기는 상황

int gameSpeed = 200; // 게임 플레이 속도
int tempGameSpeed = 0; // 게임 플레이 속도 저장용

const char* nextString = ""; // 다음 진행 상황
const char* beforeString = ""; // 이전 목표 상황 

void textcolor(int colorNum) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum);
}

void Position(int x, int y)
{
	// x와 y축을 설정하는 구조체입니다.
	COORD position = { x, y };

	// 콘솔 커서의 좌표를 설정하는 함수입니다.
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}

char maze[HEIGHT][WIDTH] =
{
	{'1','1','1','1','1','1','1','1','1','1','7','1','1','1','1','1','1','1','1','1','1'},
	{'1','2','2','1','2','2','2','2','2','2','2','2','2','2','2','2','2','1','2','2','1'},
	{'1','1','2','2','2','1','1','1','2','1','2','1','2','1','1','1','2','2','2','1','1'},
	{'1','1','2','1','2','2','2','1','2','1','1','1','2','1','2','2','2','1','2','1','1'},
	{'1','2','2','1','2','1','3','1','2','2','2','2','2','1','3','1','2','1','2','2','1'},
	{'1','2','1','1','2','1','1','1','2','1','2','1','2','1','1','1','2','1','1','2','1'},
	{'1','2','2','2','2','2','2','2','2','2','2','2','2','2','2','2','2','2','2','2','1'},
	{'1','2','1','2','1','1','1','2','1','1','1','1','1','2','1','1','1','2','1','2','1'},
	{'1','2','1','2','2','2','2','0','0','0','0','0','0','0','2','2','2','2','1','2','1'},
	{'1','2','1','2','1','1','1','0','1','1','5','1','1','0','1','1','1','2','1','2','1'},
	{'6','2','1','2','1','2','2','0','1','0','0','0','1','0','2','2','1','2','1','2','6'},
	{'1','2','1','2','1','2','1','0','1','0','0','0','1','0','1','2','1','2','1','2','1'},
	{'1','2','2','2','2','2','1','0','1','1','1','1','1','0','1','2','2','2','2','2','1'},
	{'1','2','1','2','1','2','1','0','0','0','1','0','0','0','1','2','1','2','1','2','1'},
	{'1','2','1','3','1','2','2','2','1','2','1','2','1','2','2','2','1','3','1','2','1'},
	{'1','2','1','1','1','2','1','1','1','2','0','2','1','1','1','2','1','1','1','2','1'},
	{'1','2','2','2','2','2','2','1','1','1','2','1','1','1','2','2','2','2','2','2','1'},
	{'1','2','1','2','1','1','2','2','2','2','2','2','2','2','2','1','1','2','1','2','1'},
	{'1','2','1','2','2','2','2','1','1','1','2','1','1','1','2','2','2','2','1','2','1'},
	{'1','2','2','2','1','1','2','2','3','1','2','1','3','2','2','1','1','2','2','2','1'},
	{'1','1','1','1','1','1','1','1','1','1','7','1','1','1','1','1','1','1','1','1','1'},
};
// 맵 생성 관련 코드
void Render()
{
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			textcolor(7);
			switch (maze[i][j])
			{
				// 아무것도 없음
			case '0': printf("  "); break;
				// 아무도 나가지 못하는 벽
			case '1':if (isPowerUp) { textcolor(4); }
					else if (isChangeExit)
			{
				textcolor(4);
			}
					else { textcolor(1); } printf("■"); break;
				// 플레이어가 먹어야할 코인
			case '2':			textcolor(6);
				printf("ㆍ");  break;
				// 플레이어가 먹으면 강화되는 아이템
			case '3': if (!isInsPower) { printf("  "); }
					else {
				textcolor(11);
				printf("⊙"); } break;
				// 생성되는 출구
			case'4': textcolor(14); printf("★"); break;
				// 적 기지의 입구
			case '5': printf("□"); break;
				// 닫힌 통로1 &  열린 통로2
			case '6':  textcolor(1); printf("◆");
				break;
				// 닫힌 통로2 & 열린 통로2
			case '7': textcolor(1); printf("◆");
				break;
			case '8': 			textcolor(2);
				printf("♤"); break;
			case '9':			textcolor(4);
				printf("♡"); break;
			}
		}
		printf("\n");
	}
}


// 몬스터 추가 생성 코드
void InsMonster(int enemyNum)
{
	enemys[enemyNum]->x = 20;
	enemys[enemyNum]->y = 8;
	isInsMonster1 = true;

}
// 파워업 상태 돌입
void PowerTime()
{
	// 30 딜레이?
	powerTime++;
}
// 몬스터 스피드 증가
void SpeedUP(int speed)
{
	gameSpeed = speed;
	tempGameSpeed = speed;
}
// 텔레포트 통로 생성
void InsTelpo(int potalNum)
{
	switch (potalNum)
	{
	case 0:
		for (int i = 0; i < HEIGHT; i++)
		{
			for (int j = 0; j < WIDTH; j++)
			{

				switch (maze[i][j])
				{
				case '6': maze[i][j] = '8';
				}
			}

		}
		break;
	case 1:for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{

			switch (maze[i][j])
			{
			case '7': maze[i][j] = '9';
			}
		}

	}
	}

}
// 출구 생성
void  ChangeExit()
{
	// 어떤 표시의 통로가 출구가 될 것인가
	int randomExit = rand() % 2; // 0이나 1이 나옴 (1/2확률)

	// 어떤 위치에 출구가 만들어 질 것인가
	int randomPosition = rand() % 2; // 0이나 1이 나옴
	switch (randomExit)
	{
	case 0: for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{

			switch (maze[i][j])
			{
			case '8': if (randomPosition == 0) { maze[i][j] = '4'; exitY = i; exitX = j;  return; }
					else if (randomPosition != 0) { randomPosition--; }
					break;
			}
		}
	}
	case 1: for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{

			switch (maze[i][j])
			{
			case '9': if (randomPosition == 0) { maze[i][j] = '4'; exitY = i; exitX = j; return; }
					else if (randomPosition != 0) { randomPosition--; }
					break;
			}
		}
	}
	}

	return 0;
}
// UI를 관리하는 함수
void UIManager()
{
	textcolor(7);
	// 목표 설정 & 미션 결과
	printf("코인 미션:%d / %d --> %s  \n", coins, nextcoins, nextString);
	printf("\n");
	// 파워업 상태창
	if (isPowerUp)
	{
		printf("파워 업!  무적 & 속도증가\t남은시간: %d\n", 20 - powerTime);
		printf("\n");
	}

	// 마지막 설정
	if (coins >= 25)
	{
		printf("이전 변경:%s\n", beforeString);
	}


}

void main()
{

	char key = 0;
	// 방향 (시계방향으로 12시부터 1,2,3,4)
	int dir = 0;

	MoveWindow(GetConsoleWindow(), 750, 200, 410, 600, true); // x, y, 너비, 높이순

	FILE* fileOver = fopen("GameOver.txt", "r");
	char bufferOver[BUFFERSIZE] = { 0, };
	
	fread(bufferOver, 1, BUFFERSIZE, fileOver);


	

	Render();
	UIManager();

	while (!isGameOver && !isGameClear)
	{

		// 플레이어 움직이기 코드
		{
			if (_kbhit())
			{
				key = _getch();

				if (key == -32)
				{
					key = _getch();
				}
				// 키 입력 받기
				switch (key)
				{
				case UP: if (maze[character.y - 1][character.x / 2] != '1') dir = 1;
					break;
				case LEFT:if (maze[character.y][character.x / 2 - 1] != '1') dir = 4;
					break;
				case RIGHT:if (maze[character.y][character.x / 2 + 1] != '1') dir = 2;
					break;
				case DOWN: if (maze[character.y + 1][character.x / 2] != '1')dir = 3;
					break;
				}


			}
			// 움직이기
			switch (dir)
			{
			case 1: if (maze[character.y - 1][character.x / 2] != '1' && maze[character.y - 1][character.x / 2] != '7')  character.y--; break;
			case 2: if (maze[character.y][character.x / 2 + 1] != '1' && maze[character.y][character.x / 2 + 1] != '6') character.x += 2; break;
			case 3: if (maze[character.y + 1][character.x / 2] != '1' && maze[character.y + 1][character.x / 2] != '7') character.y++; break;
			case 4: if (maze[character.y][character.x / 2 - 1] != '1' && maze[character.y][character.x / 2 - 1] != '6') character.x -= 2; break;
			}
		}

		// 적 움직이기 코드
		{
			srand(time(NULL));
			for (int i = 0; i < 4; i++)
			{
				// 벽에 닿아야만 방향전환을 하는 코드
				//	{
				//		// 벽에 도달할 때 까지 방향전환 X
				//		if (enemysDir[i] == 0)
				//		{
				//			randomDir = rand() % 4 + 1;
				//			enemysDir[i] = randomDir;
				//		}
				//	
				//		// 랜덤으로 움직이기
				//		switch (enemysDir[i])
				//		{
				//		case 1: if (maze[enemys[i]->y - 1][enemys[i]->x / 2] != '1' && maze[enemys[i]->y - 1][enemys[i]->x / 2] != '5' && maze[enemys[i]->y - 1][enemys[i]->x / 2] != '7'
				//			&& maze[enemys[i]->y - 1][enemys[i]->x / 2] != '9') enemys[i]->y--;
				//			  else { enemysDir[i] = 0; i--;  continue; }
				//			  break;
				//		case 2:if (maze[enemys[i]->y][enemys[i]->x / 2 + 1] != '1' && maze[enemys[i]->y][enemys[i]->x / 2 + 1] != '5'
				//			&& maze[enemys[i]->y][enemys[i]->x / 2 + 1] != '6' && maze[enemys[i]->y][enemys[i]->x / 2 + 1] != '8') enemys[i]->x += 2;
				//			  else { enemysDir[i] = 0; i--;   continue; }
				//			  break;
				//		case 3:if (maze[enemys[i]->y + 1][enemys[i]->x / 2] != '1' && maze[enemys[i]->y + 1][enemys[i]->x / 2] != '5'
				//			&& maze[enemys[i]->y][enemys[i]->x / 2 + 1] != '6' && maze[enemys[i]->y][enemys[i]->x / 2 + 1] != '8') enemys[i]->y++;
				//			  else { enemysDir[i] = 0; i--;  continue; }
				//			  break;
				//		case 4: if (maze[enemys[i]->y][enemys[i]->x / 2 - 1] != '1' && maze[enemys[i]->y][enemys[i]->x / 2 - 1] != '5'
				//			&& maze[enemys[i]->y][enemys[i]->x / 2 + 1] != '6' && maze[enemys[i]->y][enemys[i]->x / 2 + 1] != '8') enemys[i]->x -= 2;
				//			  else { enemysDir[i] = 0; i--;    continue; }
				//			  break;
				//		}
				//	}
				{
						randomDir = rand() % 4 + 1;
					// 랜덤으로 움직이기
					switch (randomDir)
					{
					case 1: if (maze[enemys[i]->y - 1][enemys[i]->x / 2] != '1' && maze[enemys[i]->y - 1][enemys[i]->x / 2] != '5' && maze[enemys[i]->y - 1][enemys[i]->x / 2] != '7'
						&& maze[enemys[i]->y - 1][enemys[i]->x / 2] != '9' && maze[enemys[i]->y - 1][enemys[i]->x / 2] != '4') enemys[i]->y--;
						  else {  i--;  continue; }
						  break;
					case 2:if (maze[enemys[i]->y][enemys[i]->x / 2 + 1] != '1' && maze[enemys[i]->y][enemys[i]->x / 2 + 1] != '5'
						&& maze[enemys[i]->y][enemys[i]->x / 2 + 1] != '6' && maze[enemys[i]->y][enemys[i]->x / 2 + 1] != '8' && maze[enemys[i]->y][enemys[i]->x / 2 + 1] != '4') enemys[i]->x += 2;
						  else {  i--;   continue; }
						  break;
					case 3:if (maze[enemys[i]->y + 1][enemys[i]->x / 2] != '1' && maze[enemys[i]->y + 1][enemys[i]->x / 2] != '5'
						&& maze[enemys[i]->y + 1][enemys[i]->x / 2 ] != '7' && maze[enemys[i]->y + 1][enemys[i]->x / 2 ] != '9' && maze[enemys[i]->y + 1][enemys[i]->x / 2] != '4') enemys[i]->y++;
						  else {  i--;  continue; }
						  break;
					case 4: if (maze[enemys[i]->y][enemys[i]->x / 2 - 1] != '1' && maze[enemys[i]->y][enemys[i]->x / 2 - 1] != '5'
						&& maze[enemys[i]->y][enemys[i]->x / 2 - 1] != '6' && maze[enemys[i]->y][enemys[i]->x / 2 - 1] != '8' && maze[enemys[i]->y][enemys[i]->x / 2 - 1] != '4') enemys[i]->x -= 2;
						  else { i--;    continue; }
						  break;
					}
				}

			}

		}
		// 플레이어의 충돌 코드
		{
			// 코인을 먹었을 때
			if (maze[character.y][character.x / 2] == '2')
			{
				++coins;
				maze[character.y][character.x / 2] = '0';
			}
			// 아이템을 먹었을 때
			{

				if (maze[character.y][character.x / 2] == '3' && isInsPower)
				{
					maze[character.y][character.x / 2] = '0';
					// 파워업 상태 돌입
					powerTime = 0;
					isPowerUp = true;
					gameSpeed = 100;
				}
			}
			// 딜레이마다 파워업 시간 추가(코루틴 대체)
			if (isPowerUp && powerTime < 20)
			{
				PowerTime();
			}
			else
			{
				isPowerUp = false;
				if (tempGameSpeed != 0)
				{
					gameSpeed = tempGameSpeed;

				}
			}
			// 포탈을 탔을때
			{
				// 위쪽 포탈
				if (character.y == 0 && character.x == 20 && maze[0][10] == '9')
				{
					character.y = 19;
					character.x = 20;
				}
				// 오른쪽 포탈
				else if (character.y == 10 && character.x == 40 && maze[10][20] == '8')
				{
					character.y = 10;
					character.x = 2;
				}
				// 밑쪽 포탈
				else if (character.y == 20 && character.x == 20 && maze[20][10] == '9')
				{
					character.y = 1;
					character.x = 20;
				}
				// 왼쪽 포탈
				else if (character.y == 10 && character.x == 0 && maze[10][0] == '8')
				{
					character.y = 10;
					character.x = 38;
				}
			}

			// 출구로 들어갔을때
			{
				if (character.x / 2 == exitX && character.y == exitY)
				{
					isGameClear = true;
					// 클리어 실행
				}
			}
			// 적과 충돌했을 때
			for (int i = 0; i < 4; i++)
			{
				if (character.y == enemys[i]->y && character.x == enemys[i]->x)
				{
					// 파워업 상태 확인
					if (isPowerUp)
					{
						enemys[i]->x = 20;
						enemys[i]->y = 8;
					}
					else
					{
						isGameOver = true;
					}
				}
			}
		}
		// 진행상황 관리 코드
		{
			// 코인 개수 관련 (전체 코인 개수: 187개)
			if (coins >= 187)
			{
				nextcoins = 187;
				nextString = "출구를 찾아라!";
				beforeString = "속도 증가, 출구 생성";
				if (!isChangeExit)
				{
					isChangeExit = true;
					// 통로중 한군데가 출구로 변경
					ChangeExit();
					// 다른 통로의 입구 막기
					for (int i = 0; i < HEIGHT; i++)
					{
						for (int j = 0; j < WIDTH; j++)
						{

							switch (maze[i][j])
							{
							case '8': maze[i][j] = '1'; break;
							case '9': maze[i][j] = '1'; break;
							}
						}
					}
				}
				if (!isSpeedUp3)
				{
					isSpeedUp3 = true;
					// 몬스터 스피드 증가 (매개변수: 스피드 증가값?)
					SpeedUP(125);
				}
			}
			else if (coins >= 150)
			{
				nextcoins = 187;
				nextString = "속도 증가, 출구 생성";
				beforeString = "통로 생성, 몬스터 추가";
				if (!isSpeedUp2)
				{
					isSpeedUp2 = true;
					// 몬스터 스피드 증가 (매개변수: 스피드 증가값?)
					SpeedUP(150);
				}
			}
			else if (coins >= 125)
			{
				nextcoins = 150;
				nextString = "속도 증가";
				if (!isInsTelpo2)
				{
					isInsTelpo2 = true;
					// 통로 추가 생성
					InsTelpo(1);
				}
				if (!isInsMonster3)
				{
					isInsMonster3 = true;
					// 몬스터 추가 생성
					InsMonster(3);
				}
			}
			else if (coins >= 75)
			{
				nextcoins = 125;
				nextString = "통로 생성, 몬스터 추가";
				beforeString = "통로 생성, 몬스터 추가";
				if (!isInsTelpo1)
				{
					isInsTelpo1 = true;
					// 통로 생성 (매개변수: 테레포드 통로 순서)
					InsTelpo(0);
				}

				if (!isInsMonster2)
				{
					isInsMonster2 = true;
					// 몬스터 추가 생성
					InsMonster(2);
				}
			}
			else if (coins >= 50)
			{
				nextcoins = 75;
				nextString = "통로 생성, 몬스터 추가";
				beforeString = "파워업 아이템, 속도증가";
				if (!isInsPower)
				{
					isInsPower = true;
				}
				if (!isSpeedUp1)
				{
					isSpeedUp1 = true;
					// 몬스터 스피드 증가 (매개변수: 스피드 증가값?)
					SpeedUP(175);
				}
			}
			else if (coins >= 25)
			{
				nextcoins = 50;
				nextString = "파워업 아이템, 속도증가";
				beforeString = "몬스터 추가";
				if (!isInsMonster1)
				{
					isInsMonster1 = true;
					// 몬스터 추가 생성
					InsMonster(1);
				}
			}
			// 게임 초기상황
			else {
				nextString = "몬스터 추가";
				nextcoins = 25;
			}
		}

		// 맵 삭제이후 재생성
		{
			system("cls");
			Render();
			UIManager();

		}

		// 순차대로 위치 구하고 출력하기
		{
			if (isPowerUp)
			{
				textcolor(14);
				Position(enemy1.x, enemy1.y);
				printf("%s", enemy1.shape);

				Position(enemy2.x, enemy2.y);
				printf("%s", enemy2.shape);

				Position(enemy3.x, enemy3.y);
				printf("%s", enemy3.shape);

				Position(enemy4.x, enemy4.y);
				printf("%s", enemy4.shape);
			}
			else
			{
			textcolor(12);
			Position(enemy1.x, enemy1.y);
			printf("%s", enemy1.shape);

			textcolor(10);
			Position(enemy2.x, enemy2.y);
			printf("%s", enemy2.shape);
			textcolor(11);

			Position(enemy3.x, enemy3.y);
			printf("%s", enemy3.shape);
			textcolor(13);

			Position(enemy4.x, enemy4.y);
			printf("%s", enemy4.shape);
			}
			

			if (isPowerUp)
			{
				textcolor(15);
			}
			else
			{
				textcolor(14);
			}
			Position(character.x, character.y);
			printf("%s", character.shape);
		}

		Sleep(gameSpeed);

	}
	system("cls");
	if (isGameOver)
	{
		MoveWindow(GetConsoleWindow(), 150, 200, 1700, 700, true); // x, y, 너비, 높이순

	
		textcolor(7);
		printf("%s", bufferOver);
		system("pause");
	}
	else if (isGameClear)
	{
		MoveWindow(GetConsoleWindow(), 150, 200, 1700, 700, true); // x, y, 너비, 높이순

		FILE* fileClear = fopen("GameClear.txt", "r");
		char bufferClear[BUFFERSIZE] = { 0, };
		
		fread(bufferClear, 1, BUFFERSIZE, fileClear);
		
		textcolor(7);
		printf("%s", bufferClear);
		system("pause");
	}
	return 0;
}
