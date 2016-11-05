/****************************************************************************
* DONGGUK UNIVERSITY CSE PROGRAMMING LANGUAGE AND ITS PRACTICE
*
* 프로그래밍 언어와 실습 - 최종 프로젝트 (손윤식 교수님)
*
* 공과대학 컴퓨터공학과 2014112021 함의진
* C언어 프로그램의 객체지향화 및 JAVA 테트리스 AI 알고리즘 적용
*
* DATE : 2015 / 12 / 18
*****************************************************************************/

#include <iostream> // Include iostream header
#include <time.h> // Include time related funcion header
#include <conio.h> // Include console I/O header
#include <windows.h> // Include windows header for using system function

using namespace std; // Set namespace to standard STD

#define BOARD_WIDTH 10 // Definition of board width
#define BOARD_HEIGHT 20 // Definition of board height
#define BOARD_X 4 // Definition of board X
#define BOARD_Y 2 // Definition of board Y
#define DELAY 100 // Definition of time delay on game
#define LEVEL_INCREASE 30 // Definition of incresing unit on level
#define NONE -1 // Definition of meaningless block code

#define PLACE_OK 0 // THE MESSAGE WHICH WILL BE SENT WHEN THE TEST OF PLACING BLOCK HAS FINISHED - PLACING OK RESULT
#define PLACE_ROW_FILLED 1 // THE MESSAGE WHICH WILL BE SENT WHEN THE TEST OF PLACING BLOCK HAS FINISHED - ROW FILLED RESULT
#define PLACE_OUT_BOUNDS 2 // THE MESSAGE WHICH WILL BE SENT WHEN THE TEST OF PLACING BLOCK HAS FINISHED - OUT OF BOUND RESULT
#define PLACE_BAD 3 // THE MESSAGE WHICH WILL BE SENT WHEN THE TEST OF PLACING BLOCK HAS FINISHED - BAD RESULT

class BoardController; // Declaration for forward reference

enum ControlKeys { // Controlling key units
	UP = 72, DOWN = 80, LEFT = 75, RIGHT = 77, SPACE = 32
};

// Definition of block types [KIND OF BLOCK][BLOCK Y][BLOCK X]
static int blocks[28][4][4] = {
	{ { 0, 1, 0, 0 },{ 0, 1, 0, 0 },{ 0, 1, 0, 0 },{ 0, 1, 0, 0 } },
	{ { 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 1, 1, 1, 1 },{ 0, 0, 0, 0 } },
	{ { 0, 1, 0, 0 },{ 0, 1, 0, 0 },{ 0, 1, 0, 0 },{ 0, 1, 0, 0 } },
	{ { 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 1, 1, 1, 1 },{ 0, 0, 0, 0 } },
	{ { 1, 1, 0, 0 },{ 1, 1, 0, 0 },{ 0, 0, 0, 0 },{ 0, 0, 0, 0 } },
	{ { 1, 1, 0, 0 },{ 1, 1, 0, 0 },{ 0, 0, 0, 0 },{ 0, 0, 0, 0 } },
	{ { 1, 1, 0, 0 },{ 1, 1, 0, 0 },{ 0, 0, 0, 0 },{ 0, 0, 0, 0 } },
	{ { 1, 1, 0, 0 },{ 1, 1, 0, 0 },{ 0, 0, 0, 0 },{ 0, 0, 0, 0 } },
	{ { 0, 0, 0, 0 },{ 0, 1, 0, 0 },{ 1, 1, 1, 0 },{ 0, 0, 0, 0 } },
	{ { 0, 0, 0, 0 },{ 0, 1, 0, 0 },{ 1, 1, 0, 0 },{ 0, 1, 0, 0 } },
	{ { 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 1, 1, 1, 0 },{ 0, 1, 0, 0 } },
	{ { 0, 0, 0, 0 },{ 0, 1, 0, 0 },{ 0, 1, 1, 0 },{ 0, 1, 0, 0 } },
	{ { 0, 0, 0, 0 },{ 0, 0, 1, 0 },{ 1, 1, 1, 0 },{ 0, 0, 0, 0 } },
	{ { 0, 1, 1, 0 },{ 0, 0, 1, 0 },{ 0, 0, 1, 0 },{ 0, 0, 0, 0 } },
	{ { 1, 1, 1, 0 },{ 1, 0, 0, 0 },{ 0, 0, 0, 0 },{ 0, 0, 0, 0 } },
	{ { 1, 0, 0, 0 },{ 1, 0, 0, 0 },{ 1, 1, 0, 0 },{ 0, 0, 0, 0 } },
	{ { 0, 0, 0, 0 },{ 1, 0, 0, 0 },{ 1, 1, 1, 0 },{ 0, 0, 0, 0 } },
	{ { 0, 0, 1, 0 },{ 0, 0, 1, 0 },{ 0, 1, 1, 0 },{ 0, 0, 0, 0 } },
	{ { 1, 1, 1, 0 },{ 0, 0, 1, 0 },{ 0, 0, 0, 0 },{ 0, 0, 0, 0 } },
	{ { 1, 1, 0, 0 },{ 1, 0, 0, 0 },{ 1, 0, 0, 0 },{ 0, 0, 0, 0 } },
	{ { 0, 0, 0, 0 },{ 0, 1, 1, 0 },{ 1, 1, 0, 0 },{ 0, 0, 0, 0 } },
	{ { 0, 1, 0, 0 },{ 0, 1, 1, 0 },{ 0, 0, 1, 0 },{ 0, 0, 0, 0 } },
	{ { 0, 0, 0, 0 },{ 0, 1, 1, 0 },{ 1, 1, 0, 0 },{ 0, 0, 0, 0 } },
	{ { 0, 1, 0, 0 },{ 0, 1, 1, 0 },{ 0, 0, 1, 0 },{ 0, 0, 0, 0 } },
	{ { 0, 0, 0, 0 },{ 1, 1, 0, 0 },{ 0, 1, 1, 0 },{ 0, 0, 0, 0 } },
	{ { 0, 0, 1, 0 },{ 0, 1, 1, 0 },{ 0, 1, 0, 0 },{ 0, 0, 0, 0 } },
	{ { 0, 0, 0, 0 },{ 1, 1, 0, 0 },{ 0, 1, 1, 0 },{ 0, 0, 0, 0 } },
	{ { 0, 0, 1, 0 },{ 0, 1, 1, 0 },{ 0, 1, 0, 0 },{ 0, 0, 0, 0 } },
};

class Cursors {
	/************************************************
	* TYPE : CLASS
	* NAME : Cursors
	* USES : CONDUCTING CURSOR WITH SET/GET FUNCTION
	*************************************************/
public:
	void CursorVisible(bool blnCursorVisible)    // Console.CursorVisible = false;
	{
		CONSOLE_CURSOR_INFO cursorInfo;
		GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
		cursorInfo.bVisible = blnCursorVisible;
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
	}
	void SetCursorPosition(int cursorLeft, int cursorTop)    // Console.SetCursorPosition(posX, posY); 
	{
		int posX = cursorLeft;
		int posY = cursorTop;
		COORD pos = { (short)posX, (short)posY };
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	}
	COORD GetCursorPosition(void)
	{
		COORD cursor;
		CONSOLE_SCREEN_BUFFER_INFO cursorInfo;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
		cursor.X = cursorInfo.dwCursorPosition.X;
		cursor.Y = cursorInfo.dwCursorPosition.Y;
		return cursor;
	}
};

class Printer : public Cursors {
	/************************************************
	* TYPE : CLASS
	* NAME : Printer extends Cursors
	* USES : PRINT THE STRING WITH COORDINATION
	*************************************************/
public:
	void SetPrint(int x, int y, char *c) {
		// Print the message received by parameter on specific coordination - overloaded
		SetCursorPosition(x, y); cout << c << endl; // Set the position of cursor and print out
	}

	void SetPrint(int x, int y, char *c, int var) {
		// Print the message and value of variable received by parameter on specific coordination - overloaded
		SetCursorPosition(x, y); cout << c << var << endl; // Set the position of cursor and print out
	}
};

class BrainMove : public Cursors {
	/************************************************
	* TYPE : CLASS
	* NAME : BrainMove extends Cursors
	* USES : ARTIFICIAL INTELLIGENT FOR TETRIS
	*************************************************/
private:
	bool Status = false; // Status of AutoPlay
public:
	class bestMove {
	public:
		int bestX, bestY, bestPiece; // Information to boxed in object - unit of Brain Logic
	};
	int getBlockHeight(int rotation) { // Calculate the Height of a block from parameter
		int i, j, unit = 0;
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[rotation][i][j] == 1) { ++unit; break; }
			}
		}
		return unit;
	}

	int getBlockWidth(int rotation) { // Calculate the Width of a block from parameter
		int i, j, unit = 0;
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[rotation][j][i] == 1) { ++unit; break; }
			}
		}
		return unit;
	}

	BrainMove::bestMove getBestMove(BoardController *board, int BS); // Declaration for setting it to be member function
	double rateBoard(BoardController board); // Declaration for setting it to be member function

	bool isBrain() { // get AutoPlay status
		return Status;
	}
	void toggleBrain() { // Toggle the status of AutoPlay
		Status == false ? Status = true : Status = false;
	}
};

class BoardController : public Printer {
	/************************************************
	* TYPE : CLASS
	* NAME : BoardController extends Printer
	* USES : CONDUCTING MAIN GAME - TETRIS
	*************************************************/
private:
	BrainMove brain; // Get instance of BrainMove for autoPlay
	int score = 0, level = 1, speed = 180; // Declaration of Game score, level and game speed
	int Width = (BOARD_WIDTH + 2); // real Board width
	int Height = (BOARD_HEIGHT + 1); // real Board height
public:
	int virtualBoard[BOARD_HEIGHT + 1][BOARD_WIDTH + 2]; // Testing board for BrainMove::rateBoard() - virtual does not mean C++ virtual keyword
	int tvirtualBoard[BOARD_HEIGHT + 1][BOARD_WIDTH + 2]; // Debugging board - clone of virtual board on bestMove
	int board[BOARD_HEIGHT + 1][BOARD_WIDTH + 2] = { 0, }; // Real Tetris board

	int dropHeight(int BS, int x) { // Calculate the height on the situation if the block were dropped down
		int min = 10000, result, skirt[4] = { 0, }; // Declarations - skirt has same meaning with JAVA source
		for (int xx = 0; xx < 4; xx++) {
			for (int yy = 3; yy >= 0; yy--) {
				if (blocks[BS][yy][xx]) {
					skirt[xx] = yy;
					break;
				}
			}
		}
		for (int i = x; i < x + 4; i++) {
			if (skirt[i - x] - getColumnHeight(i, 1) < min) {
				min = skirt[i - x] - getColumnHeight(i, 1);
				result = i;
			}
		}
		return (getColumnHeight(result, 1) - skirt[result - x]); // same with JAVA
	}

	int getMaxHeight() { // Get Maximum height from virtual board for rating board
		int max = 0, temp;
		for (int x = 1; x < BOARD_WIDTH + 2; x++) {
			temp = getColumnHeight(x, 0);
			if (max <= temp) max = temp;
		}
		return BOARD_HEIGHT - max;
	}

	int getColumnHeight(int x, int on) { // Get height of specific column
		for (int y = 0; y < BOARD_HEIGHT; y++) {
			if (on == 0) { // get in virtualboard
				if (virtualBoard[y][x]) return y - 1;
			}
			else { // get in real board
				if (board[y][x]) return y - 1;
			}
		}
		return 0;
	}

	int getWidth() { // get Width
		return Width;
	}
	int getHeight() { // get Height
		return Height;
	}
	void ShowPoint() { // Simply printing the level and score
		SetPrint(41, 2, "┌─────────────┐");
		SetPrint(41, 3, "│        GAME BOARD        │");
		SetPrint(41, 4, "└─────────────┘");
		SetPrint(43, 6, "■ LEVEL - ", level);
		SetPrint(43, 8, "■ SCORE - ", score);
	}
	void CountScore(void) // Counts the score and increases the level
	{
		score += 10;
		if (score % 30 == 0) {
			level += 1;
			speed -= LEVEL_INCREASE;
		}
		ShowPoint();
	}
	void DrawField(void) { // Drawing field - tetris board
		int x, y;
		for (x = 1; x <= BOARD_WIDTH + 1; x++) {
			board[BOARD_HEIGHT][x] = 1; //board 배열 중앙 1인식
			SetPrint((BOARD_X)+x * 2, BOARD_Y + BOARD_HEIGHT, "━");
		}
		for (y = 0; y < BOARD_HEIGHT + 1; y++) {
			board[y][0] = 1; //board 배열 왼쪽 1인식
			SetPrint(BOARD_X, BOARD_Y + y, y == BOARD_HEIGHT ? "┗" : "┃");
		}
		for (y = 0; y < BOARD_HEIGHT + 1; y++) {
			board[y][BOARD_WIDTH + 1] = 1; //board 배열 오른쪽 1인식
			SetPrint(BOARD_X + (BOARD_WIDTH + 2) * 2, BOARD_Y + y, y == BOARD_HEIGHT ? "┛" : "┃");
		}
		board[BOARD_HEIGHT][0] = 1;
		board[BOARD_HEIGHT][BOARD_WIDTH + 1] = 1;
		puts(" ");
	}
	void WriteBlock(int rotation) {
		int i, j;
		COORD cursor = GetCursorPosition();
		if (CanPositionedAt(rotation, 0, 0)) {
			for (i = 0; i < 4; i++) { // 행 반복
				for (j = 0; j < 4; j++) { // 열 반복
					SetCursorPosition(cursor.X + (j * 2), cursor.Y + i);
					if (blocks[rotation][i][j] == 1) cout << "■";
				}
			}
			SetCursorPosition(cursor.X, cursor.Y);
		}
	}
	bool CanPositionedAt(int rotation, int move1, int move2) {
		int x, y, arrX, arrY;
		COORD pos = GetCursorPosition();
		arrX = ((pos.X + move1) / 2) - 2;
		arrY = (pos.Y + move2) - BOARD_Y;
		for (y = 0; y < 4; y++) for (x = 0; x < 4; x++) if ((blocks[rotation][y][x] == 1) && board[arrY + y][arrX + x] == 1) return false;  //보드와 벽돌 겹침
		return true;  //겹치지 않음
	}
	void BoardInit(int n, int move1, int move2)
	{
		COORD pos = GetCursorPosition();
		int arrX = pos.X + move1, arrY = pos.Y + move2, x, y;
		/*커서위치정보->배열위치정보 변경*/
		arrX = arrX / 2 - 2; //콘솔좌표->배열 열 변환값
		arrY = arrY - 2;     //콘솔좌표->배열 행 변환값
							 //보드판에서 블록 이동시 1인식
		for (y = 0; y < 4; y++) for (x = 0; x < 4; x++) if (blocks[n][y][x] == 1) board[arrY + y][arrX + x] = 1;
	}

	void Stepper(int column)
	{
		int y, x;
		for (y = column; y >= 0; y--) for (x = 1; x <= 10; x++) board[y][x] = board[y - 1][x];
		for (x = 1; x <= 10; x++) board[0][x] = 0;
		for (y = 1; y <= 19; y++) for (x = 1; x <= 10; x++)
			SetPrint((BOARD_X)+x * 2 + 1, y + BOARD_Y, board[y][x] == 1 ? "■" : "  ");
	}
	void ClearBlock(int rotation, int move1, int move2)
	{
		int x, y;
		COORD cursor = GetCursorPosition();
		if (CanPositionedAt(rotation, move1, move2) == true) {
			for (y = 0; y < 4; y++) {
				for (x = 0; x < 4; x++) {
					SetCursorPosition(cursor.X + (x * 2), cursor.Y + y);
					if (blocks[rotation][y][x] == 1) cout << " ";
				}
			}
			SetCursorPosition(cursor.X + move1, cursor.Y + move2);
		}
	}
	void RemoveLine()
	{
		int i, x, y, z = 0;
		for (y = BOARD_HEIGHT - 1; y >= 1; y--) {
			for (z = 0; z < 4; z++) {
				i = 0;
				for (x = 1; x <= BOARD_WIDTH; x++) {
					if (board[y][x] == 1) {
						i++;
						if (i == BOARD_WIDTH) {
							for (x = 1; x < 11; x++) SetPrint((x + 2) * 2, y + 2, "  ");
							CountScore();
							Stepper(y);
						}
					}
				}
			}
		}
	}

	void StartGame() { // Conducting main game
		int n, kb, c = 2, currentX, currentY; // Declaration
		srand((unsigned)time(0)); // Initializes seed of random
		while (1) {
			//블록 생성 위치 좌표(13, 2)에서 시작 
			SetCursorPosition(13, 2);
			n = (int)(rand() % 6); // 0~27까지의 인덱스 생성 : 블록 종류 결정
			n *= 4;
			BrainMove::bestMove temp = brain.getBestMove(this, n);
			if (level >= 10) // 레벨이 10에 도달하면 게임 승리
			{
				/**************************************
				*     SIMPLE INFORMATION PRINTING
				*      NO DETAIL COMMENT ON HERE
				***************************************/
				SetPrint(41, 13, "┌─────────────┐");
				SetPrint(41, 14, "│      ☆ GAME CLEAR ☆    │");
				SetPrint(41, 15, "└─────────────┘");
				getchar(); // Wait for pressing any key
				exit(1);
			}
			if (!CanPositionedAt(n, 0, 0)) break;
			while (1) {
				int ww = 0, k = 0;
				while (!_kbhit()) {
					if (brain.isBrain()) { // if the status of brain autoPlay is ON
						// Get current location
						COORD pos = GetCursorPosition();
						currentX = ((pos.X) / 2) - 2;
						currentY = (pos.Y) - BOARD_Y;
						
						int BestMove = DOWN; // Initializes the BestMove command
						// Deciding appropriate command with BrainMove::BestMove object
						if (n != temp.bestPiece) BestMove = UP;
						else if (currentX > temp.bestX) BestMove = LEFT;
						else if (currentX < temp.bestX) BestMove = RIGHT;
						else BestMove = DOWN;
						switch (BestMove) {
						case LEFT: ClearBlock(n, -2, 0); WriteBlock(n); break;
						case RIGHT: ClearBlock(n, 2, 0); WriteBlock(n); break;
						case UP: k = n / 4; k *= 4;
							if ((n + 1) <= (k + 3)) k = n + 1;
							if (CanPositionedAt(k, 0, 0)) {
								ClearBlock(n, 0, 0);
								n = k;
								WriteBlock(n);
								break;
							}
							break;
						case DOWN: ClearBlock(n, 0, 2); break;
						default: break;
						}
					}
					WriteBlock(n);
					Sleep(DELAY + speed);
					//아래이동시 1있는지 확인
					if (CanPositionedAt(n, 0, 1) == false) {
						ww = 1;
						BoardInit(n, 0, 0);//보드 벽돌 배열 1추가
						RemoveLine();
						break;
					}
					ClearBlock(n, 0, 1);  //board배열 +1행 
				}
				/*CanPositionedAt함수에서 배열값 1발견시 중지*/
				if (ww == 1) break;
				kb = _getch();
				switch (kb)
				{
				case 'a':
					system("cls");
					cout << getMaxHeight() << endl;
					for (int xx = 0; xx < getHeight(); xx++)
					{
						for (int yy = 0; yy < getWidth(); yy++)
						{
							cout << tvirtualBoard[xx][yy];

						}
						cout << endl;
					}
					_getch();
					system("cls");
					DrawField();
					ShowPoint();
					break;
				case 'B': case 'b': brain.toggleBrain();  break;
				case LEFT: ClearBlock(n, -2, 0); WriteBlock(n); break;
				case RIGHT: ClearBlock(n, 2, 0); WriteBlock(n); break;
				case UP: k = n / 4; k *= 4;
					if ((n + 1) <= (k + 3)) k = n + 1;
					if (CanPositionedAt(k, 0, 0)) {
						ClearBlock(n, 0, 0);
						n = k;
						WriteBlock(n);
						break;
					}
					break;
				case DOWN: ClearBlock(n, 0, 2); break;
				case SPACE:
					while (1) {
						ClearBlock(n, 0, 1);
						if (!CanPositionedAt(n, 0, 1)) {
							WriteBlock(n);
							BoardInit(n, 0, 0);
							break;
						}
					}
				default: break;
				} // end switch
			} // end while(블록)        
		} // end while(게임)
	}
};

double BrainMove::rateBoard(BoardController board) { // Illustration of LameBrain.java in JAVA sources
	// Rating the board and return final result with aggravation
	int width = board.getWidth() - 2;
	int maxHeight = board.getMaxHeight() - 1;
	int sumHeight = 0;
	int holes = 0;
	for (int x = 1; x<width - 1; x++) {
		int colHeight = board.getColumnHeight(x, 0);
		sumHeight += BOARD_HEIGHT - colHeight;
		int y = board.getHeight() - 1;

		while (y >= colHeight) {
			if (!board.virtualBoard[y][x]) holes++;
			y--;
		}
	}
	double avgHeight = ((double)sumHeight) / width; // Calculating average of the height

	return (8.0 * maxHeight + 40.0 * avgHeight + 1.25 * holes); // Rate of board
}

BrainMove::bestMove BrainMove::getBestMove(BoardController *board, int BS) { // Illustraion of LameBrain.java in JAVA sources
	double bestScore = 1e20; // Initializes the bestScore with very big value for finding minimum
	double score; // Declaration
	BrainMove::bestMove temp; // Constructing temporary object for returning bestMove
	temp.bestPiece = NONE; // Initializes
	temp.bestX = 0; // Initializes
	temp.bestY = 0; // Initializes
	int limitHeight = board->getHeight(); // get Limit Height
	for (int i = 0; i < 4; i++) { // Loop for all rotation
		int yBound = limitHeight - getBlockHeight(BS + i) - 1; // Set Bound of Y
		int xBound = board->getWidth() - getBlockWidth(BS + i) - 1; // Set Bound of X
		// For current rotation, try all the possible columns
		for (int x = 1; x < xBound; x++) {
			int y = board->dropHeight(BS + i, x);
			if (y<yBound) {	// piece does not stick up too far
				memcpy(board->virtualBoard, board->board, sizeof(board->board));
				// MAKING CLONE OBJECT OF BOARD NAMED VIRTUAL BOARD *IMPORTANT CHANGE*

				for (int yy = 0; yy < 4; yy++) {
					for (int xx = 0; xx < 4; xx++) {
						if (blocks[BS + i][yy][xx]) board->virtualBoard[y + yy][x + xx] = 1;
						// DRAWING THE BLOCK ON VIRTUAL BOARD *IMPORTANT CHANGE*
					}
				}
				score = rateBoard(*board); // Rating the score of virtual board

				if (score < bestScore) { // Finding minimum score
					memcpy(board->tvirtualBoard, board->virtualBoard, sizeof(board->virtualBoard));
					temp.bestPiece = BS + i;
					temp.bestX = x;
					temp.bestY = y;
					bestScore = score;
				}
			}
		}
	}
	return temp; // returns temp
}

class SimplePrinter : public Printer {
	/************************************************
	* TYPE : CLASS
	* NAME : SimplePrinter extends Printer
	* USES : PRINTING SIMPLE INFORMATION WITH PRINTER
	*************************************************/
public:
	void ConsoleInit() {
		/**************************************
		*     SIMPLE INFORMATION PRINTING
		*      NO DETAIL COMMENT ON HERE
		***************************************/
		SetPrint(0, 3, "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
		SetPrint(29, 5, "C++ CONSOLE TETRIS GAME");
		SetPrint(0, 7, "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
		SetPrint(23, 9, "┌────────────────┐");
		SetPrint(23, 10, "│         < HOW TO PLAY >        │");
		SetPrint(23, 11, "│                                │");
		SetPrint(23, 12, "│ * [→]     블록 오른쪽 이동    │");
		SetPrint(23, 13, "│ * [←]     블록 왼쪽 이동      │");
		SetPrint(23, 14, "│ * [↑]     블록 회전           │");
		SetPrint(23, 15, "│ * [↓]     블록 한 칸 내림     │");
		SetPrint(23, 16, "│ * [SPACE]  블록 떨어뜨리기     │");
		SetPrint(23, 17, "│ * [B]      오토 플레이         │");
		SetPrint(23, 18, "│   - PRESS ANY KEY TO START -   │");
		SetPrint(23, 19, "└────────────────┘");
		SetPrint(0, 21, "        ☆  DGU CSE PROGRAMMING LANGUAGE AND ITS PRACTICE 2014112021  ☆        ");
		CursorVisible(false); // Hide cursor
		getch(); // Wait for pressing any key
		system("cls"); // Clear console
		CursorVisible(false); // Hide cursor
		SetCursorPosition(0, 0); // Initialize for drawing board
	}
	void EndGame() {
		/**************************************
		*     SIMPLE INFORMATION PRINTING
		*      NO DETAIL COMMENT ON HERE
		***************************************/
		SetPrint(41, 13, "┌─────────────┐");
		SetPrint(41, 14, "│      ☆ GAME OVER! ☆    │");
		SetPrint(41, 15, "└─────────────┘");
		_getch();
		system("cls");
		exit(0);
	}
};

void main() { // main Start
	BoardController mainGame; // Get instance of BoardController for conducting game
	SimplePrinter mainPrinter; // Get instance of SimplePrinter for simple printing

	mainPrinter.ConsoleInit(); // Initialize the console with BoardController

	mainGame.DrawField(); // Drawing Field
	mainGame.ShowPoint(); // Show Point of the game
	mainGame.StartGame(); // Start the game

	mainPrinter.EndGame(); // End game if the StartGame has been halted
} // main End
