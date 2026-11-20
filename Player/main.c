/* 1번 파트 단독 테스트용 main (최종 통합 시 팀 main으로 교체) */
#include <stdio.h>
#include <stdlib.h>
#include "character.h"

int main(void)
{
    Player player;
    int menu, ok;

#ifdef _WIN32
    system("chcp 65001 > nul");   /* 윈도우 콘솔 한글 깨짐 방지 (UTF-8) */
#endif

    create_character(&player);

    while (1) {
        printf("\n===== 마을 =====\n");
        printf("1. 캐릭터 정보\n");
        printf("2. 여관\n");
        printf("3. [테스트] 경험치 40 획득\n");
        printf("4. [테스트] HP 30 감소\n");
        printf("0. 종료\n");
        printf("선택: ");

        ok = scanf("%d", &menu);
        clear_input_buffer();
        if (ok != 1) {
            printf("숫자를 입력하세요.\n");
            continue;
        }

        switch (menu) {
        case 1: show_player_info(&player); break;
        case 2: visit_inn(&player);        break;
        case 3: add_exp(&player, 40);      break;
        case 4:
            player.hp -= 30;
            if (player.hp < 1) player.hp = 1;
            printf("HP가 감소했습니다. (%d / %d)\n", player.hp, player.max_hp);
            break;
        case 0:
            printf("게임을 종료합니다.\n");
            return 0;
        default:
            printf("잘못된 선택입니다.\n");
        }
    }
}
