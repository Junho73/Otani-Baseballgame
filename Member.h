#ifndef MEMBER_H
#define MEMBER_H

// KBO 소속 팀 구조체 (구단 이름, 홈구장, 투수 2명, 타자 11명)
typedef struct {
	char teamName[30];          // 팀 이름
	char stadiumName[30];       // 홈구장 이름
	char pitchers[2][30];       // 투수 명단 (선발, 불펜 등)
	char batters[11][30];       // 타자 명단 (1번 타자부터 백업까지)
	int offensePower;           // 팀 고유 공격력 수치
	int defensePower;           // 팀 고유 수비력 수치
} Team;

extern Team KBO_Teams[11];      // 10개 KBO 구단 + Team 코리아 배열
#endif