#ifndef MONSTER_H
#define MONSTER_H

// 몬스터의 능력치와 처치 보상을 저장하는 구조체
typedef struct {
    char name[30];   // 몬스터 이름
    int hp;          // 현재 체력
    int maxHp;       // 최대 체력
    int attack;      // 공격력
    int defense;     // 방어력
    int exp;         // 처치 경험치
    int gold;        // 처치 골드
    int boss;        // 보스 여부 (1: 보스, 0: 일반)
} Monster;

// 사냥터와 보스 여부에 따라 몬스터 생성
Monster create_monster(int area, int boss);

#endif