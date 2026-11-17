#ifndef GAME_H
#define GAME_H

// 캐릭터의 기본 정보를 저장하는 구조체
// 캐릭터 생성 및 레벨업 기능은 유림(1번) 구현

typedef struct {
    char name[30];   // 캐릭터 이름
    int level;       // 레벨
    int hp;          // 현재 체력
    int maxHp;       // 최대 체력
    int attack;      // 공격력
    int defense;     // 방어력
    int exp;         // 경험치
    int gold;        // 골드
    int potion;      // 포션 개수
} Player;

#endif