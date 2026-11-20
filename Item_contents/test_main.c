/*
 * test_main.c
 * 3번 기능만 따로 시험해 보기 위한 임시 main 파일입니다.
 * 팀 코드와 합칠 때는 이 파일을 빌드에서 빼야 합니다.
 *
 * 컴파일: gcc test_main.c item.c quest.c -o test
 * 실행  : ./test
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "game.h"

/* 1번 담당의 gainExp가 나오기 전까지 쓰는 임시 함수 (레벨업 없음) */
void gainExp(Player *player, int amount) {
    player->exp += amount;
}

int main(void) {
    Player player = {0};  /* 모든 값을 0으로 초기화 (인벤토리 0개, 장비 없음) */
    int choice;

    srand((unsigned int)time(NULL));  /* 랜덤 시작값 설정: 프로그램에서 1번만! */

    /* 테스트용 캐릭터 */
    strcpy(player.name, "테스트");
    player.level = 1;
    player.hp = 50;
    player.maxHp = 100;
    player.attack = 10;
    player.defense = 5;
    player.gold = 5000;
    player.potion = 0;
    initQuests(&player);

    while (1) {
        printf("\nHP %d/%d | 공격 %d | 방어 %d | %dG | 포션 %d개 | 경험치 %d\n",
               player.hp, player.maxHp, player.attack, player.defense,
               player.gold, player.potion, player.exp);
        printf("1 상점 2 인벤토리 3 장비 4 강화 5 퀘스트 6 슬라임 처치 0 종료: ");
        choice = readInt();

        switch (choice) {
        case 1: shopMenu(&player); break;
        case 2: inventoryMenu(&player); break;
        case 3: equipmentMenu(&player); break;
        case 4: enhanceMenu(&player); break;
        case 5: questMenu(&player); break;
        case 6: onMonsterKilled(&player, "슬라임"); break;  /* 전투 대신 가짜 처치 */
        case 0: return 0;
        default: printf("잘못된 입력입니다.\n"); break;
        }
    }
}
