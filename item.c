/*
 * item.c
 * 3번 담당: 입력, 인벤토리, 상점, 장비, 강화
 *
 * 2번 전투 코드와 맞추기 위한 규칙
 * - 포션은 인벤토리 칸이 아니라 player->potion(개수)에 저장한다.
 * - 장비를 장착하거나 강화하면 player->attack, player->defense를 바로 바꾼다.
 *   (그래서 전투 코드가 p->attack을 그대로 써도 장비가 반영된다)
 */
#include <stdio.h>   /* printf, scanf, getchar */
#include <stdlib.h>  /* exit, rand */
#include "game.h"

/* ================================================================
 * 0. 게임 데이터
 * static: 이 파일(item.c) 안에서만 쓰는 변수라는 뜻
 * const : 값을 바꾸지 않는 상수라는 뜻
 * ================================================================ */

/* 상점에서 파는 아이템 목록 {이름, 종류, 가격, 효과, 강화단계} */
static const Item SHOP_ITEMS[SHOP_ITEM_COUNT] = {
    {"나무 검",   ITEM_WEAPON, 300,  5,  0},
    {"철 검",     ITEM_WEAPON, 800,  12, 0},
    {"강철 검",   ITEM_WEAPON, 2000, 25, 0},
    {"가죽 갑옷", ITEM_ARMOR,  300,  3,  0},
    {"철 갑옷",   ITEM_ARMOR,  800,  8,  0},
    {"강철 갑옷", ITEM_ARMOR,  2000, 15, 0},
    {"포션",      ITEM_POTION, 50,   POTION_HEAL, 0}
};

/* 강화 성공 확률(%)
 * 인덱스 = 현재 강화 단계
 * 예) +0 -> +1 시도는 ENHANCE_RATE[0] = 100%
 *     +4 -> +5 시도는 ENHANCE_RATE[4] = 50% */
static const int ENHANCE_RATE[MAX_ENHANCE] = {100, 100, 70, 70, 50};

/* 강화 1단계마다 늘어나는 능력치 */
#define WEAPON_ENHANCE_BONUS 3  /* 무기: 공격력 +3 */
#define ARMOR_ENHANCE_BONUS 2   /* 방어구: 방어력 +2 */

/* 아이템 종류 번호를 글자로 바꿔 주는 작은 도우미 함수 */
static const char *getTypeName(int type) {
    if (type == ITEM_WEAPON) {
        return "무기";
    } else if (type == ITEM_ARMOR) {
        return "방어구";
    } else {
        return "포션";
    }
}

/* 장비 1개가 올려 주는 능력치 = 기본 효과 + 강화 보너스
 * 예) 나무 검(5) +2강 -> 5 + 2 * 3 = 11 */
static int getItemBonus(const Item *item) {
    if (item->type == ITEM_WEAPON) {
        return item->value + item->enhance * WEAPON_ENHANCE_BONUS;
    } else {
        return item->value + item->enhance * ARMOR_ENHANCE_BONUS;
    }
}

/* ================================================================
 * 1. 입력
 * ================================================================ */

/* 정수 하나를 입력받아 돌려준다.
 * 숫자가 아닌 글자를 입력하면 다시 입력받는다. */
int readInt(void) {
    int value;   /* 입력받은 숫자 */
    int result;  /* scanf가 성공적으로 읽은 개수 (성공하면 1) */
    int c;       /* 남은 글자를 버릴 때 쓰는 변수 */

    while (1) {
        result = scanf("%d", &value);

        if (result == 1) {
            break;              /* 숫자를 잘 읽었으면 반복 종료 */
        }
        if (result == EOF) {
            exit(0);            /* 입력이 끝났으면(Ctrl+D 등) 프로그램 종료 */
        }

        /* 숫자가 아니면: 그 줄에 남은 글자를 모두 버린다 */
        c = getchar();
        while (c != '\n' && c != EOF) {
            c = getchar();
        }
        printf("숫자를 입력하세요: ");
    }

    /* 숫자 뒤에 남은 글자(엔터 포함)도 버린다 */
    c = getchar();
    while (c != '\n' && c != EOF) {
        c = getchar();
    }

    return value;
}

/* ================================================================
 * 2. 인벤토리 (장비 보관) + 포션
 * ================================================================ */

/* 인벤토리 마지막 칸에 아이템을 넣는다.
 * 성공하면 1, 가득 차서 못 넣으면 0을 돌려준다. */
int addItem(Inventory *inventory, Item item) {
    if (inventory->count >= MAX_INVENTORY) {
        return 0;  /* 가득 참 */
    }

    inventory->items[inventory->count] = item;  /* 빈 칸에 저장 */
    inventory->count++;                         /* 개수 1 증가 */
    return 1;
}

/* index 번째 아이템을 지운다.
 * 뒤에 있는 아이템을 한 칸씩 앞으로 당겨서 빈칸이 생기지 않게 한다.
 * 예) [A][B][C][D] 에서 B(1번)를 지우면 -> [A][C][D] */
void removeItem(Inventory *inventory, int index) {
    int i;

    /* 잘못된 번호면 아무것도 하지 않는다 */
    if (index < 0 || index >= inventory->count) {
        return;
    }

    /* i < count - 1 까지만 돈다. (i + 1 이 범위를 넘지 않도록) */
    for (i = index; i < inventory->count - 1; i++) {
        inventory->items[i] = inventory->items[i + 1];
    }
    inventory->count--;
}

/* 인벤토리 목록(장비)과 포션 개수를 출력한다. 화면 번호는 1부터 시작한다. */
void showInventory(const Player *player) {
    int i;
    const Item *item;  /* 지금 출력할 아이템을 가리키는 포인터 */

    printf("\n===== 인벤토리 (%d/%d) =====\n", player->inventory.count, MAX_INVENTORY);
    printf("포션: %d개 (1개당 HP %d 회복)\n", player->potion, POTION_HEAL);

    if (player->inventory.count == 0) {
        printf("보관 중인 장비가 없습니다.\n");
        return;
    }

    for (i = 0; i < player->inventory.count; i++) {
        item = &player->inventory.items[i];

        if (item->type == ITEM_WEAPON) {
            printf("%d. %s [무기] 공격력 +%d (강화 +%d)\n",
                   i + 1, item->name, item->value, item->enhance);
        } else {
            printf("%d. %s [방어구] 방어력 +%d (강화 +%d)\n",
                   i + 1, item->name, item->value, item->enhance);
        }
    }
}

/* 포션 1개를 사용한다. (마을에서 쓰는 용도)
 * 성공하면 1, 실패하면 0을 돌려준다.
 * 규칙은 2번 전투 코드의 use_potion과 같다. (HP 40 회복, 최대 HP를 넘지 않음) */
int usePotion(Player *player) {
    int before;  /* 회복 전 HP */

    /* 1) 포션이 있는지 검사 */
    if (player->potion <= 0) {
        printf("포션이 없습니다.\n");
        return 0;
    }

    /* 2) HP가 이미 가득 찼으면 포션을 아낀다 */
    if (player->hp >= player->maxHp) {
        printf("HP가 이미 가득 찼습니다.\n");
        return 0;
    }

    /* 3) 포션 1개 소비 후 회복 */
    before = player->hp;
    player->potion--;
    player->hp = player->hp + POTION_HEAL;

    /* 4) 최대 HP를 넘으면 최대 HP로 맞춘다 */
    if (player->hp > player->maxHp) {
        player->hp = player->maxHp;
    }

    printf("HP가 %d 회복되었습니다. (%d -> %d, 남은 포션 %d개)\n",
           player->hp - before, before, player->hp, player->potion);
    return 1;
}

/* 인벤토리 메뉴: 1 포션 사용, 2 버리기, 0 뒤로 */
void inventoryMenu(Player *player) {
    int choice;
    int number;

    while (1) {
        showInventory(player);
        printf("\nHP %d/%d\n", player->hp, player->maxHp);
        printf("1. 포션 사용  2. 장비 버리기  0. 뒤로\n선택: ");
        choice = readInt();

        switch (choice) {
        case 1:
            usePotion(player);
            break;

        case 2:
            printf("버릴 장비 번호 (0: 취소): ");
            number = readInt();
            if (number == 0) {
                break;
            }
            if (number < 1 || number > player->inventory.count) {
                printf("잘못된 번호입니다.\n");
                break;
            }
            printf("%s을(를) 버렸습니다.\n", player->inventory.items[number - 1].name);
            removeItem(&player->inventory, number - 1);  /* 화면 번호 - 1 = 인덱스 */
            break;

        case 0:
            return;  /* 함수를 끝내서 이전 메뉴로 돌아간다 */

        default:
            printf("잘못된 입력입니다.\n");
            break;
        }
    }
}

/* ================================================================
 * 3. 상점
 * ================================================================ */

/* 상점 목록 출력 */
static void showShopItems(void) {
    int i;

    printf("\n===== 상점 목록 =====\n");
    for (i = 0; i < SHOP_ITEM_COUNT; i++) {
        printf("%d. %s [%s] 효과 %d / 가격 %dG\n",
               i + 1, SHOP_ITEMS[i].name, getTypeName(SHOP_ITEMS[i].type),
               SHOP_ITEMS[i].value, SHOP_ITEMS[i].price);
    }
}

/* 구매: 모든 검사를 통과한 뒤에만 골드를 깎는다 */
static void buyItem(Player *player) {
    int number;
    int index;
    Item item;  /* 상점 아이템을 복사해 둘 변수 (원본 SHOP_ITEMS는 바꾸지 않음) */

    showShopItems();
    printf("보유 골드: %dG / 포션 %d개\n", player->gold, player->potion);
    printf("구매할 번호 (0: 취소): ");
    number = readInt();

    if (number == 0) {
        return;
    }

    /* 1) 번호 검사 */
    index = number - 1;
    if (index < 0 || index >= SHOP_ITEM_COUNT) {
        printf("잘못된 번호입니다.\n");
        return;
    }

    item = SHOP_ITEMS[index];  /* 구조체 복사 */

    /* 2) 골드 검사 */
    if (player->gold < item.price) {
        printf("골드가 부족합니다. (필요 %dG, 보유 %dG)\n", item.price, player->gold);
        return;
    }

    /* 3-1) 포션이면 개수만 1 늘린다 (인벤토리 칸을 쓰지 않음) */
    if (item.type == ITEM_POTION) {
        player->potion++;
    }
    /* 3-2) 장비면 인벤토리에 넣는다 (넣기에 성공해야 다음으로 진행) */
    else if (addItem(&player->inventory, item) == 0) {
        printf("인벤토리가 가득 찼습니다.\n");
        return;
    }

    /* 4) 여기까지 왔으면 성공 -> 골드 차감 */
    player->gold = player->gold - item.price;
    printf("%s을(를) 구매했습니다. (남은 골드 %dG)\n", item.name, player->gold);
}

/* 판매: 구매 가격의 절반을 받는다 (강화 단계는 가격에 반영하지 않음)
 * 판매할 수 있는 것은 인벤토리에 보관 중인 장비뿐이다. */
static void sellItem(Player *player) {
    int number;
    int index;
    Item item;
    int sellPrice;
    int i;

    /* 판매 목록: 장비 이름과 판매 가격(구매 가격의 절반)을 함께 보여 준다 */
    printf("\n===== 판매 목록 ===== (보유 골드 %dG)\n", player->gold);
    if (player->inventory.count == 0) {
        printf("판매할 장비가 없습니다.\n");
        return;
    }
    for (i = 0; i < player->inventory.count; i++) {
        item = player->inventory.items[i];
        printf("%d. %s +%d [%s] - 판매가 %dG\n",
               i + 1, item.name, item.enhance, getTypeName(item.type), item.price / 2);
    }

    printf("판매할 번호 (0: 취소): ");
    number = readInt();
    if (number == 0) {
        return;
    }

    index = number - 1;
    if (index < 0 || index >= player->inventory.count) {
        printf("잘못된 번호입니다.\n");
        return;
    }

    /* 지우기 전에 아이템 정보를 복사해 둔다 */
    item = player->inventory.items[index];
    sellPrice = item.price / 2;

    player->gold = player->gold + sellPrice;
    removeItem(&player->inventory, index);

    printf("%s을(를) %dG에 판매했습니다. (보유 골드 %dG)\n",
           item.name, sellPrice, player->gold);
}

/* 상점 메뉴: 1 구매, 2 판매, 0 뒤로 */
void shopMenu(Player *player) {
    int choice;

    while (1) {
        printf("\n===== 상점 ===== (보유 골드 %dG)\n", player->gold);
        printf("1. 구매  2. 판매  0. 뒤로\n선택: ");
        choice = readInt();

        switch (choice) {
        case 1:
            buyItem(player);
            break;
        case 2:
            sellItem(player);
            break;
        case 0:
            return;
        default:
            printf("잘못된 입력입니다.\n");
            break;
        }
    }
}

/* ================================================================
 * 4. 장비
 * ================================================================ */

/* index 번째 장비를 장착한다. 성공 1, 실패 0.
 * - 이미 같은 부위를 장착 중이면 기존 장비는 인벤토리로 돌아간다.
 * - 기존 장비의 능력치는 빼고, 새 장비의 능력치는 더한다. */
int equipItem(Player *player, int index) {
    Item newItem;  /* 새로 장착할 장비 (복사본) */

    /* 1) 번호 검사 */
    if (index < 0 || index >= player->inventory.count) {
        printf("잘못된 번호입니다.\n");
        return 0;
    }

    /* 2) 선택한 장비를 복사해 두고 */
    newItem = player->inventory.items[index];

    /* 3) 인벤토리에서 지운다 (한 칸이 비게 됨) */
    removeItem(&player->inventory, index);

    if (newItem.type == ITEM_WEAPON) {
        /* 4) 이미 무기가 있으면: 기존 무기 능력치를 빼고 인벤토리로 돌려보낸다
         *    (방금 한 칸을 비웠으므로 항상 들어간다) */
        if (player->equipment.hasWeapon == 1) {
            player->attack = player->attack - getItemBonus(&player->equipment.weapon);
            addItem(&player->inventory, player->equipment.weapon);
            printf("%s을(를) 해제했습니다.\n", player->equipment.weapon.name);
        }
        /* 5) 새 무기를 장착하고 능력치를 더한다 */
        player->equipment.weapon = newItem;
        player->equipment.hasWeapon = 1;
        player->attack = player->attack + getItemBonus(&newItem);
    } else {
        /* 방어구도 같은 방식 */
        if (player->equipment.hasArmor == 1) {
            player->defense = player->defense - getItemBonus(&player->equipment.armor);
            addItem(&player->inventory, player->equipment.armor);
            printf("%s을(를) 해제했습니다.\n", player->equipment.armor.name);
        }
        player->equipment.armor = newItem;
        player->equipment.hasArmor = 1;
        player->defense = player->defense + getItemBonus(&newItem);
    }

    printf("%s을(를) 장착했습니다. (공격력 %d / 방어력 %d)\n",
           newItem.name, player->attack, player->defense);
    return 1;
}

/* 현재 장착한 장비와 전체 능력치를 보여 준다 */
void showEquipment(const Player *player) {
    printf("\n===== 현재 장비 =====\n");

    if (player->equipment.hasWeapon == 1) {
        printf("무기  : %s +%d (공격력 +%d)\n",
               player->equipment.weapon.name,
               player->equipment.weapon.enhance,
               getItemBonus(&player->equipment.weapon));
    } else {
        printf("무기  : 없음\n");
    }

    if (player->equipment.hasArmor == 1) {
        printf("방어구: %s +%d (방어력 +%d)\n",
               player->equipment.armor.name,
               player->equipment.armor.enhance,
               getItemBonus(&player->equipment.armor));
    } else {
        printf("방어구: 없음\n");
    }

    printf("전체 공격력: %d / 전체 방어력: %d\n", player->attack, player->defense);
}

/* 장비 관리 메뉴: 1 장착, 2 현재 장비 확인, 0 뒤로 */
void equipmentMenu(Player *player) {
    int choice;
    int number;

    while (1) {
        printf("\n===== 장비 관리 =====\n");
        printf("1. 장착  2. 현재 장비 확인  0. 뒤로\n선택: ");
        choice = readInt();

        switch (choice) {
        case 1:
            showInventory(player);
            if (player->inventory.count == 0) {
                break;
            }
            printf("장착할 번호 (0: 취소): ");
            number = readInt();
            if (number != 0) {
                equipItem(player, number - 1);
            }
            break;
        case 2:
            showEquipment(player);
            break;
        case 0:
            return;
        default:
            printf("잘못된 입력입니다.\n");
            break;
        }
    }
}

/* ================================================================
 * 5. 강화
 * ================================================================ */

/* 강화 정보(단계별 비용과 확률) 출력 */
static void showEnhanceInfo(void) {
    int i;

    printf("\n===== 강화 정보 =====\n");
    for (i = 0; i < MAX_ENHANCE; i++) {
        printf("+%d -> +%d : 비용 %dG, 성공 확률 %d%%\n",
               i, i + 1, 200 * (i + 1), ENHANCE_RATE[i]);
    }
    printf("무기는 1단계마다 공격력 +%d, 방어구는 1단계마다 방어력 +%d\n",
           WEAPON_ENHANCE_BONUS, ARMOR_ENHANCE_BONUS);
}

/* 장착 중인 장비 1개를 강화한다.
 * target  : 강화할 장비의 "주소" (복사본이 아니라 진짜 장비를 바꾸기 위해)
 * equipped: 그 부위를 장착하고 있는지 (1/0)
 * stat    : 강화 성공 시 올려 줄 능력치의 "주소" (&player->attack 또는 &player->defense)
 * bonus   : 1단계마다 올라가는 양 (무기 3, 방어구 2) */
static void enhanceItem(Player *player, Item *target, int equipped, int *stat, int bonus) {
    int cost;

    /* 1) 장착하지 않았으면 취소 */
    if (equipped == 0) {
        printf("장착한 장비가 없습니다.\n");
        return;
    }

    /* 2) 최대 단계면 취소 (배열 범위를 넘지 않도록 먼저 검사) */
    if (target->enhance >= MAX_ENHANCE) {
        printf("이미 최대 강화 단계(+%d)입니다.\n", MAX_ENHANCE);
        return;
    }

    /* 3) 비용은 단계를 올리기 "전에" 계산 */
    cost = 200 * (target->enhance + 1);
    if (player->gold < cost) {
        printf("골드가 부족합니다. (필요 %dG, 보유 %dG)\n", cost, player->gold);
        return;
    }

    /* 4) 비용 차감 (성공/실패와 상관없이) */
    player->gold = player->gold - cost;

    /* 5) 0~99 사이의 랜덤 숫자가 확률보다 작으면 성공 */
    if (rand() % 100 < ENHANCE_RATE[target->enhance]) {
        target->enhance++;
        *stat = *stat + bonus;  /* 플레이어 공격력(또는 방어력)도 바로 올린다 */
        printf("강화 성공! %s +%d (공격력 %d / 방어력 %d, 남은 골드 %dG)\n",
               target->name, target->enhance, player->attack, player->defense, player->gold);
    } else {
        printf("강화 실패... 단계는 +%d 그대로입니다. (남은 골드 %dG)\n",
               target->enhance, player->gold);
    }
}

/* 강화 메뉴: 1 무기 강화, 2 방어구 강화, 3 강화 정보 확인, 0 뒤로 */
void enhanceMenu(Player *player) {
    int choice;

    while (1) {
        printf("\n===== 장비 강화 ===== (보유 골드 %dG)\n", player->gold);
        printf("1. 무기 강화  2. 방어구 강화  3. 강화 정보 확인  0. 뒤로\n선택: ");
        choice = readInt();

        switch (choice) {
        case 1:
            /* &player->equipment.weapon : 실제 무기의 주소
             * &player->attack           : 실제 공격력의 주소 */
            enhanceItem(player, &player->equipment.weapon, player->equipment.hasWeapon,
                        &player->attack, WEAPON_ENHANCE_BONUS);
            break;
        case 2:
            enhanceItem(player, &player->equipment.armor, player->equipment.hasArmor,
                        &player->defense, ARMOR_ENHANCE_BONUS);
            break;
        case 3:
            showEnhanceInfo();
            break;
        case 0:
            return;
        default:
            printf("잘못된 입력입니다.\n");
            break;
        }
    }
}
