/*
 * quest.c
 * 3번 담당: 퀘스트
 */
#include <stdio.h>
#include <string.h>  /* strcmp: 문자열 비교 */
#include "content.h"

/* 처음 시작할 때의 퀘스트 목록
 * {이름, 처치할 몬스터, 목표 수, 현재 수, 보상 골드, 보상 경험치, 상태}
 * ※ 몬스터 이름은 2번 담당의 create_monster에 있는 이름과 글자까지 같아야 함
 *    (슬라임: 초원, 고블린: 어둠의 숲, 드래곤: 마왕의 성)
 * ※ 보상은 몬스터 1마리 골드(슬라임 10G, 고블린 35G, 드래곤 180G)에 맞춰 정함 */
static const Quest INITIAL_QUESTS[MAX_QUEST] = {
    {"슬라임 퇴치", "슬라임", 5,  0, 150, 50,  QUEST_NOT_ACCEPTED},
    {"고블린 토벌", "고블린", 10, 0, 500, 200, QUEST_NOT_ACCEPTED},
    {"드래곤 처치", "드래곤", 1,  0, 800, 300, QUEST_NOT_ACCEPTED}
};

/* 퀘스트 상태 번호를 글자로 바꿔 주는 도우미 함수 */
static const char *getStateName(int state) {
    switch (state) {
    case QUEST_NOT_ACCEPTED: return "미수락";
    case QUEST_IN_PROGRESS:  return "진행 중";
    case QUEST_COMPLETED:    return "완료 (보상 대기)";
    case QUEST_REWARDED:     return "보상 받음";
    default:                 return "알 수 없음";
    }
}

/* 캐릭터 생성 때 1번만 호출: 퀘스트 목록을 플레이어에게 복사 */
void initQuests(Player *player) {
    int i;

    for (i = 0; i < MAX_QUEST; i++) {
        player->quests[i] = INITIAL_QUESTS[i];  /* 구조체 통째로 복사 */
    }
}

/* 몬스터를 잡을 때마다 2번 담당(전투)이 호출하는 함수
 * 진행 중인 퀘스트 중 대상 몬스터 이름이 같은 것의 처치 수를 1 올린다. */
void onMonsterKilled(Player *player, const char *monsterName) {
    int i;
    Quest *quest;  /* 진짜 퀘스트를 바꿔야 하므로 포인터 사용 */

    for (i = 0; i < MAX_QUEST; i++) {
        quest = &player->quests[i];

        /* 진행 중이 아니면 건너뛴다 (미수락/완료/보상 받음은 오르지 않음) */
        if (quest->state != QUEST_IN_PROGRESS) {
            continue;
        }

        /* 문자열은 == 가 아니라 strcmp로 비교한다 (같으면 0) */
        if (strcmp(quest->target, monsterName) == 0) {
            quest->progress++;
            printf("[퀘스트] %s: %d/%d\n", quest->name, quest->progress, quest->goal);

            /* 목표에 도달하면 완료 상태로 바꾼다 */
            if (quest->progress >= quest->goal) {
                quest->state = QUEST_COMPLETED;
                printf("[퀘스트] %s 완료! 퀘스트 메뉴에서 보상을 받으세요.\n", quest->name);
            }
        }
    }
}

/* 퀘스트 목록 출력 (화면 번호는 1부터) */
static void showQuests(const Player *player) {
    int i;
    const Quest *quest;

    printf("\n===== 퀘스트 목록 =====\n");
    for (i = 0; i < MAX_QUEST; i++) {
        quest = &player->quests[i];
        printf("%d. %s - %s %d마리 처치 / 보상 %dG, 경험치 %d [%s]\n",
               i + 1, quest->name, quest->target, quest->goal,
               quest->rewardGold, quest->rewardExp, getStateName(quest->state));
    }
}

/* 수락하거나 완료한 퀘스트의 진행 상황 출력 */
static void showProgress(const Player *player) {
    int i;
    int shown = 0;  /* 출력한 퀘스트 수 */
    const Quest *quest;

    printf("\n===== 진행 상황 =====\n");
    for (i = 0; i < MAX_QUEST; i++) {
        quest = &player->quests[i];
        if (quest->state == QUEST_NOT_ACCEPTED) {
            continue;  /* 수락 안 한 퀘스트는 보여 주지 않음 */
        }
        printf("%s: %s %d/%d [%s]\n", quest->name, quest->target,
               quest->progress, quest->goal, getStateName(quest->state));
        shown++;
    }

    if (shown == 0) {
        printf("수락한 퀘스트가 없습니다.\n");
    }
}

/* 퀘스트 수락 */
static void acceptQuest(Player *player) {
    int number;
    Quest *quest;

    showQuests(player);
    printf("수락할 번호 (0: 취소): ");
    number = readInt();
    if (number == 0) {
        return;
    }
    if (number < 1 || number > MAX_QUEST) {
        printf("잘못된 번호입니다.\n");
        return;
    }

    quest = &player->quests[number - 1];
    if (quest->state != QUEST_NOT_ACCEPTED) {
        printf("이미 수락한 퀘스트입니다.\n");
        return;
    }

    quest->state = QUEST_IN_PROGRESS;
    printf("'%s' 퀘스트를 수락했습니다.\n", quest->name);
}

/* 보상 받기: 완료 상태인 퀘스트만 1번 받을 수 있다 */
static void claimReward(Player *player) {
    int number;
    Quest *quest;

    showQuests(player);
    printf("보상 받을 번호 (0: 취소): ");
    number = readInt();
    if (number == 0) {
        return;
    }
    if (number < 1 || number > MAX_QUEST) {
        printf("잘못된 번호입니다.\n");
        return;
    }

    quest = &player->quests[number - 1];

    /* 1) 완료 상태가 아니면 취소 (진행 중이거나 이미 받은 경우) */
    if (quest->state != QUEST_COMPLETED) {
        if (quest->state == QUEST_REWARDED) {
            printf("이미 보상을 받았습니다.\n");
        } else {
            printf("아직 완료하지 않은 퀘스트입니다.\n");
        }
        return;
    }

    /* 2) 골드 지급 */
    player->gold = player->gold + quest->rewardGold;
    printf("보상: %dG, 경험치 %d 획득!\n", quest->rewardGold, quest->rewardExp);

    /* 3) 경험치 지급 (레벨업은 1번 담당의 add_exp가 처리) */
    add_exp(player, quest->rewardExp);

    /* 4) 다시 받지 못하도록 상태 변경 */
    quest->state = QUEST_REWARDED;
}

/* 퀘스트 메뉴: 1 목록, 2 수락, 3 진행 상황, 4 보상 받기, 0 뒤로 */
void questMenu(Player *player) {
    int choice;

    while (1) {
        printf("\n===== 퀘스트 =====\n");
        printf("1. 목록 확인  2. 수락  3. 진행 상황 확인  4. 보상 받기  0. 뒤로\n선택: ");
        choice = readInt();

        switch (choice) {
        case 1:
            showQuests(player);
            break;
        case 2:
            acceptQuest(player);
            break;
        case 3:
            showProgress(player);
            break;
        case 4:
            claimReward(player);
            break;
        case 0:
            return;
        default:
            printf("잘못된 입력입니다.\n");
            break;
        }
    }
}
