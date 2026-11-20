/*
 * item.h
 * 3번 담당(아이템·콘텐츠)의 "타입 설명서" 파일입니다.
 * - 상수(#define): 게임에서 쓰는 고정된 숫자
 * - 구조체(struct): Item, Inventory, Equipment, Quest
 *
 * ※ Player 구조체는 여기 없습니다. 1번의 character.h에 있는 Player가 팀 표준입니다.
 *    character.h가 이 파일을 include해서 Player 안에 Inventory, Equipment, Quest를 넣습니다.
 *    (그래서 이 파일은 Player를 쓰면 안 됩니다. 쓰면 헤더끼리 서로를 include하는 순환이 생깁니다.)
 * ※ 3번 함수 선언은 content.h에 있습니다.
 */

/* 같은 헤더가 두 번 포함되어도 문제가 없도록 막아 주는 장치 */
#ifndef ITEM_H
#define ITEM_H

/* ===== 상수 ===== */
#define MAX_INVENTORY 20     /* 인벤토리 최대 칸 수 (장비만 들어감) */
#define MAX_QUEST 3          /* 퀘스트 개수 */
#define MAX_ENHANCE 5        /* 최대 강화 단계 (+5) */
#define SHOP_ITEM_COUNT 7    /* 상점에서 파는 아이템 종류 수 */
#define POTION_HEAL 40       /* 포션 1개 회복량 (2번 전투 코드의 use_potion과 같은 값) */

/* 아이템 종류 */
#define ITEM_WEAPON 0        /* 무기 */
#define ITEM_ARMOR 1         /* 방어구 */
#define ITEM_POTION 2        /* 포션 (상점 목록에서만 사용, 인벤토리에는 들어가지 않음) */

/* 퀘스트 상태 */
#define QUEST_NOT_ACCEPTED 0 /* 아직 수락 안 함 */
#define QUEST_IN_PROGRESS 1  /* 진행 중 */
#define QUEST_COMPLETED 2    /* 목표 달성 (보상 대기) */
#define QUEST_REWARDED 3     /* 보상까지 받음 */

/* ===== 구조체 ===== */

/* 아이템 1개 */
typedef struct {
    char name[30];  /* 이름 */
    int type;       /* ITEM_WEAPON, ITEM_ARMOR, ITEM_POTION 중 하나 */
    int price;      /* 구매 가격 */
    int value;      /* 무기=공격력, 방어구=방어력, 포션=HP 회복량 */
    int enhance;    /* 강화 단계 (포션은 항상 0) */
} Item;

/* 인벤토리 = 장비 배열 + 현재 몇 개 들어 있는지 */
typedef struct {
    Item items[MAX_INVENTORY];
    int count;      /* 현재 보유 개수 (0 ~ MAX_INVENTORY) */
} Inventory;

/* 장착 중인 장비 */
typedef struct {
    Item weapon;    /* 장착한 무기 */
    Item armor;     /* 장착한 방어구 */
    int hasWeapon;  /* 1이면 무기 장착 중, 0이면 없음 */
    int hasArmor;   /* 1이면 방어구 장착 중, 0이면 없음 */
} Equipment;

/* 퀘스트 1개 */
typedef struct {
    char name[50];    /* 퀘스트 이름 */
    char target[30];  /* 처치해야 할 몬스터 이름 */
    int goal;         /* 목표 처치 수 */
    int progress;     /* 현재 처치 수 */
    int rewardGold;   /* 보상 골드 */
    int rewardExp;    /* 보상 경험치 */
    int state;        /* QUEST_NOT_ACCEPTED ~ QUEST_REWARDED */
} Quest;

#endif
