#ifndef CHARACTER_H
#define CHARACTER_H
#include "item.h"

#define NAME_LEN 32
#define INN_COST 30   /* 여관 이용 요금 */

/* 직업 */
enum Job {
    JOB_WARRIOR = 1,  /* 전사   */
    JOB_MAGE          /* 마법사 */
};

/* 플레이어 구조체 (2번, 3번 파트에서도 공용으로 사용) */
typedef struct {
    char name[NAME_LEN];
    int  job;           /* enum Job 값 */
    int  level;
    int  exp;           /* 현재 경험치 */
    int  exp_to_next;   /* 다음 레벨까지 필요한 경험치 */
    int  hp;
    int  max_hp;
    int  atk;           /* 공격력 */
    int  def;           /* 방어력 */
    int  gold;          /* 3번 파트(상점)에서 사용 */
    int  potion;        /* 2번/3번 파트(포션)에서 사용 */
} Player;

/* 입력 버퍼 비우기 */
void clear_input_buffer(void);

/* 직업 이름 문자열 반환 */
const char *get_job_name(int job);

/* 캐릭터 생성 (이름, 직업 선택 후 초기 능력치 설정) */
void create_character(Player *p);

/* 캐릭터 정보 출력 */
void show_player_info(const Player *p);

/* 경험치 획득 (필요하면 자동으로 레벨업, 여러 번 가능) */
void add_exp(Player *p, int amount);

/* 레벨업 처리 (직업별 성장) */
void level_up(Player *p);

/* 여관 (골드를 내고 HP 전체 회복) */
void visit_inn(Player *p);

#endif
