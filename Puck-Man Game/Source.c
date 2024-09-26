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


void Position(int x, int y)
{
	// x와 y축을 설정하는 구조체입니다.
	COORD position = { x, y };

	// 콘솔 커서의 좌표를 설정하는 함수입니다.
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}

char maze[HEIGHT][WIDTH] =
{
	{'1','1','1','1','1','1','1','1','1','1','6','1','1','1','1','1','1','1','1','1','1'},
	{'1','2','2','2','2','2','2','2','2','2','2','2','2','2','2','2','2','2','2','2','1'},
	{'1','1','2','1','2','1','1','1','2','1','2','1','2','1','1','1','2','1','2','1','1'},
	{'1','1','2','1','2','2','2','1','2','1','1','1','2','1','2','2','2','1','2','1','1'},
	{'1','2','2','1','2','1','3','1','2','2','2','2','2','1','3','1','2','1','2','2','1'},
	{'1','2','1','1','2','1','1','1','2','1','2','1','2','1','1','1','2','1','1','2','1'},
	{'1','2','2','2','2','2','2','2','2','2','2','2','2','2','2','2','2','2','2','2','1'},
	{'1','2','1','2','1','1','1','2','1','1','1','1','1','2','1','1','1','2','1','2','1'},
	{'1','2','1','2','2','2','2','0','0','0','0','0','0','0','2','2','2','2','1','2','1'},
	{'1','2','1','2','1','1','1','0','1','1','5','1','1','0','1','1','1','2','1','2','1'},
	{'6','2','1','2','1','2','2','0','1','0','2','0','1','0','2','2','1','2','1','2','6'},
	{'1','2','1','2','1','2','1','0','1','0','0','0','1','0','1','2','1','2','1','2','1'},
	{'1','2','2','2','2','2','1','0','1','1','1','1','1','0','1','2','2','2','2','2','1'},
	{'1','2','1','2','1','2','1','0','0','0','1','0','0','0','1','2','1','2','1','2','1'},
	{'1','2','1','3','1','2','2','2','1','2','1','2','1','2','2','2','1','3','1','2','1'},
	{'1','2','1','1','1','2','1','1','1','2','2','2','1','1','1','2','1','1','1','2','1'},
	{'1','2','2','2','2','2','2','1','1','1','2','1','1','1','2','2','2','2','2','2','1'},
	{'1','2','1','2','1','1','2','2','2','2','2','2','2','2','2','1','1','2','1','2','1'},
	{'1','2','1','2','2','2','2','1','1','1','2','1','1','1','2','2','2','2','1','2','1'},
	{'1','2','2','2','1','1','2','2','3','1','2','1','3','2','2','1','1','2','2','2','1'},
	{'1','1','1','1','1','1','1','1','1','1','6','1','1','1','1','1','1','1','1','1','1'},
};
// 맵 생성 관련 코드
void Render()
{
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{

			switch (maze[i][j])
			{
				// 아무것도 없음
			case '0': printf("  "); break;
				// 아무도 나가지 못하는 벽
			case '1': printf("■"); break;
				// 플레이어가 먹어야할 코인
			case '2': printf("ㆍ"); break;
				// 플레이어가 먹으면 강화되는 아이템
			case '3': printf("⊙"); break;
				// 생성되는 출구
			case'4': printf("★"); break;
				// 적 기지의 입구
			case '5': printf("□"); break;
				//막혀있는 통로 (나중에 그냥 벽으로 바꾸기 -- 코드 복작해짐)
			case '6': printf("◆"); break;
			}
		}
		printf("\n");
	}
}

bool isInsMonster = false; // 몬스터 추가생성 확인
bool isInsPower = false;// 파워업  생성 확인
bool isSpeedUp = false;// 속도 증가 확인
bool isChangeExit = false;// 출구생성 확인

// 몬스터 추가 생성 코드
void InsMonster(int enemyNum)
{
	if (!isInsMonster)
	{
		enemys[enemyNum]->x = 20;
		enemys[enemyNum]->y = 8;
		isInsMonster = true;
	}
	return 0;
}
// 파워업 아이템 생성
void InsPower()
{
	if (!isInsPower)
	{

	}
	return 0;
}
// 몬스터 스피드 증가
void SpeedUP(int speedNum)
{
	if (!isSpeedUp)
	{

	}
	return 0;
}
// 출구 생성
void  ChangeExit()
{
	if (!isChangeExit)
	{

	}
	return 0;
}
void main()
{
	
	
	
	char key = 0;
	// 방향 (시계방향으로 12시부터 1,2,3,4)
	int dir = 0;

	 Render();

	 while (1)
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
			case 1: if (maze[character.y - 1][character.x / 2] != '1')  character.y--; break;
			case 2: if (maze[character.y][character.x / 2 + 1] != '1') character.x += 2; break;
			case 3: if (maze[character.y + 1][character.x / 2] != '1') character.y++; break;
			case 4: if (maze[character.y][character.x / 2 - 1] != '1') character.x -= 2; break;

			}
		}
		
		// 적 움직이기 코드
		{
			srand(time(NULL));
			// int size = sizeof(enemys) / sizeof(Character); 값 이상함
			for (int i = 0; i < 4; i++)
			{
				
				 int randomDir = rand() % 4 + 1;
				 // 키 입력 받기
				 switch (randomDir)
				 {
				 case 1: if (maze[enemys[i]->y - 1][enemys[i]->x / 2] != '1'&& maze[enemys[i]->y - 1][enemys[i]->x / 2] != '5') enemys[i]->y--; break;
				 	break;
				 case 2:if (maze[enemys[i]->y][enemys[i]->x / 2 + 1] != '1' && maze[enemys[i]->y][enemys[i]->x / 2 + 1] != '5') enemys[i]->x += 2; break;
				 	break;												  
				 case 3:if (maze[enemys[i]->y + 1][enemys[i]->x / 2] != '1' && maze[enemys[i]->y + 1][enemys[i]->x / 2] != '5') enemys[i]->y++; break;
				 	break;												  
				 case 4: if (maze[enemys[i]->y][enemys[i]->x / 2 - 1] != '1' && maze[enemys[i]->y][enemys[i]->x / 2 - 1] != '5') enemys[i]->x -= 2; break;
				 	break;
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
			// 적과 충돌했을 때
			for (int i = 0; i < 4; i++)
			{
				if (character.y == enemys[i]->y && character.x == enemys[i]->x)
				{
					// 게임 오버 실행
				}
			}
		}
		// 진행상황 관리 코드
		{
			//	// 코인 개수 관련 (전체 코인 개수: 189개)
			//	if (coins >= 189)
			//	{
			//		// 통로중 한군데가 출구로 변경
			//		ChangeExit();
			//		// 몬스터 스피드 증가
			//		SpeedUP(2);
			//	}
			//	else if (coins >= 150)
			//	{
			//		// 몬스터 스피드 증가
			//		SpeedUP(1);
			//	}
			//	else if (coins >= 125)
			//	{
			//		// 통로 추가 생성
			//		InsTelpo(1);
			//		// 몬스터 추가 생성
			//		InsMonster(3);
			//	}
			//	else if (coins >= 75)
			//	{
			//		// 통로 생성 (매개변수: 테레포드 통로 순서)
			//		InsTelpo(0);
			//		// 몬스터 스피드 증가 (매개변수: 스피드 증가값?)
			//		SpeedUp(0);
			//		// 몬스터 추가 생성
			//		INsMonster(2);
			//	}
			//	else if (coins >= 50)
			//	{
			//		// 파워업 아이템 생성
			//		InsPower();
			//	}
			//	else if (coins >= 25)
			//	{
			//		// 몬스터 추가 생성
			//		InsMonster(1);
			//	} 
		}// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@해야할 것: 함수에서 한번씩 호출되게끔 만들기, 함수 만들기
		
		// 맵 삭제이후 재생성
		{
			 system("cls");
			 Render();
		}

		// 순차대로 위치 구하고 출력하기
		{
			Position(enemy1.x, enemy1.y);
			printf("%s", enemy1.shape);

			Position(enemy2.x, enemy2.y);
			printf("%s", enemy2.shape);

			Position(enemy3.x, enemy3.y);
			printf("%s", enemy3.shape);

			Position(enemy4.x, enemy4.y);
			printf("%s", enemy4.shape);

			Position(character.x, character.y);
			printf("%s", character.shape);
		}
		
		Sleep(200);

	}

	return 0;
}
