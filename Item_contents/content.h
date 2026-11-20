/*
 * content.h
 * 3번 담당(아이템·콘텐츠)의 "함수 목록" 파일입니다.
 * item.c, quest.c에 있는 함수 중 다른 파일(팀 main, 2번 전투)이 쓰는 것을 알려 줍니다.
 *
 * ※ Player는 1번의 character.h에 있습니다. (팀 표준 Player)
 *    character.h → item.h 순서로 include되므로, 여기서는 character.h만 include하면 됩니다.
 */

#ifndef CONTENT_H
#define CONTENT_H

#include "character.h"  /* Player (1번) + Item, Inventory, Quest 등 (item.h) */

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

/* 경험치 지급은 1번의 add_exp() (character.h)를 사용합니다. (예전 gainExp는 삭제) */

#endif
