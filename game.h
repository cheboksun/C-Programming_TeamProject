/*
 * game.h
 * 여러 .c 파일이 함께 쓰는 "공용 설명서" 파일입니다.
 * - 상수(#define): 게임에서 쓰는 고정된 숫자
 * - 구조체(struct): 여러 값을 하나로 묶은 자료형
 * - 함수 선언: "이런 함수가 어딘가에 있다"라고 알려 주는 부분
 */

/* 같은 헤더가 두 번 포함되어도 문제가 없도록 막아 주는 장치 */
#ifndef GAME_H
#define GAME_H

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

/* 플레이어
 * 앞의 9개(name ~ potion)는 2번 전투 코드의 Player와 순서까지 똑같다.
 * (그래서 2번 main.c의 {"용사", 1, 100, ...} 초기화가 그대로 동작함)
 * 뒤의 3개(inventory, equipment, quests)는 3번이 추가한 것이다. */
typedef struct {
    char name[30];    /* 캐릭터 이름 */
    int level;        /* 레벨 */
    int hp;           /* 현재 체력 */
    int maxHp;        /* 최대 체력 */
    int attack;       /* 공격력 (장비와 강화 보너스까지 포함된 값) */
    int defense;      /* 방어력 (장비와 강화 보너스까지 포함된 값) */
    int exp;          /* 경험치 */
    int gold;         /* 골드 */
    int potion;       /* 포션 개수 */
    Inventory inventory;
    Equipment equipment;
    Quest quests[MAX_QUEST];
} Player;

/* ===== 함수 선언 ===== */

/* 공용 입력 (item.c) */
int readInt(void);

/* 인벤토리 (item.c) */
int addItem(Inventory *inventory, Item item);
void removeItem(Inventory *inventory, int index);
void showInventory(const Player *player);
int usePotion(Player *player);
void inventoryMenu(Player *player);

/* 상점 (item.c) */
void shopMenu(Player *player);

/* 장비 (item.c) */
int equipItem(Player *player, int index);
void showEquipment(const Player *player);
void equipmentMenu(Player *player);

/* 강화 (item.c) */
void enhanceMenu(Player *player);

/* 퀘스트 (quest.c) */
void initQuests(Player *player);
void onMonsterKilled(Player *player, const char *monsterName);
void questMenu(Player *player);

/* 1번 담당이 구현하는 함수 (경험치 획득 + 레벨업) */
void gainExp(Player *player, int amount);

#endif
