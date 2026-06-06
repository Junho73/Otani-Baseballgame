#ifndef RENDER_H
#define RENDER_H

#include "GameLogic.h"

// 콘솔 조작 유틸리티
void GotoXY(int x, int y);                                   // 콘솔 창에서 지정한 (x, y) 좌표로 커서를 이동시킵니다.
void SetColor(int color);                                    // 콘솔 텍스트의 색상을 변경합니다.
void HideCursor();                                           // 화면에서 깜빡이는 커서를 숨겨서 UI를 깔끔하게 만듭니다.
int GetKeyDown();                                            // 키보드 입력을 감지하고, 눌린 키의 ASCII 값을 반환합니다.

// 화면 그리기 함수
void DrawTitle();                                            // 메인 타이틀 화면의 아스키 아트 로고를 그립니다.
void DrawBlinkMessage(MenuState* state);                     // 타이틀 화면에서 "아무 키나 누르세요" 메시지를 깜빡이게 합니다.
void ShowLoadingScreen(const char* stadiumName);             // 인게임 진입 전 경기장으로 향하는 로딩 연출을 보여줍니다.
void DrawWeather(int weather);                               // 현재 날씨(비, 눈)에 따라 화면에 날씨 이펙트를 출력합니다.
void DrawMainMenu(int selectedIndex);                        // 로비(대기실)의 메뉴 항목들을 출력하고 선택된 항목을 강조합니다.
void DrawPlayerModel(int type, int x, int y, int teamIdx);   // 투수/타자 도트를 해당 팀의 상징색에 맞춰 출력합니다.
void DrawStadiumUI(SaveData* data, InGameState* state);      // 인게임 스코어보드, 스트라이크/볼 카운트, 진루 상태 등 게임 UI를 그립니다.
void ShowPostGameStory(SaveData* data, InGameState* gs);     // 경기 종료 후 승리/패배 연출과 보상 획득, 컷신 스토리를 출력합니다.

#endif
