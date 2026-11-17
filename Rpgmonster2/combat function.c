#include <stdio.h>
#include <stdlib.h>
#include "combat.h"

// 공격 데미지를 계산하는 함수
// 공격력에서 방어력을 빼고 최소 데미지는 1로 설정
int calculate_damage(int attack, int defense)
{
    int damage = attack - defense;

    if (damage < 1)
        damage = 1;

    return damage;
}

// 캐릭터와 몬스터의 현재 상태 출력
void show_status(const Player* p, const Monster* m)
{
    printf("\n========================================\n");
    printf("              전투 상태\n");
    printf("========================================\n");

    printf("캐릭터 : %s (레벨 %d)\n", p->name, p->level);
    printf("체력 : %d / %d\n", p->hp, p->maxHp);
    printf("공격력 : %d | 방어력 : %d\n",
        p->attack, p->defense);

    printf("----------------------------------------\n");

    printf("몬스터 : %s\n", m->name);
    printf("체력 : %d / %d\n", m->hp, m->maxHp);
    printf("공격력 : %d | 방어력 : %d\n",
        m->attack, m->defense);

    printf("========================================\n");
}

// 포션을 사용하여 체력을 회복하는 함수
// 사용 성공 시 1, 사용 불가능하면 0 반환
int use_potion(Player* p)
{
    if (p->potion <= 0) {
        printf("포션이 없습니다.\n");
        return 0;
    }

    if (p->hp >= p->maxHp) {
        printf("체력이 이미 가득 찼습니다.\n");
        return 0;
    }

    // 포션 1개 소비
    p->potion--;

    // 체력 40 회복
    p->hp += 40;

    // 최대 체력을 초과하지 않도록 제한
    if (p->hp > p->maxHp)
        p->hp = p->maxHp;

    printf("\n포션을 사용했습니다!\n");
    printf("현재 체력 : %d / %d\n", p->hp, p->maxHp);

    return 1;
}

// 몬스터 처치 시 경험치와 골드 지급
void give_reward(Player* p, const Monster* m)
{
    p->exp += m->exp;
    p->gold += m->gold;

    onMonsterKilled(p, m->name);  /* 몬스터 처치를 퀘스트에 알려 줌 (이게 있어야 처치 수가 올라감) */

    printf("\n========================================\n");
    printf("                승리!\n");
    printf("========================================\n");

    printf("%s 처치 성공!\n", m->name);
    printf("획득 경험치 : %d\n", m->exp);
    printf("획득 골드 : %d\n", m->gold);

    printf("----------------------------------------\n");
    printf("현재 경험치 : %d\n", p->exp);
    printf("현재 골드 : %d\n", p->gold);
    printf("========================================\n");
}

// 전투 행동 번호를 입력받는 함수
// 숫자가 아닌 값을 입력하면 -1 반환
int read_action(void)
{
    int choice;
    int ch;

    printf("선택 >> ");

    if ((ch = scanf("%d", &choice)) != 1) {
        if (ch == EOF)   // 입력이 끝나면(Ctrl+Z) 프로그램 종료
            exit(0);

        // 잘못 입력된 문자를 제거
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }

        return -1;
    }

    return choice;
}

// 실제 전투를 진행하는 함수
int fight(Player* p, Monster m)
{
    int choice;
    int damage;
    int chance;

    printf("\n========================================\n");
    printf("              몬스터 등장!\n");
    printf("========================================\n");
    printf("%s(이)가 나타났습니다!\n", m.name);
    printf("========================================\n");

    // 캐릭터와 몬스터가 모두 살아 있는 동안 반복
    while (p->hp > 0 && m.hp > 0) {

        show_status(p, &m);

        printf("\n-------------- 행동 선택 --------------\n");
        printf("1. 공격하기\n");
        printf("2. 포션 사용 (남은 개수 : %d)\n", p->potion);
        printf("3. 도망가기\n");
        printf("----------------------------------------\n");

        choice = read_action();

        // 1. 공격하기
        if (choice == 1) {

            damage = calculate_damage(
                p->attack, m.defense
            );

            m.hp -= damage;

            if (m.hp < 0)
                m.hp = 0;

            printf("\n%s의 공격!\n", p->name);
            printf("%s에게 %d의 피해를 주었습니다.\n",
                m.name, damage);

            // 몬스터 처치 여부 확인
            if (m.hp == 0) {
                give_reward(p, &m);
                return WIN;
            }
        }

        // 2. 포션 사용
        else if (choice == 2) {

            // 포션을 사용하지 못하면 턴을 소비하지 않음
            if (use_potion(p) == 0)
                continue;
        }

        // 3. 도망가기
        else if (choice == 3) {

            // 보스전에서는 도망 불가능
            if (m.boss == 1) {
                printf("\n보스전에서는 도망갈 수 없습니다!\n");
            }
            else {
                chance = rand() % 100;

                // 50% 확률로 도망 성공
                if (chance < 50) {
                    printf("\n도망에 성공했습니다!\n");
                    return RUN_AWAY;
                }
                else {
                    printf("\n도망에 실패했습니다!\n");
                }
            }
        }

        // 잘못된 번호 입력
        else {
            printf("\n1부터 3까지의 번호를 입력해주세요.\n");
            continue;
        }

        // 몬스터 반격
        damage = calculate_damage(
            m.attack, p->defense
        );

        p->hp -= damage;

        if (p->hp < 0)
            p->hp = 0;

        printf("\n%s의 반격!\n", m.name);
        printf("%d의 피해를 받았습니다.\n", damage);

        // 캐릭터 패배 판정
        if (p->hp == 0) {
            printf("\n========================================\n");
            printf("               전투 패배\n");
            printf("========================================\n");
            printf("%s이(가) 쓰러졌습니다.\n", p->name);
            printf("여관에서 체력을 회복하세요.\n");
            printf("========================================\n");

            return LOSE;
        }
    }

    return LOSE;
}