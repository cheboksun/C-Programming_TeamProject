#include <stdio.h>
#include <string.h>
#include "character.h"

void clear_input_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

const char *get_job_name(int job)
{
    switch (job) {
    case JOB_WARRIOR: return "전사";
    case JOB_MAGE:    return "마법사";
    default:          return "알 수 없음";
    }
}

/* 직업별 초기 능력치 설정 */
static void set_base_stats(Player *p)
{
    switch (p->job) {
    case JOB_WARRIOR:  /* 체력/방어형 */
        p->max_hp = 120; p->atk = 12; p->def = 8;
        break;
    case JOB_MAGE:     /* 공격형, 체력/방어 낮음 */
        p->max_hp = 80;  p->atk = 18; p->def = 3;
        break;
    }
    p->hp = p->max_hp;
}

void create_character(Player *p)
{
    int len, choice, ok;

    printf("\n===== 캐릭터 생성 =====\n");

    /* 이름 입력 */
    while (1) {
        printf("이름을 입력하세요 (최대 %d자): ", NAME_LEN - 1);
        if (fgets(p->name, NAME_LEN, stdin) == NULL)
            continue;

        len = (int)strlen(p->name);
        if (len > 0 && p->name[len - 1] == '\n')
            p->name[len - 1] = '\0';
        else
            clear_input_buffer();   /* 너무 길게 입력한 경우 남은 입력 제거 */

        if (strlen(p->name) == 0) {
            printf("이름은 비워둘 수 없습니다.\n");
            continue;
        }
        break;
    }

    /* 직업 선택 */
    while (1) {
        printf("\n직업을 선택하세요.\n");
        printf("1. 전사   (HP 120 / 공격 12 / 방어 8)\n");
        printf("2. 마법사 (HP  80 / 공격 18 / 방어 3)\n");
        printf("선택: ");

        ok = scanf("%d", &choice);
        clear_input_buffer();

        if (ok == 1 && choice >= JOB_WARRIOR && choice <= JOB_MAGE) {
            p->job = choice;
            break;
        }
        printf("잘못된 입력입니다. 1~2 중에서 선택하세요.\n");
    }

    /* 공통 초기값 */
    p->level       = 1;
    p->exp         = 0;
    p->exp_to_next = p->level * 50;
    p->gold        = 100;
    p->potion      = 3;
    set_base_stats(p);

    printf("\n캐릭터가 생성되었습니다!\n");
    show_player_info(p);
}

void show_player_info(const Player *p)
{
    printf("\n---------- 캐릭터 정보 ----------\n");
    printf(" 이름   : %s\n", p->name);
    printf(" 직업   : %s\n", get_job_name(p->job));
    printf(" 레벨   : %d\n", p->level);
    printf(" 경험치 : %d / %d\n", p->exp, p->exp_to_next);
    printf(" HP     : %d / %d\n", p->hp, p->max_hp);
    printf(" 공격력 : %d\n", p->atk);
    printf(" 방어력 : %d\n", p->def);
    printf(" 골드   : %d G\n", p->gold);
    printf(" 포션   : %d 개\n", p->potion);
    printf("---------------------------------\n");
}

void level_up(Player *p)
{
    int hp_up, atk_up, def_up;

    /* 직업별 성장치 */
    switch (p->job) {
    case JOB_WARRIOR: hp_up = 20; atk_up = 3; def_up = 3; break;
    case JOB_MAGE:    hp_up = 10; atk_up = 5; def_up = 1; break;
    default:          hp_up = 10; atk_up = 2; def_up = 1; break;
    }

    p->level++;
    p->max_hp += hp_up;
    p->atk    += atk_up;
    p->def    += def_up;
    p->hp      = p->max_hp;               /* 레벨업 시 HP 전체 회복 */
    p->exp_to_next = p->level * 50;       /* 레벨이 오를수록 필요 경험치 증가 */

    printf("\n★ 레벨 업! Lv.%d 이(가) 되었습니다! ★\n", p->level);
    printf("  최대 HP +%d, 공격력 +%d, 방어력 +%d (HP 전체 회복)\n",
           hp_up, atk_up, def_up);
}

void add_exp(Player *p, int amount)
{
    if (amount <= 0)
        return;

    p->exp += amount;
    printf("경험치 %d 획득!\n", amount);

    /* 경험치가 많으면 연속 레벨업 */
    while (p->exp >= p->exp_to_next) {
        p->exp -= p->exp_to_next;
        level_up(p);
    }
}

void visit_inn(Player *p)
{
    printf("\n===== 여관 =====\n");

    if (p->hp >= p->max_hp) {
        printf("이미 체력이 가득 차 있습니다.\n");
        return;
    }
    if (p->gold < INN_COST) {
        printf("골드가 부족합니다. (필요: %d G / 보유: %d G)\n",
               INN_COST, p->gold);
        return;
    }

    p->gold -= INN_COST;
    p->hp = p->max_hp;
    printf("푹 쉬었습니다. HP가 모두 회복되었습니다! (-%d G)\n", INN_COST);
    printf("현재 HP: %d / %d, 남은 골드: %d G\n", p->hp, p->max_hp, p->gold);
}
