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
	// x�� y���� �����ϴ� ����ü�Դϴ�.
	COORD position = { x, y };

	// �ܼ� Ŀ���� ��ǥ�� �����ϴ� �Լ��Դϴ�.
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

// �� ���� ���� �ڵ�
void Render()
{
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			switch (maze[i][j])
			{
				// �ƹ��͵� ����
			case '0': printf("  "); break;
				// �ƹ��� ������ ���ϴ� ��
			case '1': printf("��"); break;
				// �÷��̾ �Ծ���� ����
			case '2': printf("��"); break;
				// �÷��̾ ������ ��ȭ�Ǵ� ������
			case '3': printf("��"); break;
				// �����Ǵ� �ⱸ
			case'4': printf("��"); break;
				// �� ������ �Ա�
			case '5': printf("��"); break;
				//�����ִ� ��� (���߿� �׳� ������ �ٲٱ� -- �ڵ� ��������)
			case '6': printf("��"); break;
			}
		}
		printf("\n");
	}
}

// �÷��̾� �����̱� �ڵ�
void CharacterMove()
{

}
void main()
{
	// �÷��̾� ĳ���� ����
	Character character = { 20,15,"��" };

	// �� ĳ���� ����
	Character enemy1 = { 20,6,"��" };
	Character enemy2 = { 20,8,"��" };
	Character enemy3 = { 20,10,"��" };
	Character enemy4 = { 20,11,"��" };
	
	Character enemys[4] =
	{
		enemy1, enemy2, enemy3, enemy4
	};

	char key = 0;
	// ���� (�ð�������� 12�ú��� 1,2,3,4)
	int dir = 0;

	Render();

	while (1)
	{
		// �÷��̾� �����̱� �ڵ�
		{
			if (_kbhit())
			{
				key = _getch();

				if (key == -32)
				{
					key = _getch();
				}
				// Ű �Է� �ޱ�
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
			// �����̱�
			switch (dir)
			{
			case 1: if (maze[character.y - 1][character.x / 2] != '1')  character.y--; break;
			case 2: if (maze[character.y][character.x / 2 + 1] != '1') character.x += 2; break;
			case 3: if (maze[character.y + 1][character.x / 2] != '1') character.y++; break;
			case 4: if (maze[character.y][character.x / 2 - 1] != '1') character.x -= 2; break;

			}
		}
		// �� �����̱� �ڵ�
		{
			int size = sizeof(enemys) / sizeof(Character);
			for (int i = 0; i < 4; i++)
			{
				srand(time(NULL));
				int randomDir = rand() % 4 + 1;
				// Ű �Է� �ޱ�
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
		// �÷��̾��� �浹 �ڵ�
		{

		}
		// �����Ȳ ���� �ڵ�
		{

		}
		// �� �������� �����
		{
			system("cls");
			Render();
		}
		// ������� ��ġ ���ϰ� ����ϱ�
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