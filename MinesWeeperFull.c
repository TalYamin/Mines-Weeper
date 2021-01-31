#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define EXIT 0
#define ROW_SIZE 22
#define COL_SIZE 22
#define MINE '*'
#define SPACE ' '
#define HIDDEN 'X'
#define MINES_AMOUNT 8
#define EXPOSE_NUM -1
#define FIRST_COL '0'
#define EIGHT_SIZE 8
#define TWELVE_SIZE 12
#define FIFTEEN_SIZE 15
#define OPEN_SQUARE 'O'
#define FLAG_SQUARE 'F'
#define INDEX_ROW_SIZE 0
#define INDEX_COL_SIZE 1
#define INDEX_ROW_MOVE 0
#define INDEX_COL_MOVE 1
#define INDEX_GAME_FLAG 0
#define INDEX_MOVE_COUNT 1


void printColumns(int colSize);
void printSpacers(int colSize);
int selectFromGameMenu();
int initGameMode(int gameModeSelection, int row_col_size[]);
void buildCustomBoard(int row_col_size[]);
int startGame(char displayBoard[][COL_SIZE], char hiddenBoard[][COL_SIZE], int rowSize, int colSize);
int calculateInitMinesAmount(int rowSize, int colSize);
int randomNumber(int size);
void initHiddenBoard(char hiddenBoard[][COL_SIZE], int rowSize, int colSize,
	int minesAmount);
void printBoard(char board[][COL_SIZE], int rowSize, int colSize);
void initDisplayBoard(char displayBoard[][COL_SIZE], int rowSize, int colSize);
int playGame(char hiddenBoard[][COL_SIZE], char displayBoard[][COL_SIZE],
	int rowSize, int colSize, int minesAmount);
char askForMove(char displayBoard[][COL_SIZE], int rowSize, int colSize, int move_row_col[]);
void makeMove(char displayBoard[][COL_SIZE], char hiddenBoard[][COL_SIZE], char moveSign, int moveRow, int moveCol, int rowSize, int colSize, int gameInfo[]);
void flagMove(char displayBoard[][COL_SIZE], int moveRow, int moveCol, int rowSize, int colSize);
int cheatMove(char displayBoard[][COL_SIZE], char hiddenBoard[][COL_SIZE], int moveCol, int rowSize, int colSize);
int nonMineMove(char displayBoard[][COL_SIZE], char hiddenBoard[][COL_SIZE], int moveRow, int moveCol, int rowSize, int colSize);
int gameLoss(char displayBoard[][COL_SIZE], char hiddenBoard[][COL_SIZE], int moveRow, int moveCol, int rowSize, int colSize);
int gameWin();
int countMinesAround(char hiddenBoard[][COL_SIZE], int moveRow, int moveCol,
	int rowSize, int colSize);
char checkSquareChar(int minesCounter);
int validateMove(char displayBoard[][COL_SIZE], int moveRow, int moveCol,
	int rowSize, int colSize, char moveSign);
int exposeNonMines(char displayBoard[][COL_SIZE], char hiddenBoard[][COL_SIZE],
	int rowSize, int colSize, int squaresAmount);
void markSquareWithFlag(char displayBoard[][COL_SIZE], int moveRow, int moveCol, int rowSize, int colSize);
char convertToUpperCase(char c);
int openSpace(char displayBoard[][COL_SIZE], char hiddenBoard[][COL_SIZE],
	int moveRow, int moveCol, int rowSize, int colSize);

/**********************************************************************************************
This program implements Mines Weeper game.

The program shows game menu in order to select board size.
User can choose board with size 8*8, 12*12, 15*15 and option of custom size.
If user inserts invalid input, message will be shown and user will have to insert valid input.

The program initializes hidden board which includes random mines squares, number
squares which represent the number of mines around and empty squares.
In addition, program initializes dispaly board which includes only
X squares, this board is visible to user.

The goal of the game is to discover all the mines and uncover
all the minless squares.
At any phase user choose square on board:
- If user selects mine square - game is over.
- If user selects number square - the number represents the number of mines
in 8 squares around.
- If user selects empty square - it means that there are no mines in 8
squares around.
- In addition, customer has an option to mark square with flag as
suspected to be mine.
***********************************************************************************************/





/*
Main function runs until user ask to exit or game is over.
Main calls to selectFromGameMenu() function in order user will choose
the board size or ask to exit. Then Main calls to initGameMode() function
in order to initialize the relevant board. Then after user insert
valid input and board is initialized - game is starting by calling
startGame() function. startGame() function return isGameOver flag
which will stop main function running.
*/
void main() {

	srand(time(NULL));

	int gameModeSelection = -1;
	int isGameOver = 0;
	int isInvalidChoice = 0;

	char hiddenBoard[ROW_SIZE][COL_SIZE];
	char displayBoard[ROW_SIZE][COL_SIZE];
	int row_col_size[2];

	while (gameModeSelection != EXIT && !isGameOver) {

		gameModeSelection = selectFromGameMenu();
		isInvalidChoice = initGameMode(gameModeSelection, row_col_size);

		if (!isInvalidChoice && gameModeSelection != EXIT) {
			isGameOver = startGame(displayBoard, hiddenBoard, row_col_size[INDEX_ROW_SIZE], row_col_size[INDEX_COL_SIZE]);
		}

	}

}



/*
This function prints game menu and enables to user to select board size.
This function returns int varaible of game mode selection,
 according to user selection. Assuming user will insert integer input.
*/
int selectFromGameMenu() {

	int gameModeSelection;

	printf("Welcome to Minesweeper!\n\n");
	printf("Please choose one of the following options and enter it's number:\n\n");
	printf("1 - for size 8X8\n\n");
	printf("2 - for size 12X12\n\n");
	printf("3 - for size 15X15\n\n");
	printf("4 - for custom size\n\n");
	printf("0 - Exit\n\n");

	scanf("%d", &gameModeSelection);
	return gameModeSelection;
}



/*
The function receives game mode selection of user and array which holds row size and col size.
According to user selection, this function allocates board size.
Using buildCustomBoard function for user custom selection.
If user asks to exit, relevant message will be shown.
If user insert invalid input, flag is activated.
Function return the flag of invalid input.
*/
int initGameMode(int gameModeSelection, int row_col_size[]) {

	int isInvalidChoice = 0;

	switch (gameModeSelection)
	{
	case 1:
		row_col_size[INDEX_ROW_SIZE] = EIGHT_SIZE;
		row_col_size[INDEX_COL_SIZE] = EIGHT_SIZE;
		break;
	case 2:
		row_col_size[INDEX_ROW_SIZE] = TWELVE_SIZE;
		row_col_size[INDEX_COL_SIZE] = TWELVE_SIZE;
		break;
	case 3:
		row_col_size[INDEX_ROW_SIZE] = FIFTEEN_SIZE;
		row_col_size[INDEX_COL_SIZE] = FIFTEEN_SIZE;
		break;
	case 4:
		buildCustomBoard(row_col_size);
		break;
	case 0: printf("Good bye.\n"); break;
	default:
		printf("Invalid choice.\n");
		isInvalidChoice = 1;
	}

	return isInvalidChoice;
}


/*
This function is used in order to initialize custom board
according to user selection. This function receives array of int
which holds row size and column size.
Function checks game limits for board size, and receive input until
this is valid input. Row size and column size which chosen,
are inserted to array.
*/
void buildCustomBoard(int row_col_size[]) {

	int rowSize = 0;
	int colSize = 0;

	do {
		printf("Please enter the size of the board (Lines <= 22 and Cols <= 22  ): ");
		scanf("%d%d", &rowSize, &colSize);
		if (rowSize < 1 || rowSize > 22) {
			printf("Number of lines is out of range!\n");
		}
		if (colSize < 1 || colSize > 22) {
			printf("Number of columns is out of range!\n");
		}
	} while (rowSize < 1 || colSize < 1 || rowSize > 22 || colSize > 22);
	row_col_size[INDEX_ROW_SIZE] = rowSize;
	row_col_size[INDEX_COL_SIZE] = colSize;
}


/*
This function is used to preaper boards for game.
calculates mines amount and initializes hidden board which includes random mines
squares, number squares which represent the number of mines around and empty squares.
In addition, program initializes dispaly board which includes only
X squares, this board is visible to user.
function prints the board, and calles to playGame function.
This function return isGameOver flag when playGame function is over.
*/
int startGame(char displayBoard[][COL_SIZE], char hiddenBoard[][COL_SIZE], int rowSize, int colSize) {

	int minesAmount = 0;
	int isGameOver = 0;

	minesAmount = calculateInitMinesAmount(rowSize, colSize);
	initHiddenBoard(hiddenBoard, rowSize, colSize, minesAmount);
	initDisplayBoard(displayBoard, rowSize, colSize);
	printBoard(displayBoard, rowSize, colSize);
	isGameOver = playGame(hiddenBoard, displayBoard, rowSize, colSize,
		minesAmount);

	return isGameOver;

}



/*
This function is used to print spacers.
This function receives int column size and spacers size will be determined
according to column size.
*/
void printSpacers(int colSize) {
	int i;
	printf("    ");
	for (i = 0; i < colSize; i++) {
		printf("____");
	}
	printf("____\n");
}

/*
This function is used to print columns graphics.
This function receives int column size and columns printing is
determined according to column size received.
In order to create aligned graphics, if column number is higher than 9,
margin is smaller.
This function calls printSpacers() function in order to print spacers after
printing columns.
*/
void printColumns(int colSize) {
	int i;
	printf("\n");
	printf("      ");
	for (i = 0; i < colSize; i++) {

		if (i > 9) {
			printf("  %d", i);
		}
		else {
			printf("   %d", i);
		}
	}
	printf("\n");
	printSpacers(colSize);
}



/*
This function is used to print board.
This fucntion receives char 2D array which represent board, int row size
and int col size. Function calls to printColumns() function in order to print
board colums graphics. Then, according to row size and column size, function
prints the chars which inclueded in 2D array which recived. In order to create
aligned graphics, if column number is higher than 9, margin is smaller.
And then, function calls printSpacers() in order to prints board graphics.
*/
void printBoard(char board[][COL_SIZE], int rowSize, int colSize) {
	int i, j;

	printColumns(colSize);
	for (i = 0; i < rowSize; i++) {
		for (j = 0; j < colSize; j++) {

			if (j == 0) {
				if (i > 9) {
					printf("    %d |", i);
				}
				else {
					printf("    %d  |", i);
				}
			}
			printf(" %c |", board[i][j]);
		}
		printf("\n");
	}

	printSpacers(colSize);
}


/*
This function used to calculae the amount of the mines, according to board size.
Function receives row size and column size.The number of mines in the board is
the integer value of the square root of the number of squares in the board.
Using sqrt() function from math.h. Function returns the mines amount.
*/
int calculateInitMinesAmount(int rowSize, int colSize) {
	int minesAmount;
	int squaresAmount;

	squaresAmount = rowSize * colSize;
	minesAmount = sqrt(squaresAmount);

	return minesAmount;
}


/*
This function is used to generate random number.
This function receives int size, the random number using rand() fucntion
from stdlib.h library. and function returns the random number.
*/
int randomNumber(int size) {

	int randomNum;
	randomNum = rand() % size;
	return randomNum;

}

/*
This function is used to count mines in 8 squares around specific square.
This function receives 2D array of char which represents hidden board,
int move row, int move column, int row size and column size.
This function holds mines counter which will represents the number of mines
in 8 squares around. Function receives move coordinates, that user selected.
According to move row and move column, function calculates
9 squares frame boundaries, as an area to check how many mines there are in 8
square around selected move square. In addition, there is validation that frame
boundaries are no out of scope of board 2D array,
by using row size and column size. Any time, that mine is found, mines counter
is raised. At the end, mines counter is returned.
*/
int countMinesAround(char hiddenBoard[][COL_SIZE], int moveRow, int moveCol,
	int rowSize, int colSize) {

	int i, j;
	int minesCounter = 0;

	for (i = moveRow - 1; i <= moveRow + 1; i++) {
		for (j = moveCol - 1; j <= moveCol + 1; j++) {

			if (i >= 0 && i < rowSize && j >= 0 && j < colSize) {
				if (hiddenBoard[i][j] == MINE) {
					minesCounter++;
				}
			}
		}
	}

	return minesCounter;

}


/*
This function is used to check which char has to be filled in square -
space char or number char. This function receives int mines counter.
If mines counter is 0 so space is the char which determined.
Assuming the maximal mines around specific square is 8 mines,
function converts according the ASCII table the mines counter to char which
represents the mines counter.
At the end, the relevant square char is returned.
*/
char checkSquareChar(int minesCounter) {

	char squareChar = SPACE;

	if (minesCounter != 0) {

		squareChar = minesCounter + '0';
	}

	return squareChar;
}



/*
This function is used to initialize hidden board.
hidden board includes random mines squares, number squares which represent
the number of mines around and empty squares.
This function receives 2D array of char which represents hidden board,
int row size, int col size and int mines amount.
At first, board is filled with mines according to mines amount, the location
in 2D array is randomly by using randomNumber() function.
Then, after all mines locations were determined, any square without
mine is filled with number of mines in 8 squares around or with space char in
case there are no mines it 8 squares around. The counting of mines around
specific square is done by countMinesAround() function. And the selection of
relevant char to fill the square - number or space, is done by
checkDisplayChar() function.
*/
void initHiddenBoard(char hiddenBoard[][COL_SIZE], int rowSize, int colSize,
	int minesAmount) {

	int i, j;
	int currentRow;
	int currentCol;
	int minesCounter;
	char squareChar;

	for (i = 0; i < minesAmount; i++) {

		do {

			currentRow = randomNumber(rowSize);
			currentCol = randomNumber(colSize);

		} while (hiddenBoard[currentRow][currentCol] == MINE);

		hiddenBoard[currentRow][currentCol] = MINE;
	}

	for (i = 0; i < rowSize; i++) {
		for (j = 0; j < colSize; j++) {
			if (hiddenBoard[i][j] != MINE) {
				minesCounter = countMinesAround(hiddenBoard, i, j, rowSize, colSize);
				squareChar = checkSquareChar(minesCounter);
				hiddenBoard[i][j] = squareChar;
			}
		}
	}

}


/*
This function is used to initialize display board.
Dispaly board is the board which is visible to user, and in any user's step,
the display board is updated and is shown to user. Display board at first is
initiailized with X char only. This function receives 2D array of char which
represent display board, int row size, and int col size. Accrding to row and
column sizes - filles board with X.
*/
void initDisplayBoard(char displayBoard[][COL_SIZE], int rowSize, int colSize) {
	int i, j;

	for (i = 0; i < rowSize; i++) {
		for (j = 0; j < colSize; j++) {
			displayBoard[i][j] = HIDDEN;
		}
	}

}


/*
This function manages game state and determine when game is over - with win or loss.

This function receives 2D array of char which represents hidden board,
2D array of char which represents display board,  int row size, int column size
and int mines amount.

This function calculates the number of moves to win according to row and column
size and according to mines amount. In addition, any legal move is counted by
move counter.

As long as the game is not over and moves counter is lower then moves to win
- there is calling to askForMove() function which ask from user to insert move.

Then, after asking from user for move and validating the move,
there is calling to makeMove() function which performs the move.

After move is done, array holds game over flag and move counter.
Game is keep running if game over flag is not activated and if there is no win yet.

In case, the game is not over because of loss, and the moves counter is
equal or bigger then moves to win - calls gameWin().

At the end, isGameOver flag is returned.
*/
int playGame(char hiddenBoard[][COL_SIZE], char displayBoard[][COL_SIZE], int
	rowSize, int colSize, int minesAmount) {


	char moveSign;
	int isGameOver = 0;
	int moveCount = 0;
	int movesToWin = (rowSize * colSize) - minesAmount;
	int move_row_col[2];
	int gameInfo[2]; // gameOver flag and moveCounter

	while (!isGameOver && moveCount < movesToWin){

		moveSign = askForMove(displayBoard, rowSize, colSize, move_row_col);
		makeMove(displayBoard, hiddenBoard, moveSign, move_row_col[INDEX_ROW_MOVE], move_row_col[INDEX_COL_MOVE], rowSize, colSize, gameInfo);
		isGameOver = gameInfo[INDEX_GAME_FLAG];
		moveCount += gameInfo[INDEX_MOVE_COUNT];
	}

	if (!isGameOver && movesToWin >= moveCount) {
		isGameOver = gameWin();
	}

	return isGameOver;

}


/*
This function is used to ask from user to insert move and to validate it.
This function receives 2D array of char which represents hidden board,
2D array of char which represents display board,  int row size, int column size
and array of move row and move column.
User has to select square to expose and which kind of move.
User can choose to open square ('O') or to mark square with a flag ('F').
When user inserts input, using validateMove() function checks if his input
represents a legal move. If no, he has to insert vaild input.
Assuming user will insert integer as input.
Function inserted to array the move row and move column which selected.
And fucntion returns move sign - 'O' or 'F'.
*/

char askForMove(char displayBoard[][COL_SIZE], int rowSize, int colSize, int move_row_col[]) {

	char moveSign;
	int moveRow;
	int moveCol;
	int isValidMove;

	do
	{
		moveSign = ' ';

		printf("Please enter your move, row and column : ");
		scanf("%d %d", &moveRow, &moveCol);

		if (moveRow != EXPOSE_NUM) {

			scanf(" %c", &moveSign);
		}

		moveSign = convertToUpperCase(moveSign);

		isValidMove = validateMove(displayBoard, moveRow, moveCol, rowSize, colSize, moveSign);

	} while (!isValidMove);

	move_row_col[INDEX_ROW_MOVE] = moveRow;
	move_row_col[INDEX_COL_MOVE] = moveCol;
	return moveSign;
}


/*
This function is used to perform the move which selected by user.

This function receives 2D array of char which represents hidden board,
2D array of char which represents display board, char moveSign, int row size, int column size
move row, move column, and array of gameInfo which holds game over flag and moves counter.

(1) In case that user choose F, calls flagMove().
(2) In case that move row is (-1), calls cheatMove().
(3) In case that square selected is not of mine, calls nonMineMove().
(4) In  case - which means no cheat using, and in case that selected square
includes mine - calls gameLoss().

In any case, legal moves are counted and added to move counter.
Function inserts gameOver flag and move counter to gameInfo array.

*/
void makeMove(char displayBoard[][COL_SIZE], char hiddenBoard[][COL_SIZE], char moveSign, int moveRow, int moveCol, int rowSize, int colSize, int gameInfo[]) {

	int moveCount = 0;
	int isGameOver = 0;


	if (moveSign == FLAG_SQUARE) {
		flagMove(displayBoard, moveRow, moveCol, rowSize, colSize);
	}
	else if (moveRow == EXPOSE_NUM) {
		moveCount += cheatMove(displayBoard, hiddenBoard, moveCol, rowSize, colSize);
	}
	else if (hiddenBoard[moveRow][moveCol] != MINE) {
		moveCount += nonMineMove(displayBoard, hiddenBoard, moveRow, moveCol, rowSize, colSize);
	}
	else {
		isGameOver = gameLoss(displayBoard, hiddenBoard, moveRow, moveCol, rowSize, colSize);
	}

	gameInfo[INDEX_GAME_FLAG] = isGameOver;
	gameInfo[INDEX_MOVE_COUNT] = moveCount;
}


/*
This function is used in case that user choose F, if square is hidden so square is marked
with flag. If square is already with flag, so flag is removed.
*/
void flagMove(char displayBoard[][COL_SIZE], int moveRow, int moveCol, int rowSize, int colSize) {
	markSquareWithFlag(displayBoard, moveRow, moveCol, rowSize, colSize);
	printBoard(displayBoard, rowSize, colSize);
}


/*
This function is used in case that move row is (-1), exposeNonMines() function is called as cheat
function which expose only squares without mines according to move col which
given. Function returns expose move counter.
*/
int cheatMove(char displayBoard[][COL_SIZE], char hiddenBoard[][COL_SIZE], int moveCol, int rowSize, int colSize) {
	int exposeMoveCount = 0;

	exposeMoveCount = exposeNonMines(displayBoard, hiddenBoard, rowSize, colSize, moveCol);
	printBoard(displayBoard, rowSize, colSize);

	return exposeMoveCount;
}

/*
This function is used in case that square selected is not of mine.
In case the square is number of mines around the display board is updated with the square which selected.
move is counted and added to move counter. In case that square is empty, there is using of recursive function openSpace()
in order that space will be displayed in place of the square and all the empty
squares in its vicinity are also exposed, until an area of empty squares is created,
surrounded by numbered squares. Function returns move counter.
*/
int nonMineMove(char displayBoard[][COL_SIZE], char hiddenBoard[][COL_SIZE], int moveRow, int moveCol, int rowSize, int colSize) {
	int moveCount = 0;

	if (hiddenBoard[moveRow][moveCol] == SPACE) {
		moveCount = openSpace(displayBoard, hiddenBoard, moveRow, moveCol, rowSize, colSize);
	}
	else {
		displayBoard[moveRow][moveCol] = hiddenBoard[moveRow][moveCol];
		moveCount++;
	}
	printBoard(displayBoard, rowSize, colSize);

	return moveCount;

}


/*
This function is used  in  case when no cheat using, and in case that selected square
includes mine - game over flag is activated, loss message is shown and hidden
board is printed fully.
Function returns game over flag.
*/
int gameLoss(char displayBoard[][COL_SIZE], char hiddenBoard[][COL_SIZE], int moveRow, int moveCol, int rowSize, int colSize) {

	int isGameOverLoss = 1;
	displayBoard[moveRow][moveCol] = hiddenBoard[moveRow][moveCol];
	printBoard(displayBoard, rowSize, colSize);
	printf("\n");
	printf("You've hit a bomb! Game over!\n");
	printBoard(hiddenBoard, rowSize, colSize);
	printf("Hope you have enjoyed playing !!!!!\n");

	return isGameOverLoss;
}

/*
This function is used in case, the game is not over because of loss, and the moves counter is
equal or bigger then moves to win, game over flag is activated, winner message
is shown and hidden board is printed fully.
Function returns game over flag.
*/
int gameWin() {
	int isGameOverWin = 1;
	printf("\n");
	printf("You won!! congragulations! CHAMPION!\n");
	printf("Hope you have enjoyed playing !!!!!\n");
	return isGameOverWin;
}



/*
This function used to convert lowercase char to uppercase char.
*/
char convertToUpperCase(char c) {
	if (c >= 'a' && c <= 'z') {
		c = c - ('a' - 'A');
	}
	return c;
}


/*
This fucntion validates move according to game rules and board size.
This function receives 2D array of char which represents display board,
int move row, int move column, int row size, int column size and move sign.
In case move sign is not flag, open or move row is not expose number,
isValidMove flag is activated as invalid..
In case of using cheat by (-1) in move row and move column is bigger then 0,
isValidMove flag is activated as vaild.
Function checks board frame, and in case that move row or move col is out of
scope, isValidMove flag is activated as invalid.
Function checks game rules, and if user's move is for square which already
exposed before or in case there is no flag, isValidMove flag is activated as invalid.
In case, there is flag in square and the move sign is to open the square,
isValidMove flag is activated as invalid.
At the end, isValidMove flag is returned.
*/
int validateMove(char displayBoard[][COL_SIZE], int moveRow, int moveCol, int
	rowSize, int colSize, char moveSign) {

	int isValidMove = 1;

	if (moveSign != FLAG_SQUARE && moveSign != OPEN_SQUARE && moveRow != EXPOSE_NUM) {

		isValidMove = 0;
		printf("Please enter a valid choice!\n");

	}
	else if ((moveRow != EXPOSE_NUM || moveCol <= 0)) {

		if (moveRow < 0 || moveRow >= rowSize || moveCol < 0 || moveCol >= colSize) {
			isValidMove = 0;
			printf("Please enter a valid choice!\n");
		}
		else if (displayBoard[moveRow][moveCol] != HIDDEN && displayBoard[moveRow][moveCol] != FLAG_SQUARE) {
			isValidMove = 0;
			printf("Invalid move, Please enter a valid choice!\n");
		}
		else if (displayBoard[moveRow][moveCol] == FLAG_SQUARE && moveSign == OPEN_SQUARE) {
			isValidMove = 0;
			printf("Invalid move, Please enter a valid choice!\n");
		}
	}

	return isValidMove;

}

/*
This function is used as cheat function to expose only squares without mines.
This function receives 2D array of char which represents display board,
2D array of char which represents hidden board, int row size, int column size
and int squares amount.
This function holds counter which counts how many squares are exposed.
According to row and column size, and as long as squares amount are bigger then
0, if square is hidden and is not mine, square is exposed and display board is
updated. In addition, squares amount is decreased and counter is raised.
At the end, exposeMoveCount is returned.
*/
int exposeNonMines(char displayBoard[][COL_SIZE], char hiddenBoard[][COL_SIZE],
	int rowSize, int colSize, int squaresAmount) {

	int i, j;
	int exposeMoveCount = 0;

	for (i = 0; i < rowSize && squaresAmount > 0; i++) {
		for (j = 0; j < colSize && squaresAmount > 0; j++) {

			if (hiddenBoard[i][j] != MINE && displayBoard[i][j] == HIDDEN) {
				displayBoard[i][j] = hiddenBoard[i][j];
				squaresAmount--;
				exposeMoveCount++;
			}
		}
	}

	return exposeMoveCount;
}

/*
This function used to mark square with flag and to remove flag if already exists.
Function receives 2D array of char which represents display board,
move row, move column, row size and column size.
If square is already marked with flag-  remove square.
Else, mark square with flag.
*/
void markSquareWithFlag(char displayBoard[][COL_SIZE], int moveRow, int moveCol, int rowSize, int colSize) {

	if (displayBoard[moveRow][moveCol] == FLAG_SQUARE) {
		displayBoard[moveRow][moveCol] = HIDDEN;
	}
	else {
		displayBoard[moveRow][moveCol] = FLAG_SQUARE;
	}

}


/*
This function is recursive function.

Function is used in case that square is empty. space will be displayed in place of
the square and all the empty squares in its vicinity are also exposed, until an area
of empty squares is created, surrounded by numbered squares.

Function receives 2D array of char which represents display board,
2D array of char which represents hidden board, move row, move column,
row size and column size.

In case that square is empty, hidden and not flagged -
the square is exposed, and the there is recursive call to check all the
8 squares around. Checking that there is no exceeding from the array frame.
In case that square is not empty, not flagged but empty
so the numbers squares are exposed.

All moves are counted and added to move counter.
Function returns the move counter.

*/
int openSpace(char displayBoard[][COL_SIZE], char hiddenBoard[][COL_SIZE], int moveRow, int moveCol, int rowSize, int colSize) {

	int i;
	int j;
	int moveCounter = 0;

	if (displayBoard[moveRow][moveCol] == HIDDEN && hiddenBoard[moveRow][moveCol] == SPACE) {

		displayBoard[moveRow][moveCol] = hiddenBoard[moveRow][moveCol];
		moveCounter++;

		for (i = moveRow - 1; i <= moveRow + 1; i++) {
			for (j = moveCol - 1; j <= moveCol + 1; j++) {

				if (i >= 0 && i < rowSize && j >= 0 && j < colSize) {
					if ((moveRow != i || moveCol != j)) {
						moveCounter += openSpace(displayBoard, hiddenBoard, i, j, rowSize, colSize);
					}
				}
			}

		}
	}
	else if (displayBoard[moveRow][moveCol] == HIDDEN) {
		displayBoard[moveRow][moveCol] = hiddenBoard[moveRow][moveCol];
		moveCounter++;
	}

	return moveCounter;
}
