#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <stdbool.h>
#include <time.h>
#include "Member.h"

// 타이틀 화면 구조체
typedef struct {
	bool isBlinkVisible;
	bool isGameStarted;
	int tickCount;
} MenuState;

// 세이브 데이터 구조체
typedef struct {
	char playerName[30];
	int matchIndex;        // 0 ~ 10 (키움부터 팀 코리아까지의 스테이지 번호)
	int strength;          // 근력 스탯
	int agility;           // 민첩 스탯
	int endurance;         // 지구력 스탯
	int money;             // 게임 내 재화
	int playTimeSeconds;   // 총 누적 플레이 시간
	int hitGauge;          // 안타 게이지 
} SaveData;

// 인게임 상태 구조체
typedef struct {
	int inning;            // 현재 진행 중인 이닝
	int isBottom;          // 0: 초(오타니 공격), 1: 말(오타니 수비)
	int balls;
	int strikes;
	int outs;
	int scoreOhtani;
	int scoreOpponent;
	int base1;             // 1루 주자 유무 (1=있음, 0=없음)
	int base2;             // 2루 주자 유무
	int base3;             // 3루 주자 유무
	int currentBatterIndex;// 현재 상대팀 타자의 순서 번호
	int weather;           // 0: 맑음, 1: 비(페널티), 2: 눈(피로도 가속)
	int fielderX;          // 오타니 수비 시 수비수 X 좌표
	int fielderY;          // 오타니 수비 시 수비수 Y 좌표
	time_t lastTime;
} InGameState;

int GetTeamIndex(int matchIndex);                         // 스테이지 번호를 받아 실제 KBO 팀 인덱스를 반환
void SaveGameToFile(SaveData* data);                      // 플레이어 데이터를 파일(save.dat)에 저장
void LoadGame();                                          // 저장된 게임을 불러와 로비로 진입
void StartNewGame();                                      // 세이브를 초기화하고 처음부터 새 게임 시작
void ShowTutorial();                                      // 게임 조작법 및 규칙을 안내하는 튜토리얼 출력
void ShowSchedule(SaveData* data);                        // 현재 오타니가 상대해야 할 팀 일정표 출력
void RollInitialStats(SaveData* data);                    // 새 게임 시작 시 오타니의 초기 능력치를 랜덤 부여
void EnterGameLobby(SaveData* data);                      // 상점, 스탯 확인, 일정 확인 등을 할 수 있는 대기실(로비) 루프
void ShowStatus(SaveData* data);                          // 현재 오타니의 자금, 스탯, 플레이 타임 등을 화면에 출력
void VisitShop(SaveData* data);                           // 모은 자금으로 능력치를 올리는 상점 루프
void PlayNextMatch(SaveData* data);                       // 다음 스테이지 팀과의 경기를 준비하고 실행
bool PlayGame(SaveData* data);                            // 3이닝 동안 진행되는 1경기의 메인 루프 (승리 시 true 반환)

void PlayBatting(SaveData* data, InGameState* gs);        // 오타니 공격 턴 (타이밍 맞춰 S를 눌러 타격 및 도루)
bool PlayPitching(SaveData* data, InGameState* gs);       // 오타니 수비 턴 중 투구 (타이밍 맞춰 하단 게이지를 멈춤)
void PlayFielding(SaveData* data, InGameState* gs);       // 오타니 수비 턴 중 수비 (공의 낙구 지점을 찾아 이동)

#endif