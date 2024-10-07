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

bool isInsMonster1 = false; // ����1 �߰����� Ȯ��
bool isInsMonster2 = false; // ����2 �߰����� Ȯ��
bool isInsMonster3 = false; // ����3 �߰����� Ȯ��

bool isInsPower = false;// �Ŀ���  ���� Ȯ��

bool isSpeedUp1 = false;// �ӵ� 1���� Ȯ��
bool isSpeedUp2 = false;// �ӵ� 2���� Ȯ��
bool isSpeedUp3 = false;// �ӵ� 3���� Ȯ��

bool isInsTelpo1 = false; // ���1 ����
bool isInsTelpo2 = false; // ���2 ����

bool isChangeExit = false;// �ⱸ���� Ȯ��

bool isGameOver = false; // ���� ���� Ȯ��
bool isGameClear = false; // ���� Ŭ���� Ȯ��

bool isPowerUp = false; // �Ŀ��� ���� Ȯ��

int randomDir = 0;

int exitX = 0;
int exitY = 0;

int enemysDir[4] = { 0,0,0,0 }; // ���� �̵� ������ ����
bool enemysFind[4] = { false,false,false,false }; // ���� �÷��̾� ã���� ����

int powerTime = 0;



typedef struct Character
{
	int x;
	int y;
	const char* shape;
}Character;

// �÷��̾� ĳ���� ����
Character character = { 20,15,"��" };

// �� ĳ���� ����
Character enemy1 = { 20,8,"��" };
Character enemy2 = { 18,11,"��" };
Character enemy3 = { 20,11,"��" };
Character enemy4 = { 22,11,"��" };

Character* enemys[] = { &enemy1,&enemy2, &enemy3, &enemy4 };

int coins = 0; // �÷��̾ ���� ������ ����
int nextcoins = 0; // �÷��̾��� ���� ���� ��ǥ

char nextAction[]; // �÷��̾��� ���� ����� ��Ȳ

int gameSpeed = 200; // ���� �÷��� �ӵ�
int tempGameSpeed = 0; // ���� �÷��� �ӵ� �����

const char* nextString = ""; // ���� ���� ��Ȳ
const char* beforeString = ""; // ���� ��ǥ ��Ȳ 

void textcolor(int colorNum) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum);
}

void Position(int x, int y)
{
	// x�� y���� �����ϴ� ����ü�Դϴ�.
	COORD position = { x, y };

	// �ܼ� Ŀ���� ��ǥ�� �����ϴ� �Լ��Դϴ�.
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
// �� ���� ���� �ڵ�
void Render()
{
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			textcolor(7);
			switch (maze[i][j])
			{
				// �ƹ��͵� ����
			case '0': printf("  "); break;
				// �ƹ��� ������ ���ϴ� ��
			case '1':if (isPowerUp) { textcolor(4); }
					else if (isChangeExit)
			{
				textcolor(4);
			}
					else { textcolor(1); } printf("��"); break;
				// �÷��̾ �Ծ���� ����
			case '2':			textcolor(6);
				printf("��");  break;
				// �÷��̾ ������ ��ȭ�Ǵ� ������
			case '3': if (!isInsPower) { printf("  "); }
					else {
				textcolor(11);
				printf("��"); } break;
				// �����Ǵ� �ⱸ
			case'4': textcolor(14); printf("��"); break;
				// �� ������ �Ա�
			case '5': printf("��"); break;
				// ���� ���1 &  ���� ���2
			case '6':  textcolor(1); printf("��");
				break;
				// ���� ���2 & ���� ���2
			case '7': textcolor(1); printf("��");
				break;
			case '8': 			textcolor(2);
				printf("��"); break;
			case '9':			textcolor(4);
				printf("��"); break;
			}
		}
		printf("\n");
	}
}


// ���� �߰� ���� �ڵ�
void InsMonster(int enemyNum)
{
	enemys[enemyNum]->x = 20;
	enemys[enemyNum]->y = 8;
	isInsMonster1 = true;

}
// �Ŀ��� ���� ����
void PowerTime()
{
	// 30 ������?
	powerTime++;
}
// ���� ���ǵ� ����
void SpeedUP(int speed)
{
	gameSpeed = speed;
	tempGameSpeed = speed;
}
// �ڷ���Ʈ ��� ����
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
// �ⱸ ����
void  ChangeExit()
{
	// � ǥ���� ��ΰ� �ⱸ�� �� ���ΰ�
	int randomExit = rand() % 2; // 0�̳� 1�� ���� (1/2Ȯ��)

	// � ��ġ�� �ⱸ�� ����� �� ���ΰ�
	int randomPosition = rand() % 2; // 0�̳� 1�� ����
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
// UI�� �����ϴ� �Լ�
void UIManager()
{
	textcolor(7);
	// ��ǥ ���� & �̼� ���
	printf("���� �̼�:%d / %d --> %s  \n", coins, nextcoins, nextString);
	printf("\n");
	// �Ŀ��� ����â
	if (isPowerUp)
	{
		printf("�Ŀ� ��!  ���� & �ӵ�����\t�����ð�: %d\n", 20 - powerTime);
		printf("\n");
	}

	// ������ ����
	if (coins >= 25)
	{
		printf("���� ����:%s\n", beforeString);
	}


}

void main()
{

	char key = 0;
	// ���� (�ð�������� 12�ú��� 1,2,3,4)
	int dir = 0;

	MoveWindow(GetConsoleWindow(), 750, 200, 410, 600, true); // x, y, �ʺ�, ���̼�

	FILE* fileOver = fopen("GameOver.txt", "r");
	char bufferOver[BUFFERSIZE] = { 0, };
	
	fread(bufferOver, 1, BUFFERSIZE, fileOver);


	

	Render();
	UIManager();

	while (!isGameOver && !isGameClear)
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
			case 1: if (maze[character.y - 1][character.x / 2] != '1' && maze[character.y - 1][character.x / 2] != '7')  character.y--; break;
			case 2: if (maze[character.y][character.x / 2 + 1] != '1' && maze[character.y][character.x / 2 + 1] != '6') character.x += 2; break;
			case 3: if (maze[character.y + 1][character.x / 2] != '1' && maze[character.y + 1][character.x / 2] != '7') character.y++; break;
			case 4: if (maze[character.y][character.x / 2 - 1] != '1' && maze[character.y][character.x / 2 - 1] != '6') character.x -= 2; break;
			}
		}

		// �� �����̱� �ڵ�
		{
			srand(time(NULL));
			for (int i = 0; i < 4; i++)
			{
				// ���� ��ƾ߸� ������ȯ�� �ϴ� �ڵ�
				//	{
				//		// ���� ������ �� ���� ������ȯ X
				//		if (enemysDir[i] == 0)
				//		{
				//			randomDir = rand() % 4 + 1;
				//			enemysDir[i] = randomDir;
				//		}
				//	
				//		// �������� �����̱�
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
					// �������� �����̱�
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
		// �÷��̾��� �浹 �ڵ�
		{
			// ������ �Ծ��� ��
			if (maze[character.y][character.x / 2] == '2')
			{
				++coins;
				maze[character.y][character.x / 2] = '0';
			}
			// �������� �Ծ��� ��
			{

				if (maze[character.y][character.x / 2] == '3' && isInsPower)
				{
					maze[character.y][character.x / 2] = '0';
					// �Ŀ��� ���� ����
					powerTime = 0;
					isPowerUp = true;
					gameSpeed = 100;
				}
			}
			// �����̸��� �Ŀ��� �ð� �߰�(�ڷ�ƾ ��ü)
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
			// ��Ż�� ������
			{
				// ���� ��Ż
				if (character.y == 0 && character.x == 20 && maze[0][10] == '9')
				{
					character.y = 19;
					character.x = 20;
				}
				// ������ ��Ż
				else if (character.y == 10 && character.x == 40 && maze[10][20] == '8')
				{
					character.y = 10;
					character.x = 2;
				}
				// ���� ��Ż
				else if (character.y == 20 && character.x == 20 && maze[20][10] == '9')
				{
					character.y = 1;
					character.x = 20;
				}
				// ���� ��Ż
				else if (character.y == 10 && character.x == 0 && maze[10][0] == '8')
				{
					character.y = 10;
					character.x = 38;
				}
			}

			// �ⱸ�� ������
			{
				if (character.x / 2 == exitX && character.y == exitY)
				{
					isGameClear = true;
					// Ŭ���� ����
				}
			}
			// ���� �浹���� ��
			for (int i = 0; i < 4; i++)
			{
				if (character.y == enemys[i]->y && character.x == enemys[i]->x)
				{
					// �Ŀ��� ���� Ȯ��
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
		// �����Ȳ ���� �ڵ�
		{
			// ���� ���� ���� (��ü ���� ����: 187��)
			if (coins >= 187)
			{
				nextcoins = 187;
				nextString = "�ⱸ�� ã�ƶ�!";
				beforeString = "�ӵ� ����, �ⱸ ����";
				if (!isChangeExit)
				{
					isChangeExit = true;
					// ����� �ѱ����� �ⱸ�� ����
					ChangeExit();
					// �ٸ� ����� �Ա� ����
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
					// ���� ���ǵ� ���� (�Ű�����: ���ǵ� ������?)
					SpeedUP(125);
				}
			}
			else if (coins >= 150)
			{
				nextcoins = 187;
				nextString = "�ӵ� ����, �ⱸ ����";
				beforeString = "��� ����, ���� �߰�";
				if (!isSpeedUp2)
				{
					isSpeedUp2 = true;
					// ���� ���ǵ� ���� (�Ű�����: ���ǵ� ������?)
					SpeedUP(150);
				}
			}
			else if (coins >= 125)
			{
				nextcoins = 150;
				nextString = "�ӵ� ����";
				if (!isInsTelpo2)
				{
					isInsTelpo2 = true;
					// ��� �߰� ����
					InsTelpo(1);
				}
				if (!isInsMonster3)
				{
					isInsMonster3 = true;
					// ���� �߰� ����
					InsMonster(3);
				}
			}
			else if (coins >= 75)
			{
				nextcoins = 125;
				nextString = "��� ����, ���� �߰�";
				beforeString = "��� ����, ���� �߰�";
				if (!isInsTelpo1)
				{
					isInsTelpo1 = true;
					// ��� ���� (�Ű�����: �׷����� ��� ����)
					InsTelpo(0);
				}

				if (!isInsMonster2)
				{
					isInsMonster2 = true;
					// ���� �߰� ����
					InsMonster(2);
				}
			}
			else if (coins >= 50)
			{
				nextcoins = 75;
				nextString = "��� ����, ���� �߰�";
				beforeString = "�Ŀ��� ������, �ӵ�����";
				if (!isInsPower)
				{
					isInsPower = true;
				}
				if (!isSpeedUp1)
				{
					isSpeedUp1 = true;
					// ���� ���ǵ� ���� (�Ű�����: ���ǵ� ������?)
					SpeedUP(175);
				}
			}
			else if (coins >= 25)
			{
				nextcoins = 50;
				nextString = "�Ŀ��� ������, �ӵ�����";
				beforeString = "���� �߰�";
				if (!isInsMonster1)
				{
					isInsMonster1 = true;
					// ���� �߰� ����
					InsMonster(1);
				}
			}
			// ���� �ʱ��Ȳ
			else {
				nextString = "���� �߰�";
				nextcoins = 25;
			}
		}

		// �� �������� �����
		{
			system("cls");
			Render();
			UIManager();

		}

		// ������� ��ġ ���ϰ� ����ϱ�
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
		MoveWindow(GetConsoleWindow(), 150, 200, 1700, 700, true); // x, y, �ʺ�, ���̼�

	
		textcolor(7);
		printf("%s", bufferOver);
		system("pause");
	}
	else if (isGameClear)
	{
		MoveWindow(GetConsoleWindow(), 150, 200, 1700, 700, true); // x, y, �ʺ�, ���̼�

		FILE* fileClear = fopen("GameClear.txt", "r");
		char bufferClear[BUFFERSIZE] = { 0, };
		
		fread(bufferClear, 1, BUFFERSIZE, fileClear);
		
		textcolor(7);
		printf("%s", bufferClear);
		system("pause");
	}
	return 0;
}
