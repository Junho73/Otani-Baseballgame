#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include "GameLogic.h"
#include "Render.h"
#include "Sound.h"

// 메인
int main() {
	system("mode con cols=85 lines=35"); 
	system("title KBO 도장깨기 - 두근두근 오타니"); 
	HideCursor();
	PlayBGM();
	MenuState menu = { true, false, 0 }; 
	system("cls"); 
	DrawTitle();

	// 타이틀 대기 루프
	while (!menu.isGameStarted) {
		DrawBlinkMessage(&menu);
		if (_kbhit()) { 
			int key = _getch(); 
			if (key == 'p' || key == 'P') {
				menu.isGameStarted = true; 
			}
		}
		Sleep(100); 
		menu.tickCount++;
		if (menu.tickCount >= 5) { 
			menu.isBlinkVisible = !menu.isBlinkVisible; 
			menu.tickCount = 0; 
		}
	}
	ShowLoadingScreen(NULL);
	system("cls"); 
	DrawTitle();
	int selectedIndex = 1;

	// 메인 메뉴 선택 루프
	while (1) {
		DrawMainMenu(selectedIndex); 
		int key = GetKeyDown();
		
		if (key == 72) { // 방향키 위
			selectedIndex--; 
			if (selectedIndex < 1) {
				selectedIndex = 3; 
			}
		} 
		else if (key == 80) { // 방향키 아래
			selectedIndex++; 
			if (selectedIndex > 3) {
				selectedIndex = 1; 
			}
		} 
		else if (key == 32 || key == 13) { // 스페이스바 또는 엔터
			if (selectedIndex == 1) {
				LoadGame(); 
			}
			else if (selectedIndex == 2) {
				StartNewGame(); 
			}
			else {
				break; // 게임 종료
			}
			system("cls");
			DrawTitle(); // 로비에서 빠져나오면 다시 타이틀을 그려줌
		}
	}
	StopBGM();
	return 0;
}