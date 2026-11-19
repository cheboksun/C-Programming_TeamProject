#ifndef COMBAT_H
#define COMBAT_H

#include "character.h"
#include "monster.h"

// 전투 결과를 숫자로 구분
#define LOSE 0
#define WIN 1
#define RUN_AWAY 2

// 공격 데미지 계산
int calculate_damage(int attack, int defense);

// 전투 상태 출력
void show_status(const Player* p, const Monster* m);

// 포션 사용
int use_potion(Player* p);

// 몬스터 처치 보상 지급
void give_reward(Player* p, const Monster* m);

// 전투 진행
int fight(Player* p, Monster m);

#endif