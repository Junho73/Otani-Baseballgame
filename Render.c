#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include "Render.h"
#include "Sound.h"

// 콘솔 제어 (x, y)좌표
void GotoXY(int x, int y) {
    COORD Pos; 
    Pos.X = x; 
    Pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

// 콘솔 창 텍스트 색상 변경
void SetColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// 콘솔 창에서 커서 숨기기
void HideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo = { 0, };
    cursorInfo.dwSize = 1; 
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

// 키보드 입력 확인
int GetKeyDown() {
    if (_kbhit() != 0) {
        int key = _getch();
        if (key == 224) return _getch();
        return key;
    }
    return 0;
}

// 화면 렌더링

//  KBO 로고 아스키 아트
void DrawTitle() {
    SetColor(11);
    GotoXY(5, 3); printf(" ######     ###     #####  ####### ######     ###    #       #       ");
    GotoXY(5, 4); printf(" #     #   #   #   #     # #       #     #   #   #   #       #       ");
    GotoXY(5, 5); printf(" ######   #######  #       #####   ######   #######  #       #       ");
    GotoXY(5, 6); printf(" #     #  #     #  #  #### #       #     #  #     #  #       #       ");
    GotoXY(5, 7); printf(" ######   #     #   #####  ####### ######   #     #  ####### ####### ");
    SetColor(14);
    GotoXY(28, 11); printf("=== 두근두근 오타니 ===");
    SetColor(7);
    GotoXY(25, 13); printf("~ KBO에 놀러온 오타니 쇼헤이 ~");
}

// 메인 타이틀 화면 P키를 눌러 시작하세요
void DrawBlinkMessage(MenuState* state) {
    GotoXY(28, 16);
    if (state->isBlinkVisible) { 
        SetColor(14); printf("[ PRESS 'P' TO START ]"); 
    }
    else { 
        printf("                      "); 
    } 
    SetColor(7);
}

// 대기실
void DrawMainMenu(int selectedIndex) {
    GotoXY(30, 16); SetColor(selectedIndex == 1 ? 14 : 7); printf("1. 이어하기  ");
    GotoXY(30, 17); SetColor(selectedIndex == 2 ? 14 : 7); printf("2. 새 게임   ");
    GotoXY(30, 18); SetColor(selectedIndex == 3 ? 14 : 7); printf("3. 게임 종료 ");
    SetColor(7);
}

// 로딩 연출
void ShowLoadingScreen(const char* stadiumName) {
    system("cls"); SetColor(15); GotoXY(18, 10);
    if (stadiumName == NULL) {
        printf(">> 게임 로딩중");
    }
    else {
        printf(">> 오타니가 %s(으)로 향합니다", stadiumName);
    }
    for (int i = 0; i < 3; i++) {
        Sleep(500);
        printf(".");
    }
    Sleep(500);
}

// 날씨
void DrawWeather(int weather) {
    if (weather == 0) return; // 맑음
    for(int i = 12; i < 23; i++) {
        GotoXY(0, i);
        for(int j=0; j<80; j++) {
            if (weather == 1 && rand() % 15 == 0) {
                SetColor(9); printf("│"); SetColor(10);
            }
    else if (weather == 2 && rand() % 20 == 0) {
                SetColor(15); printf("*"); SetColor(10);
            }
    else {
                printf(" ");
            }
        }
    }
}

// 인게임 스코어보드, 타자 정보, 베이스 상태
void DrawStadiumUI(SaveData* data, InGameState* state) {
    system("cls");   
    char* enemyName = KBO_Teams[GetTeamIndex(data->matchIndex)].teamName;
    GotoXY(25, 1); SetColor(15); printf("+--------------------------------+");
    GotoXY(25, 2); printf("| SCORE BOARD"); GotoXY(58, 2); printf("|");
    GotoXY(25, 3); printf("| 오타니  %d : %d  %-14s", state->scoreOhtani, state->scoreOpponent, enemyName); GotoXY(58, 3); printf("|");
    
    GotoXY(25, 4); printf("| B ");
    for(int i=0; i<3; i++) { if(i < state->balls) printf("■ "); else printf("□ "); }
    GotoXY(58, 4); printf("|");
    
    GotoXY(25, 5); printf("| S ");
    for(int i=0; i<2; i++) { if(i < state->strikes) printf("■ "); else printf("□ "); }
    GotoXY(58, 5); printf("|");
    
    GotoXY(25, 6); printf("| O ");
    for(int i=0; i<2; i++) { if(i < state->outs) printf("■ "); else printf("□ "); }
    GotoXY(58, 6); printf("|");
    
    GotoXY(25, 7); printf("+--------------------------------+");
    
    GotoXY(2, 2); SetColor(15); printf("▶ 조작법");
    GotoXY(2, 3); printf("S : 스윙(타격)");
    GotoXY(2, 4); printf("R : 도루(주자시)");
    GotoXY(2, 5); printf("Space : 투구, 번트");
    GotoXY(2, 7); SetColor(11);
    if(state->weather == 0) printf("날씨: 맑음");
    else if(state->weather == 1) printf("날씨: 비");
    else printf("날씨: 눈");
    
    GotoXY(60, 1); SetColor(14); printf("+-------+");
    GotoXY(60, 2); printf("|   %s", state->base2 ? "◆" : "◇"); GotoXY(68, 2); printf("|");
    GotoXY(60, 3); printf("| %s   %s", state->base3 ? "◆" : "◇", state->base1 ? "◆" : "◇"); GotoXY(68, 3); printf("|");
    GotoXY(60, 4); printf("|   ◇"); GotoXY(68, 4); printf("|");
    GotoXY(60, 5); printf("+-------+");
    
    time_t now = time(NULL);
    data->playTimeSeconds += (int)(now - state->lastTime);
    state->lastTime = now;
    int m = data->playTimeSeconds / 60;
    int s = data->playTimeSeconds % 60;
    GotoXY(60, 7); SetColor(14); printf("플레이타임: %02d:%02d", m, s);
    
    GotoXY(25, 8); 
    if (state->isBottom == 0) {
        SetColor(13); printf("[ 홈런 게이지: ");
        for (int i=0; i<10; i++) {
            if (i < data->hitGauge) printf("■"); else printf("□");
        }
        printf(" ]");
    }
    else {
        printf("                                    ");
    }
    
    GotoXY(32, 9); SetColor(15);
    printf("%-12s [ %d회 %s ]", enemyName, state->inning, state->isBottom ? "말" : "초");
    
    GotoXY(0, 11); SetColor(15);
    printf("================================================================================");
}

// 팀별 색상
void DrawPlayerModel(int type, int x, int y, int teamIdx) {
    int capColor = 15, bodyColor = 15, batColor = 6;
    switch(teamIdx) {
        case 0: capColor = 12; break; // 키움
        case 1: capColor = 14; break; // 한화
        case 2: capColor = 9; break;  // 삼성
        case 3: capColor = 12; break; // 롯데
        case 4: capColor = 12; break; // 기아
        case 5: capColor = 15; break; // 두산
        case 6: capColor = 11; break; // NC
        case 7: capColor = 12; break; // SSG
        case 8: capColor = 8; break;  // KT
        case 9: capColor = 12; break; // LG
        case 10: capColor = 9; break; // Team Korea
        default: capColor = 15; break;
    }
    
    GotoXY(x, y);
    if (type == 1) { 
        SetColor(capColor); printf("  ■■■"); GotoXY(x, y+1);
        SetColor(15); printf("  ■■■"); GotoXY(x, y+2);
        SetColor(bodyColor); printf("■■■■■"); SetColor(batColor); printf("■"); GotoXY(x, y+3);
        SetColor(15); printf("  ■  "); SetColor(batColor); printf("■"); GotoXY(x, y+4);
        SetColor(15); printf("■■■■");
    }
    else if (type == 2) { 
        SetColor(capColor); printf("  ■■■"); GotoXY(x, y+1);
        SetColor(15); printf("  ■■■"); GotoXY(x, y+2);
        SetColor(bodyColor); printf("■■■■■"); GotoXY(x, y+3);
        SetColor(15); printf("  ■  ■");
    }
    else { 
        SetColor(capColor); printf("  ■■■"); GotoXY(x, y+1);
        SetColor(bodyColor); printf("■■■■■"); GotoXY(x, y+2);
        SetColor(15); printf("  ■"); GotoXY(x, y+3);
        SetColor(15); printf("■  ■");
    }
}

// 경기 종료 후 컷신
void ShowPostGameStory(SaveData* data, InGameState* gs) {
    system("cls"); DrawTitle();
    bool isWin = gs->scoreOhtani > gs->scoreOpponent;
    int prevMatchIndex = data->matchIndex;
    
    GotoXY(5, 15); SetColor(15); printf("=========================================");
    if (isWin) {
        GotoXY(5, 17); SetColor(14); printf(">> 오타니 승리!! (+3000원)");
        data->matchIndex++; 
        data->money += 3000; 
    }
    else {
        GotoXY(5, 17); SetColor(12); printf(">> 오타니 패배... (+1000원)");
        data->money += 1000;
    }
    
    int m = data->playTimeSeconds / 60;
    int s = data->playTimeSeconds % 60;
    GotoXY(5, 19); SetColor(11); printf("플레이타임: %02d분 %02d초", m, s);
    
    GotoXY(5, 20); SetColor(10); 
    printf("경기 결과: 오타니 %d : %d %s", gs->scoreOhtani, gs->scoreOpponent, KBO_Teams[GetTeamIndex(prevMatchIndex)].teamName);
    
    GotoXY(5, 22); SetColor(13);
    if (data->matchIndex > 10) printf("모든 상대를 물리쳤습니다! 게임 클리어!");
    else printf("다음 상대는: %s", (data->matchIndex == 10) ? "???" : KBO_Teams[GetTeamIndex(data->matchIndex)].teamName);
    
    GotoXY(5, 24); SetColor(8); printf("스페이스바를 눌러서 로비로 돌아가기...");
    while(GetKeyDown() != 32) Sleep(50);
    
    if (isWin && prevMatchIndex == 9) {
        system("cls"); DrawTitle();
        GotoXY(5, 15); SetColor(11);
        printf("\"KBO에는 나의 적수가 없구나...\"");
        GotoXY(5, 16); SetColor(7);
        printf("- 오타니 쇼헤이 -");
        
        GotoXY(5, 24); SetColor(8);
        printf("스페이스바를 눌러서 다음으로...");
        while(GetKeyDown() != 32) Sleep(50);
        
        system("cls"); DrawTitle();
        GotoXY(5, 15); SetColor(14);
        printf("\"잠깐! 아직 우리가 남아있다!!\"");
        GotoXY(5, 16); SetColor(15);
        printf("우르르르... (KBO 최고 스타들이 유니폼을 맞춰 입고 나타난다)");
        GotoXY(5, 18); SetColor(10);
        printf("\"우리 대한민국 대표팀과 한 수 부탁드립니다!\"");
        
        GotoXY(5, 20); SetColor(11);
        printf("\"...좋다. 들어와라!\"");
        
        GotoXY(5, 24); SetColor(8);
        printf("스페이스바를 눌러서 최종전으로...");
        while(GetKeyDown() != 32) Sleep(50);
    }
    
    if (data->matchIndex > 10) {
        system("cls");
        SetColor(14);
        GotoXY(5, 10); printf("  ____    _    __  __ _____     ____ _     _____    _    ____  ");
        GotoXY(5, 11); printf(" / ___|  / \\  |  \\/  | ____|   / ___| |   | ____|  / \\  |  _ \\ ");
        GotoXY(5, 12); printf("| |  _  / _ \\ | |\\/| |  _|    | |   | |   |  _|   / _ \\ | |_) |");
        GotoXY(5, 13); printf("| |_| |/ ___ \\| |  | | |___   | |___| |___| |___ / ___ \\|  _ < ");
        GotoXY(5, 14); printf(" \\____/_/   \\_\\_|  |_|_____|   \\____|_____|_____/_/   \\_\\_| \\_\\");
        GotoXY(20, 20); SetColor(11);
        printf("KBO 도장깨기를 완료했습니다! 축하합니다!");
        GotoXY(20, 24); SetColor(8); printf("스페이스바를 눌러서 종료...");
        while(GetKeyDown() != 32) Sleep(50);
        exit(0);
    }
}