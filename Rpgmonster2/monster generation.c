#include <stdlib.h>
#include <string.h>
#include "monster.h"

// 사냥터에 따라 몬스터를 생성하는 함수
// area : 사냥터 번호
// boss : 보스 여부
Monster create_monster(int area, int boss)
{
    Monster m = { 0 };
    int r = rand() % 3;

    m.boss = boss;

    // 1번 사냥터 : 초원
    if (area == 1) {
        if (boss) {
            strcpy(m.name, "거대 슬라임");
            m.hp = 100;
            m.attack = 15;
            m.defense = 5;
            m.exp = 100;
            m.gold = 150;
        }
        else if (r == 0) {
            strcpy(m.name, "슬라임");
            m.hp = 25;
            m.attack = 6;
            m.defense = 1;
            m.exp = 15;
            m.gold = 10;
        }
        else if (r == 1) {
            strcpy(m.name, "토끼");
            m.hp = 20;
            m.attack = 5;
            m.defense = 1;
            m.exp = 10;
            m.gold = 8;
        }
        else {
            strcpy(m.name, "늑대");
            m.hp = 40;
            m.attack = 10;
            m.defense = 3;
            m.exp = 25;
            m.gold = 20;
        }
    }

    // 2번 사냥터 : 어둠의 숲
    else if (area == 2) {
        if (boss) {
            strcpy(m.name, "숲의 수호자");
            m.hp = 180;
            m.attack = 25;
            m.defense = 10;
            m.exp = 250;
            m.gold = 300;
        }
        else if (r == 0) {
            strcpy(m.name, "고블린");
            m.hp = 60;
            m.attack = 14;
            m.defense = 4;
            m.exp = 40;
            m.gold = 35;
        }
        else if (r == 1) {
            strcpy(m.name, "오크");
            m.hp = 85;
            m.attack = 18;
            m.defense = 6;
            m.exp = 55;
            m.gold = 50;
        }
        else {
            strcpy(m.name, "해골 병사");
            m.hp = 70;
            m.attack = 16;
            m.defense = 5;
            m.exp = 45;
            m.gold = 40;
        }
    }

    // 3번 사냥터 : 마왕의 성
    else {
        if (boss) {
            strcpy(m.name, "마왕");
            m.hp = 350;
            m.attack = 40;
            m.defense = 15;
            m.exp = 700;
            m.gold = 1000;
        }
        else if (r == 0) {
            strcpy(m.name, "암흑 기사");
            m.hp = 120;
            m.attack = 25;
            m.defense = 10;
            m.exp = 90;
            m.gold = 100;
        }
        else if (r == 1) {
            strcpy(m.name, "악마");
            m.hp = 140;
            m.attack = 30;
            m.defense = 8;
            m.exp = 110;
            m.gold = 130;
        }
        else {
            strcpy(m.name, "드래곤");
            m.hp = 170;
            m.attack = 35;
            m.defense = 12;
            m.exp = 150;
            m.gold = 180;
        }
    }

    // 생성된 몬스터의 최대 체력 설정
    m.maxHp = m.hp;

    return m;
}