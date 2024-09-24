#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>

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

// 플레이어 움직이기 코드
void CharacterMove()
{

}
void main()
{
	// 플레이어 캐릭터 생성
	Character character = { 20,15,"●" };

	// 적 캐릭터 생성
	Character enemy1 = { 20,6,"△" };
	Character enemy2 = { 20,8,"▽" };
	Character enemy3 = { 20,10,"◁" };
	Character enemy4 = { 20,11,"▷" };
	
	Character enemys[4] =
	{
		enemy1, enemy2, enemy3, enemy4
	};

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
			int size = sizeof(enemys) / sizeof(Character);
			for (int i = 0; i < 4; i++)
			{
				srand(time(NULL));
				int randomDir = rand() % 4 + 1;
				// 키 입력 받기
				switch (randomDir)
				{
				case 1: if (maze[enemys[i].y - 1][enemys[i].x / 2] != '1') enemys[i].y--; break;
					break;
				case 2:if (maze[enemys[i].y][enemys[i].x / 2 + 1] != '1') enemys[i].x += 2; break;
					break;												  
				case 3:if (maze[enemys[i].y + 1][enemys[i].x / 2] != '1') enemys[i].y++; break;
					break;												  
				case 4: if (maze[enemys[i].y][enemys[i].x / 2 - 1] != '1') enemys[i].x -= 2; break;
					break;
				}
			}
		}
		// 플레이어의 충돌 코드
		{

		}
		// 진행상황 관리 코드
		{

		}
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