#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include "GameLogic.h"
#include "Render.h"
#include "Sound.h"

Team KBO_Teams[11] = {
    {"키움 히어로즈", "고척 스카이돔", {"후라도", "헤이수스"}, {"이주형", "도슨", "김혜성", "최주환", "송성문", "이형종", "김휘집", "김재현", "이용규", "임지열", "박수종"}, 70, 70},
    {"한화 이글스", "한화생명 이글스파크", {"류현진", "페냐"}, {"최인호", "페라자", "노시환", "안치홍", "채은성", "문현빈", "이진영", "최재훈", "이도윤", "김태연", "하주석"}, 72, 72},
    {"삼성 라이온즈", "삼성 라이온즈 파크", {"원태인", "레예스"}, {"김지찬", "이재현", "구자욱", "맥키넌", "류지혁", "강민호", "김영웅", "김성윤", "김현준", "오재일", "윤정빈"}, 75, 75},
    {"롯데 자이언츠", "사직 야구장", {"반즈", "윌커슨"}, {"윤동희", "고승민", "레이예스", "전준우", "정훈", "유강남", "손호영", "박승욱", "김민석", "황성빈", "노진혁"}, 78, 78},
    {"KIA 타이거즈", "기아 챔피언스 필드", {"네일", "양현종"}, {"박찬호", "김도영", "나성범", "최형우", "소크라테스", "이우성", "김선빈", "한준수", "최원준", "서건창", "이창진"}, 80, 80},
    {"두산 베어스", "잠실 야구장", {"알칸타라", "곽빈"}, {"정수빈", "허경민", "라모스", "김재환", "양석환", "강승호", "양의지", "김대한", "전민재", "조수행", "이유찬"}, 82, 82},
    {"NC 다이노스", "창원 NC 파크", {"하트", "카스타노"}, {"박민우", "손아섭", "박건우", "데이비슨", "권희동", "서호철", "김성욱", "김형준", "김주원", "도태훈", "최정원"}, 85, 85},
    {"SSG 랜더스", "SSG 랜더스필드", {"김광현", "엘리아스"}, {"최지훈", "박성한", "최정", "에레디아", "한유섬", "고명준", "이지영", "하재훈", "안상현", "오태곤", "전의산"}, 88, 88},
    {"KT 위즈", "KT 위즈 파크", {"쿠에바스", "고영표"}, {"로하스", "천성호", "강백호", "문상철", "장성우", "황재균", "조용호", "배정대", "김상수", "심우준", "조대현"}, 90, 90},
    {"LG 트윈스", "잠실 야구장", {"켈리", "엔스"}, {"홍창기", "박해민", "김현수", "오스틴", "문보경", "박동원", "문성주", "구본혁", "신민재", "오지환", "김범석"}, 92, 92},
    {"대한민국 대표팀", "도쿄 돔", {"곽빈", "문동주"}, {"이정후", "김하성", "김도영", "강백호", "노시환", "최정", "박건우", "양의지", "박성한", "김혜성", "에드먼"}, 99, 99}
};
// 스테이지 번호 -> KBO 팀 인덱스
int GetTeamIndex(int matchIndex) {
    if (matchIndex >= 10) return 10;
    int order[10] = { 0, 5, 4, 3, 8, 6, 2, 7, 1, 9 }; 
    if (matchIndex >= 0 && matchIndex < 10) return order[matchIndex];
    return 0;
}
// 저장 로직
void SaveGameToFile(SaveData* data) {
    FILE* f = fopen("save.dat", "wb");
    if (f) { 
        fwrite(data, sizeof(SaveData), 1, f); 
        fclose(f); 
    }
}
// 저장 데이터 불러오기
void LoadGame() {
    FILE* file = fopen("save.dat", "rb");
    if (file == NULL) { 
        system("cls"); GotoXY(15, 12); SetColor(12); 
        printf("저장된 게임 파일이 없습니다! '새 게임'을 시작해주세요."); 
        Sleep(2000); 
        return; 
    }
    SaveData data; 
    fread(&data, sizeof(SaveData), 1, file); 
    fclose(file);
    EnterGameLobby(&data);
}
// 조작법 및 초반 스토리
void ShowTutorial() {
    system("cls"); DrawTitle();
    GotoXY(5, 15); SetColor(15); printf("오타니는 KBO에 놀러와서 작년 꼴등팀부터 1위팀까지");
    GotoXY(5, 16); printf("오타니 VS KBO 도장깨기를 진행하기로 했다.");
    GotoXY(5, 18); SetColor(14); printf("자, 작년 꼴찌팀 키움 히어로즈를 먼저 상대하도록 하자.");
    GotoXY(5, 21); SetColor(8);  printf("스페이스바를 눌러 다음으로...");
    while(GetKeyDown() != 32) Sleep(50);

    system("cls"); DrawTitle();
    GotoXY(5, 15); SetColor(11); printf("[ 게임 규칙 및 팁 ]");
    GotoXY(5, 16); SetColor(15); printf("- 콜드게임: 점수차가 10점 이상 벌어지면 공수교대시 해당 경기는 즉시 종료!");
    GotoXY(5, 17); SetColor(15); printf("- 날씨효과: [비] 헛스윙/볼 제구확률 증가, [눈] 피로도 페널티 3회부터 앞당겨짐");
    GotoXY(5, 18); SetColor(15); printf("- 스탯강화: 경기에서 얻은 상금으로 상점에서 스탯을 올릴 수 있습니다.");
    GotoXY(5, 21); SetColor(8);  printf("스페이스바를 눌러 대기실로 이동...");
    while(GetKeyDown() != 32) Sleep(50);
}

// 경기 일정
void ShowSchedule(SaveData* data) {
    while(_kbhit()) _getch();
    system("cls");
    GotoXY(5, 3); SetColor(11); printf("=== KBO 도장깨기 경기 일정 ===");
    
    for (int i = 0; i < 11; i++) {
        GotoXY(5, 5 + i);
        if (i < data->matchIndex) {
            SetColor(8); 
            printf("[완료] Stage %-2d: %s", i + 1, (i == 10) ? "???" : KBO_Teams[GetTeamIndex(i)].teamName);
        }
    else if (i == data->matchIndex) {
            SetColor(14); 
            printf(">>>> Stage %-2d: %s (도전)", i + 1, (i == 10) ? "???" : KBO_Teams[GetTeamIndex(i)].teamName);
        }
    else {
            SetColor(15); 
            printf("[대기] Stage %-2d: %s", i + 1, (i == 10) ? "???" : KBO_Teams[GetTeamIndex(i)].teamName);
        }
    }
    
    GotoXY(5, 18); SetColor(8); printf("돌아가려면 아무 키나 누르세요...");
    while(!_kbhit()) Sleep(50);
    while(_kbhit()) _getch();
}

// 새 게임 로직
void StartNewGame() {
    SaveData data;
    strcpy(data.playerName, "오타니");
    data.matchIndex = 0;
    data.money = 3000;
    data.playTimeSeconds = 0;
    data.hitGauge = 0;
    RollInitialStats(&data);
    ShowTutorial();
    EnterGameLobby(&data);
}
// 메이플 주사위
void RollInitialStats(SaveData* data) {
    int tryCount = 3;
    srand((unsigned int)time(NULL));
    while (tryCount > 0) {
        system("cls");
        GotoXY(5, 5); SetColor(15); printf("=== 초기 스탯 굴리기 (남은 횟수: %d) ===", tryCount);
        
        int r1 = (rand() % 6) + 1; int r2 = (rand() % 6) + 1; data->strength = r1 + r2;
        r1 = (rand() % 6) + 1; r2 = (rand() % 6) + 1; data->agility = r1 + r2;
        r1 = (rand() % 6) + 1; r2 = (rand() % 6) + 1; data->endurance = r1 + r2;
        
        GotoXY(5, 7); SetColor(14); printf("근력: %d  민첩: %d  지구력: %d", data->strength, data->agility, data->endurance);
        GotoXY(5, 9); SetColor(8); printf("이 스탯으로 결정하시겠습니까? (Y/N)");
        
        while (1) {
            int key = GetKeyDown();
            if (key == 'y' || key == 'Y') return;
            if (key == 'n' || key == 'N') { tryCount--; break; }
            Sleep(50);
        }
    }
}
// 대기실 
void EnterGameLobby(SaveData* data) {
    int selectedIndex = 1;
    system("cls");
    while (1) {
        GotoXY(5, 5); SetColor(15); printf("=== 대기실 ===");
        
        SetColor(selectedIndex == 1 ? 14 : 7); GotoXY(10, 7); printf("1. 다음 경기 (vs %-16s)", KBO_Teams[GetTeamIndex(data->matchIndex)].teamName);
        SetColor(selectedIndex == 2 ? 14 : 7); GotoXY(10, 8); printf("2. 경기 일정 보기               ");
        SetColor(selectedIndex == 3 ? 14 : 7); GotoXY(10, 9); printf("3. 내 스탯 확인                 ");
        SetColor(selectedIndex == 4 ? 14 : 7); GotoXY(10, 10); printf("4. 스탯 상점 가기               ");
        SetColor(selectedIndex == 5 ? 14 : 7); GotoXY(10, 11); printf("5. 저장하고 나가기             ");
        
        int key = GetKeyDown();
        if (key == 72) { selectedIndex--; if(selectedIndex < 1) selectedIndex = 5; }
    else if (key == 80) { selectedIndex++; if(selectedIndex > 5) selectedIndex = 1; }
    else if (key == 13 || key == 32) {
            if (selectedIndex == 1) { PlayGame(data); system("cls"); }
    else if (selectedIndex == 2) { ShowSchedule(data); system("cls"); }
    else if (selectedIndex == 3) { ShowStatus(data); system("cls"); }
    else if (selectedIndex == 4) { VisitShop(data); system("cls"); }
    else if (selectedIndex == 5) { SaveGameToFile(data); return; }
        }
        Sleep(50);
    }
}
// 스탯
void ShowStatus(SaveData* data) {
    while(_kbhit()) _getch();
    system("cls");
    GotoXY(5, 5); SetColor(14); printf("플레이어: %s (자금: %d원)                           ", data->playerName, data->money);
    GotoXY(5, 7); SetColor(11); printf("근력: %d  |  민첩: %d  |  지구력: %d", data->strength, data->agility, data->endurance);
    GotoXY(5, 9); SetColor(15); printf("- 근력: 홈런 확률 증가");
    GotoXY(5, 10); SetColor(15); printf("- 민첩: 안타 및 도루 확률 증가");
    GotoXY(5, 11); SetColor(15); printf("- 지구력: 4회 이후 피로도(CC기: 헛스윙/파울 증가) 방어");
    GotoXY(5, 13); SetColor(8); printf("돌아가려면 아무 키나 누르세요...");
    while(!_kbhit()) Sleep(50);
    while(_kbhit()) _getch();
}
// 상점
void VisitShop(SaveData* data) {
    int sel = 1;
    system("cls");
    while(1) {
        GotoXY(5, 5); SetColor(15); printf("자금: %-6d원   ", data->money);
        GotoXY(5, 6); SetColor(11); printf("[현재 스탯] 근력: %d | 민첩: %d | 지구력: %d   ", data->strength, data->agility, data->endurance);
        SetColor(sel == 1 ? 14 : 7); GotoXY(10, 7); printf("1. 근력 +1 (500원)  ");
        SetColor(sel == 2 ? 14 : 7); GotoXY(10, 8); printf("2. 민첩 +1 (500원)  ");
        SetColor(sel == 3 ? 14 : 7); GotoXY(10, 9); printf("3. 지구력 +1 (500원)");
        SetColor(sel == 4 ? 14 : 7); GotoXY(10, 10); printf("4. 랜덤 스탯향상 비약 (1000원)");
        SetColor(sel == 5 ? 14 : 7); GotoXY(10, 11); printf("5. 나가기            ");
        
        int k = GetKeyDown();
        if (k == 72) { sel--; if(sel<1) sel=5; }
    else if (k == 80) { sel++; if(sel>5) sel=1; }
    else if (k == 13 || k == 32) {
            if (sel == 5) return;
            
            if (sel == 1 && data->strength >= 50) { GotoXY(10, 13); SetColor(12); printf("근력 최대치(50) 도달!     "); Sleep(1000); GotoXY(10, 13); printf("                         "); continue; }
            if (sel == 2 && data->agility >= 50) { GotoXY(10, 13); SetColor(12); printf("민첩 최대치(50) 도달!     "); Sleep(1000); GotoXY(10, 13); printf("                         "); continue; }
            if (sel == 3 && data->endurance >= 50) { GotoXY(10, 13); SetColor(12); printf("지구력 최대치(50) 도달!   "); Sleep(1000); GotoXY(10, 13); printf("                         "); continue; }
            
            int price = (sel == 4) ? 1000 : 500;
            if (data->money >= price) {
                data->money -= price;
                if (sel == 1) data->strength++;
                else if (sel == 2) data->agility++;
                else if (sel == 3) data->endurance++;
                else if (sel == 4) {
                    for(int i=0; i<3; i++) {
                        int r = rand() % 3;
                        if (r == 0 && data->strength < 50) data->strength++;
                        else if (r == 1 && data->agility < 50) data->agility++;
                        else if (r == 2 && data->endurance < 50) data->endurance++;
                    }
                }
                GotoXY(10, 13); SetColor(10); printf("구매 완료!      "); Sleep(1000); GotoXY(10, 13); printf("                ");
            }
    else {
                GotoXY(10, 13); SetColor(12); printf("돈이 부족합니다!"); Sleep(1000); GotoXY(10, 13); printf("                ");
            }
        }
        Sleep(50);
    }
}
// 인게임 로직
bool PlayGame(SaveData* data) {
    data->playTimeSeconds = 0;
    ShowLoadingScreen(KBO_Teams[GetTeamIndex(data->matchIndex)].stadiumName);
    srand((unsigned int)time(NULL));
    InGameState state = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, rand() % 3, 50, 12, time(NULL)};
    
    while (state.inning <= 5) {
        bool inningOver = false;
        state.currentBatterIndex = 0; 
        
        while (!inningOver) {
            DrawStadiumUI(data, &state);
            DrawWeather(state.weather);
            
            if (state.isBottom == 0) { 
                PlayBatting(data, &state);
                
            }
    else { 
                bool hit = PlayPitching(data, &state);
                if (hit) PlayFielding(data, &state);
            }
            
            if (state.outs >= 3) {
                state.outs = 0; state.balls = 0; state.strikes = 0;
                state.base1 = 0; state.base2 = 0; state.base3 = 0;
                
                if (state.isBottom == 0) data->hitGauge = 0;
                
                if (abs(state.scoreOhtani - state.scoreOpponent) >= 10) {
                    state.inning = 6;
                    inningOver = true;
                    break;
                }
                
                state.isBottom = !state.isBottom;
                if (state.isBottom == 0) state.inning++;
                inningOver = true;
            }
            Sleep(50);
        }
    }
    
    bool isWin = state.scoreOhtani > state.scoreOpponent;
    ShowPostGameStory(data, &state);
    return isWin;
}
int getTier(int stat) {
    if (stat >= 50) return 5;
    if (stat >= 40) return 4;
    if (stat >= 30) return 3;
    if (stat >= 20) return 2;
    if (stat >= 10) return 1;
    return 0;
}
// 타자 로직
void PlayBatting(SaveData* data, InGameState* gs) {
    int pitcherX = 38, pitcherY = 12;
    int batterX = 38, batterY = 18;
    int msgX = batterX + 15, msgY = batterY + 2;
    int teamIdx = GetTeamIndex(data->matchIndex);
    int fatigueStart = (gs->weather == 2) ? 3 : 4;
    
    DrawPlayerModel(2, pitcherX, pitcherY, teamIdx);
    DrawPlayerModel(1, batterX, batterY, 10);
    
    GotoXY(pitcherX - 2, pitcherY - 1); SetColor(11); printf("%s", KBO_Teams[teamIdx].pitchers[0]);
    GotoXY(batterX, batterY - 1); SetColor(10); printf("오타니");
    if (gs->inning >= fatigueStart) { SetColor(12); printf(" (CC기: 피로도가 찼습니다.)"); }
    
    GotoXY(2, 23); SetColor(14); printf("투수가 공을 던질 준비를 합니다...                        ");
    
    int waitTime = 1000 + (rand() % 2000);
    Sleep(waitTime);
    while(_kbhit()) _getch(); 
    
    GotoXY(2, 23); SetColor(14); printf("타이밍 맞게 S를 눌러 스윙! (번트: Space, 도루: R)                  ");
    
    int ballX = pitcherX, ballY = pitcherY;
    bool isHit = false;
    bool isSwing = false;
    bool isBunt = false;
    bool stealAttempt = false;
    
    while (ballY < batterY) {
        GotoXY(ballX, ballY); printf("o");
        Sleep(100);
        GotoXY(ballX, ballY); printf(" ");
        ballY++;
        
        if (_kbhit()) {
            int key = _getch();
            if (key == 'm' || key == 'M') { gs->scoreOhtani += 100; gs->inning = 6; gs->outs = 3; return; }
    else if (key == 's' || key == 'S') {
                isSwing = true;
                if (abs(ballY - batterY) <= 1) { isHit = true; }
                break;
            }
    else if (key == 32) {
                isSwing = true;
                isBunt = true;
                if (abs(ballY - batterY) <= 1) { isHit = true; }
                break;
            }
    else if ((key == 'r' || key == 'R') && ((gs->base1 && !gs->base2) || (gs->base2 && !gs->base3))) {
                stealAttempt = true;
                break;
            }
        }
    }
    
    if (stealAttempt) {
        GotoXY(msgX, msgY);
        int successChance = 50 + (data->agility * 2);
        if ((rand() % 100) < successChance) {
            SetColor(10); printf("도루 성공!");
            if (gs->base2) { gs->base3 = 1; gs->base2 = 0; }
            if (gs->base1) { gs->base2 = 1; gs->base1 = 0; }
        }
    else {
            SetColor(12); printf("도루 실패!");
            if (gs->base2 && !gs->base3) gs->base2 = 0; else if (gs->base1 && !gs->base2) gs->base1 = 0;
            gs->outs++;
        }
        Sleep(1500);
        return; 
    }
    
    GotoXY(msgX, msgY);
    if (isSwing) {
        if (isHit) {
            if (isBunt) {
                if (rand() % 2 == 0) {
                    SetColor(10); printf("번트 성공! (1루 진루)");
                    if (gs->base3) { gs->scoreOhtani++; gs->base3 = 0; }
                    if (gs->base2) { gs->base3 = 1; gs->base2 = 0; }
                    if (gs->base1) { gs->base2 = 1; }
                    gs->base1 = 1;
                    gs->balls = 0; gs->strikes = 0;
                }
    else {
                    SetColor(12); printf("번트 실패! (아웃)");
                    gs->outs++;
                    gs->balls = 0; gs->strikes = 0;
                }
            }
    else {
                int r = rand() % 100;
                int tier = getTier(data->strength);
                int diffPen = data->matchIndex * 2; // 0 ~ 20% Penalty
                int hitChance = 40 + (tier * 2) - diffPen;
                int hrChance = 3 + tier;
                int foulChance = 37 - tier + (diffPen / 2);
                int missChance = 10 - tier + (diffPen / 2) + (diffPen % 2);
                if (hitChance < 0) hitChance = 0;
                
                if (gs->weather == 1) {
                    missChance += 15;
                    foulChance -= 5;
                    hitChance -= 10;
                }
                
                if (gs->inning >= fatigueStart) {
                    int fatiguePenalty = 30 - data->endurance;
                    if (fatiguePenalty > 0) {
                        hrChance -= fatiguePenalty / 2;
                        hitChance -= fatiguePenalty;
                        foulChance += fatiguePenalty / 2;
                        missChance += fatiguePenalty;
                        if (hrChance < 0) hrChance = 0;
                        if (hitChance < 0) hitChance = 0;
                    }
                }
                
                if (data->hitGauge >= 10) {
                    SetColor(13); printf("홈런!!");
                    gs->scoreOhtani += 1 + gs->base1 + gs->base2 + gs->base3;
                    gs->base1 = 0; gs->base2 = 0; gs->base3 = 0;
                    gs->balls = 0; gs->strikes = 0;
                    data->hitGauge = 0;
                }
    else if (r < hrChance) {
                    SetColor(13); printf("홈런!!");
                    gs->scoreOhtani += 1 + gs->base1 + gs->base2 + gs->base3;
                    gs->base1 = 0; gs->base2 = 0; gs->base3 = 0;
                    gs->balls = 0; gs->strikes = 0;
                    data->hitGauge++;
                }
    else if (r < hrChance + hitChance) {
                    int r2 = rand() % 100;
                    if (r2 < 10) {
                        SetColor(10); printf("안타(3루타)!");
                        gs->scoreOhtani += gs->base1 + gs->base2 + gs->base3;
                        gs->base1 = 0; gs->base2 = 0; gs->base3 = 1;
                    }
    else if (r2 < 40) {
                        SetColor(10); printf("안타(2루타)!");
                        gs->scoreOhtani += gs->base2 + gs->base3;
                        gs->base3 = gs->base1; gs->base2 = 1; gs->base1 = 0;
                    }
    else {
                        SetColor(10); printf("안타(1루타)!");
                        if (gs->base3) { gs->scoreOhtani++; gs->base3 = 0; }
                        if (gs->base2) { gs->base3 = 1; gs->base2 = 0; }
                        if (gs->base1) { gs->base2 = 1; }
                        gs->base1 = 1;
                    }
                    gs->balls = 0; gs->strikes = 0;
                    data->hitGauge++;
                }
    else if (r < hrChance + hitChance + foulChance) {
                    SetColor(14); printf("파울!");
                    if (gs->strikes < 2) gs->strikes++;
                }
    else {
                    SetColor(12); printf("헛스윙!");
                    gs->strikes++;
                }
            }
        }
    else {
            if (isBunt) {
                SetColor(12); printf("번트 헛스윙!");
                gs->strikes++;
            }
    else {
                SetColor(12); printf("헛스윙!");
                gs->strikes++;
            }
        }
    }
    else {
        if (rand() % 100 < 50) {
            SetColor(9); printf("볼!");
            gs->balls++;
            if (gs->balls >= 4) {
                GotoXY(msgX, msgY); printf("볼넷!");
                if (gs->base1 && gs->base2 && gs->base3) gs->scoreOhtani++;
                else if (gs->base1 && gs->base2) gs->base3 = 1;
                else if (gs->base1) gs->base2 = 1;
                gs->base1 = 1;
                gs->balls = 0; gs->strikes = 0;
            }
        }
    else {
            SetColor(12); printf("스트라이크!");
            gs->strikes++;
        }
    }
    
    if (gs->strikes >= 3) { 
        GotoXY(msgX, msgY); SetColor(12); printf("삼진 아웃!"); 
        gs->outs++; gs->strikes = 0; gs->balls = 0; 
    }
    Sleep(1500);
}

// 투수 로직
bool PlayPitching(SaveData* data, InGameState* gs) {
    int pitcherX = 38, pitcherY = 12; // 오타니
    int batterX = 38, batterY = 18;   // 적
    int msgX = batterX + 15, msgY = batterY + 2;
    int teamIdx = GetTeamIndex(data->matchIndex);
    
    int fatigueStart = (gs->weather == 2) ? 3 : 4;
    
    DrawPlayerModel(2, pitcherX, pitcherY, 10); 
    DrawPlayerModel(1, batterX, batterY, teamIdx); 
    
    GotoXY(pitcherX - 2, pitcherY - 1); SetColor(10); printf("오타니");
    if (gs->inning >= fatigueStart) { SetColor(12); printf(" (CC기: 피로도가 찼습니다.)"); }
    GotoXY(batterX - 2, batterY - 1); SetColor(11); printf("%s", KBO_Teams[teamIdx].batters[gs->currentBatterIndex % 11]);

    GotoXY(2, 23); SetColor(14); printf("투구 준비! (↑:직구, ←:커브, →:포크볼) 구질을 선택하세요.         ");
    int pitchType = 0; 
    while(1) {
        if (_kbhit()) {
            int k = _getch();
            if (k == 'm' || k == 'M') { gs->scoreOhtani += 100; gs->inning = 6; gs->outs = 3; return false; }
            if (k == 224) { 
                k = _getch();
                if (k == 72) { pitchType = 0; break; } 
                if (k == 75) { pitchType = 1; break; } 
                if (k == 77) { pitchType = 2; break; } 
            }
        }
        Sleep(50);
    }
    
    int zoneSize = (pitchType == 0) ? 6 : ((pitchType == 1) ? 4 : 2);
    int speed = (pitchType == 0) ? 20 : ((pitchType == 1) ? 40 : 70);
    speed -= data->matchIndex; 
    if (speed < 10) speed = 10;
    
    GotoXY(2, 23); printf("스페이스바를 눌러 게이지를 맞추세요!                               ");
        
    int cursorX = 0;
    int dir = 1;
    bool success = false;
    
    while(1) {
        GotoXY(2, 24); SetColor(15);
        printf("[");
        for(int i=0; i<20; i++) {
            if (i >= 10 - zoneSize/2 && i < 10 + zoneSize/2) SetColor(10);
            else SetColor(8);
            
            if (i == cursorX) printf("▼");
            else if (i >= 10 - zoneSize/2 && i < 10 + zoneSize/2) printf("=");
            else printf("-");
        }
        SetColor(15); printf("]");
        
        if (_kbhit()) {
            int k = _getch();
            if (k == 'm' || k == 'M') { gs->scoreOhtani += 100; gs->inning = 6; gs->outs = 3; return false; }
            if (k == 32) {
                if (cursorX >= 10 - zoneSize/2 && cursorX < 10 + zoneSize/2) {
                    success = true;
                }
                break;
            }
        }
        
        cursorX += dir;
        if (cursorX >= 20 || cursorX < 0) {
            dir *= -1;
            cursorX += dir * 2;
        }
        Sleep(speed);
    }
    
    GotoXY(2, 24); printf("                                            ");
    GotoXY(2, 23); printf("                                            ");
    
    int ballX = pitcherX, ballY = pitcherY;
    while(ballY < batterY) {
        GotoXY(ballX, ballY); printf("o");
        Sleep(pitchType == 0 ? 50 : (pitchType == 1 ? 80 : 120));
        GotoXY(ballX, ballY); printf(" ");
        ballY++;
        if (pitchType == 1) ballX += (rand()%3 - 1); 
        else if (pitchType == 2) { if (ballY > 16) ballY++; } 
    }
    
    int tier = getTier(data->agility);
    int diffBonus = data->matchIndex * 2;
    int strikeChance = 40 + tier - (diffBonus / 2);
    int foulChance = 10;
    int inPlayChance = 15 + diffBonus; 
    int ballChance = 30 - tier - (diffBonus / 2);
    
    if (gs->weather == 1) {
        ballChance += 20;
        strikeChance -= 10;
        inPlayChance -= 10;
    }
    
    if (success) {
        strikeChance += 20;
        inPlayChance -= 10;
        ballChance -= 10;
    }
    else {
        strikeChance -= 20;
        ballChance += 10;
        inPlayChance += 10;
    }
    
    int r = rand() % 100;
    GotoXY(msgX, msgY);
    
    if (r < strikeChance) {
        SetColor(10); printf("스트라이크!");
        gs->strikes++;
        if(gs->strikes >= 3) { gs->outs++; gs->strikes=0; gs->balls=0; gs->currentBatterIndex++; }
        Sleep(1500);
        return false;
    }
    else if (r < strikeChance + foulChance) {
        SetColor(14); printf("파울!");
        if(gs->strikes < 2) gs->strikes++;
        Sleep(1500);
        return false;
    }
    else if (r < strikeChance + foulChance + inPlayChance) {
        SetColor(12); printf("타격!");
        Sleep(500);
        return true; 
    }
    else if (r < strikeChance + foulChance + inPlayChance + ballChance) {
        SetColor(9); printf("볼!");
        gs->balls++;
        if (gs->balls >= 4) {
            GotoXY(msgX, msgY); SetColor(12); printf("볼넷!");
            if (gs->base1 && gs->base2 && gs->base3) gs->scoreOpponent++;
            else if (gs->base1 && gs->base2) gs->base3 = 1;
            else if (gs->base1) gs->base2 = 1;
            gs->base1 = 1;
            gs->balls = 0; gs->strikes = 0;
            gs->currentBatterIndex++;
        }
        Sleep(1500);
        return false;
    }
    else {
        SetColor(12); printf("데드볼!");
        if (gs->base1 && gs->base2 && gs->base3) gs->scoreOpponent++;
        else if (gs->base1 && gs->base2) gs->base3 = 1;
        else if (gs->base1) gs->base2 = 1;
        gs->base1 = 1;
        gs->balls = 0; gs->strikes = 0;
        gs->currentBatterIndex++;
        Sleep(1500);
        return false;
    }
}

// 적 안타 시 수비 로직
void PlayFielding(SaveData* data, InGameState* gs) {
    gs->fielderX = 38; gs->fielderY = 15;
    int ballX = 38, ballY = 21;
    int ballTargX = 20 + rand()%40, ballTargY = 5 + rand()%10;
    int teamIdx = GetTeamIndex(data->matchIndex);
    
    while(ballY > ballTargY) {
        DrawStadiumUI(data, gs);
        DrawPlayerModel(3, gs->fielderX, gs->fielderY, 10);
        GotoXY(gs->fielderX, gs->fielderY - 2); SetColor(10); printf("오타니");
        
        GotoXY(ballX, ballY); SetColor(15); printf("o");
        
        if (_kbhit()) {
            int k = _getch();
            if (k == 72) gs->fielderY--;
            if (k == 80) gs->fielderY++;
            if (k == 75) gs->fielderX--;
            if (k == 77) gs->fielderX++;
        }
        
        if (abs(ballX - gs->fielderX) <= 2 && abs(ballY - gs->fielderY) <= 2) {
            GotoXY(gs->fielderX + 15, gs->fielderY + 2); SetColor(10); printf("아웃!");
            gs->outs++;
            gs->currentBatterIndex++;
            Sleep(1500); return;
        }
        
        ballY--;
        if (ballX < ballTargX) ballX++; else if (ballX > ballTargX) ballX--;
        Sleep(100);
    }
    
    GotoXY(gs->fielderX + 15, gs->fielderY + 2); SetColor(12); printf("안타!");
    if (gs->base3) { gs->scoreOpponent++; gs->base3 = 0; }
    if (gs->base2) { gs->base3 = 1; gs->base2 = 0; }
    if (gs->base1) { gs->base2 = 1; }
    gs->base1 = 1;
    gs->balls = 0; gs->strikes = 0;
    gs->currentBatterIndex++;
    Sleep(1500);
}