#include <stdio.h>
#include "battle.h"
#include "monster.h"
#include "combat.h"

// 사냥터를 선택하고 전투를 시작하는 함수
void battle_menu(Player* p)
{
    int area;
    int choice;
    int result;
    int ch;

    Monster m;

    while (1) {

        printf("\n========================================\n");
        printf("                사냥터\n");
        printf("========================================\n");
        printf("1. 초원       (초급)\n");
        printf("2. 어둠의 숲  (중급)\n");
        printf("3. 마왕의 성  (상급)\n");
        printf("0. 마을로 돌아가기\n");
        printf("========================================\n");
        printf("사냥터 선택 >> ");

        if ((ch = scanf("%d", &area)) != 1) {
            if (ch == EOF)   // 입력이 끝나면(Ctrl+Z) 종료
                return;

            while ((ch = getchar()) != '\n' && ch != EOF) {
            }

            printf("숫자를 입력해주세요.\n");
            continue;
        }

        // 마을로 돌아가기
        if (area == 0)
            return;

        // 잘못된 사냥터 번호
        if (area < 1 || area > 3) {
            printf("존재하지 않는 사냥터입니다.\n");
            continue;
        }

        printf("\n----------------------------------------\n");
        printf("1. 일반 몬스터 사냥\n");
        printf("2. 보스 몬스터 도전\n");
        printf("0. 사냥터 다시 선택\n");
        printf("----------------------------------------\n");
        printf("전투 선택 >> ");

        if ((ch = scanf("%d", &choice)) != 1) {
            if (ch == EOF)
                return;

            while ((ch = getchar()) != '\n' && ch != EOF) {
            }

            printf("숫자를 입력해주세요.\n");
            continue;
        }

        if (choice == 0)
            continue;

        if (choice != 1 && choice != 2) {
            printf("잘못된 번호입니다.\n");
            continue;
        }

        // 선택한 사냥터의 몬스터 생성
        // choice가 2이면 보스 몬스터 생성
        m = create_monster(area, choice == 2);

        // 전투 시작
        result = fight(p, m);

        // 전투 결과 확인
        if (result == WIN) {
            printf("\n사냥을 완료했습니다!\n");
        }
        else if (result == RUN_AWAY) {
            printf("\n사냥터에서 빠져나왔습니다.\n");
        }
        // 패배(HP 0)는 fight() 안에서 게임 오버 처리 후 종료됨
    }
}