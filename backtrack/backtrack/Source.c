#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctype.h>
#include <malloc.h>
#include <stdbool.h>
#include <windows.h>
#include <locale.h>

#define N 100

int board[N][N];

void resetQueen(int i, int j, int n) {
	for (int x = 0; x < n; x++) {
		board[i][x] -= 1;
		board[x][j] -=1;
		int diag;
		diag = j - i + x; 
		if (diag >= 0 && diag < n) {
			board[x][diag] -= 1;
		}
		diag = j + i - x;
		if (diag >= 0 && diag < n) {
			board[x][diag] -= 1;
		}
	}
	board[i][j] = 0;

	//for (int i = 0; i < n; i++) {
	//	for (int j = 0; j < n; j++) {
	//		if (board[i][j] == -1)
	//			printf("[Q]");
	//		else printf("[ ]");
	//	}
	//	printf("\n");
	//}
	//fflush(stdin);
	//Sleep(1);
	//system("cls");
}

void setQueen(int i, int j, int n) {
	for (int x = 0; x < n; x++) {
		board[i][x] += 1;
		board[x][j] += 1;
		int diag;
		diag = j - i + x;
		if (diag >= 0 && diag < n) {
			board[x][diag] += 1;
		}
		diag = j + i - x;	
		if (diag >= 0 && diag < n) {
			board[x][diag] += 1;
		}
	}
	board[i][j] = -1;

	//for (int i = 0; i < n; i++) {
	//	for (int j = 0; j < n; j++) {
	//		if (board[i][j] == -1)
	//			printf("[Q]");
	//		else printf("[ ]");
	//	}
	//	printf("\n");
	//}
	//fflush(stdin);
	//Sleep(1);
	//system("cls");
}


bool tryQueen(int i, int n) {
	bool result = false;
	for (int j = 0; j < n; j++) {
		if (board[i][j] == 0) {
			setQueen(i, j, n);
			if (i == n-1)
				result = true;
			else
			{
				if (!(result = tryQueen(i+1, n)))
					resetQueen(i, j, n);
			}
		}
		if (result)
			break;
	}
	return result;
}


void initBoard(int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			board[i][j] = 0;
		}
	}
}

void main() {
	int n = 0;
	while (n < 4) {
		printf("Input size board\n");
		scanf("%d",&n);
	}
	initBoard(n);
	tryQueen(0,n);

	printf("RESULT\n");
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (board[i][j] == -1)
			printf("[Q]");
			else printf("[ ]");
		}
		printf("\n");
	}

}