#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"
#include "battle.h"

int main(void)
{
    // 임시 캐릭터 생성
    // 나중에 1번 담당자의 캐릭터 정보와 연결
    Player player = {
        "용사",
        1,       // 레벨
        100,     // 현재 체력
        100,     // 최대 체력
        20,      // 공격력
        5,       // 방어력
        0,       // 경험치
        100,     // 골드
        5        // 포션 개수
    };

    // 랜덤 몬스터 생성을 위한 난수 초기화
    srand((unsigned int)time(NULL));

    printf("\n========================================\n");
    printf("             텍스트 RPG 게임\n");
    printf("========================================\n");
    printf("모험의 세계에 오신 것을 환영합니다!\n");
    printf("========================================\n");

    // 사냥터 메뉴 실행
    battle_menu(&player);

    // 게임 종료 후 캐릭터 상태 확인
    printf("\n========================================\n");
    printf("              캐릭터 정보\n");
    printf("========================================\n");
    printf("이름 : %s\n", player.name);
    printf("레벨 : %d\n", player.level);
    printf("체력 : %d / %d\n", player.hp, player.maxHp);
    printf("경험치 : %d\n", player.exp);
    printf("골드 : %d\n", player.gold);
    printf("포션 : %d개\n", player.potion);
    printf("========================================\n");

    return 0;
}