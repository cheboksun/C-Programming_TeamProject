/*
 * main.c
 * 팀 공용 main 파일 (1번 + 2번 + 3번 통합)
 *
 * ================================================================
 * 이 파일이 하는 일
 * ================================================================
 * 게임의 "마을(메인 메뉴)"이다. 직접 게임 기능을 만들지 않고,
 * 메뉴 번호를 입력받아 각 담당이 만든 함수를 불러 주는 "연결 역할"만 한다.
 *
 *   프로그램 시작
 *     → 한글 설정(윈도우) + 랜덤 초기화 + 타이틀 출력
 *     → [반복] 마을 메뉴 출력 → 번호 입력 → 해당 담당 함수 호출
 *     → 0번 입력 시 최종 정보 출력 후 종료
 *
 * ================================================================
 * 메뉴와 호출하는 함수
 * ================================================================
 *   1 캐릭터 생성 : create_character (1번) → initQuests (3번)
 *   2 캐릭터 정보 : show_player_info (1번) + showEquipment (3번)
 *   3 사냥터      : battle_menu (2번)
 *   4 상점        : shopMenu (3번)
 *   5 인벤토리    : inventoryMenu (3번)
 *   6 장비 관리   : equipmentMenu (3번)
 *   7 여관        : visit_inn (1번)
 *   8 퀘스트      : questMenu (3번)
 *   9 장비 강화   : enhanceMenu (3번)
 *   0 게임 종료   : show_player_info (1번) 후 종료
 *
 * ================================================================
 * 사용하는 헤더 (폴더가 3개로 나뉘어 있음)
 * ================================================================
 *   character.h (Player 폴더)        : 팀 표준 Player 구조체 + 1번 함수
 *   battle.h    (Rpgmonster2 폴더)   : 2번 사냥터 함수
 *   content.h   (Item_contents 폴더) : 3번 함수 + readInt (공용 입력)
 *
 * ================================================================
 * 빌드 방법 (헤더 폴더를 -I로 꼭 알려 줘야 함)
 * ================================================================
 *   gcc -IPlayer -IItem_contents -IRpgmonster2 main.c Player/character.c
 *       Item_contents/item.c Item_contents/quest.c
 *       "Rpgmonster2/battle selection.c" "Rpgmonster2/combat function.c"
 *       "Rpgmonster2/monster generation.c" -o rpg
 *
 *   Visual Studio: 프로젝트 속성 → C/C++ → 일반 → 추가 포함 디렉터리에
 *                  세 폴더(Player, Item_contents, Rpgmonster2)를 추가한다.
 *
 * ※ 기존 테스트용 main 3개(Player/main.c, Rpgmonster2/main.c,
 *    Item_contents/test_main.c)는 빌드에서 빼야 한다. (main이 2개 이상이면 에러)
 */
#include <stdio.h>    /* printf */
#include <stdlib.h>   /* srand, system */
#include <time.h>     /* time (랜덤 시작값으로 현재 시간을 씀) */

#include "character.h"  /* 1번: Player, 캐릭터 생성/정보/여관 */
#include "battle.h"     /* 2번: 사냥터 */
#include "content.h"    /* 3번: 상점/인벤토리/장비/강화/퀘스트, readInt */

/* ================================================================
 * 메인 메뉴 번호
 * switch 문에서 숫자 대신 이름을 쓰면 어떤 메뉴인지 바로 알 수 있다.
 * (예: case 3: 보다 case MENU_HUNT: 가 읽기 쉽다)
 * ================================================================ */
#define MENU_EXIT       0   /* 게임 종료 */
#define MENU_CREATE     1   /* 캐릭터 생성 */
#define MENU_INFO       2   /* 캐릭터 정보 */
#define MENU_HUNT       3   /* 사냥터 */
#define MENU_SHOP       4   /* 상점 */
#define MENU_INVENTORY  5   /* 인벤토리 */
#define MENU_EQUIPMENT  6   /* 장비 관리 */
#define MENU_INN        7   /* 여관 */
#define MENU_QUEST      8   /* 퀘스트 */
#define MENU_ENHANCE    9   /* 장비 강화 */

/* 게임 시작 화면 (2번 테스트 main에서 가져옴)
 * static: 이 파일(main.c) 안에서만 쓰는 함수라는 뜻. 다른 파일과 이름이 겹쳐도 괜찮다. */
static void showTitle(void)
{
    printf("\n========================================\n");
    printf("             텍스트 RPG 게임\n");
    printf("========================================\n");
    printf("모험의 세계에 오신 것을 환영합니다!\n");
    printf("========================================\n");
}

/* 마을(메인 메뉴) 출력
 * player : 상단 상태줄에 보여 줄 캐릭터 (const: 읽기만 하고 바꾸지 않음)
 * created: 캐릭터를 만들었으면 1 → 상태줄(이름/레벨/HP/골드/포션) 출력
 *          아직 안 만들었으면 0 → "먼저 캐릭터를 생성하세요" 안내 출력
 *          (만들기 전의 player는 이름이 비어 있어서 상태줄이 이상하게 보이기 때문) */
static void showMainMenu(const Player *player, int created)
{
    printf("\n================ 마을 ================\n");
    if (created) {
        printf(" %s | Lv.%d | HP %d/%d | %dG | 포션 %d개\n",
               player->name, player->level, player->hp, player->max_hp,
               player->gold, player->potion);
    } else {
        printf(" ※ 먼저 1번에서 캐릭터를 생성하세요.\n");
    }
    printf("--------------------------------------\n");
    printf(" 1. 캐릭터 생성      2. 캐릭터 정보\n");
    printf(" 3. 사냥터           4. 상점\n");
    printf(" 5. 인벤토리         6. 장비 관리\n");
    printf(" 7. 여관             8. 퀘스트\n");
    printf(" 9. 장비 강화        0. 게임 종료\n");
    printf("======================================\n");
    printf("선택: ");
}

/* 게임 종료 화면 (2번 테스트 main의 "게임 종료 후 캐릭터 상태 확인"을 가져옴)
 * 캐릭터를 만든 경우에만 최종 정보를 보여 준다. */
static void showGoodbye(const Player *player, int created)
{
    if (created) {
        printf("\n========== 최종 캐릭터 정보 ==========\n");
        show_player_info(player);  /* 1번 함수 재사용 */
    }
    printf("게임을 종료합니다. 플레이해 주셔서 감사합니다!\n");
}

int main(void)
{
    /* 플레이어 1명. 게임 전체에서 이 변수 하나를 모든 함수가 주소(&player)로 받아 고친다.
     * {0}: 모든 값을 0으로 시작 → 인벤토리 0개, 장비 없음(hasWeapon=0), 퀘스트 비어 있음.
     *      쓰레기 값이 들어가지 않도록 반드시 0으로 초기화한다. */
    Player player = {0};

    /* 캐릭터를 만들었는지 기억하는 변수 (0: 아직, 1: 만듦)
     * 캐릭터 없이 사냥터/상점에 들어가면 이름도 HP도 0이라 게임이 꼬이므로 이것으로 막는다. */
    int created = 0;

    int menu;  /* 사용자가 고른 메뉴 번호 */

#ifdef _WIN32
    /* 윈도우 콘솔은 기본이 한글 CP949라 UTF-8 글자가 깨진다.
     * chcp 65001 = 콘솔을 UTF-8로 바꾸는 명령. (1번 테스트 main에서 가져옴)
     * #ifdef _WIN32: 윈도우에서만 실행되고, macOS/리눅스에서는 이 줄이 없는 것처럼 무시된다. */
    system("chcp 65001 > nul");
#endif

    /* 랜덤 시작값 설정. rand()를 쓰는 곳(몬스터 종류, 도망 확률, 강화 성공 확률)이 매번 다르게 나오도록.
     * ※ 프로그램 전체에서 여기서 딱 1번만 부른다. 다른 파일에서 또 부르면 안 된다. */
    srand((unsigned int)time(NULL));

    showTitle();

    /* 게임 루프: 0번(종료)을 고를 때까지 마을 메뉴를 계속 반복한다. */
    while (1) {
        showMainMenu(&player, created);

        /* 3번의 공용 입력 함수
         * - 숫자가 아닌 글자를 넣으면 "숫자를 입력하세요"라고 하고 다시 받는다.
         * - 숫자 뒤의 엔터까지 지워 주므로, 다음에 1번 create_character가
         *   fgets로 이름을 받을 때 빈 줄이 들어가는 문제가 생기지 않는다.
         * - 입력이 끝나면(Ctrl+Z / Ctrl+D) 프로그램을 종료한다. */
        menu = readInt();

        /* 캐릭터가 없으면 "1 생성"과 "0 종료"만 허용하고 나머지는 막는다. */
        if (!created && menu != MENU_CREATE && menu != MENU_EXIT) {
            printf("먼저 캐릭터를 생성하세요.\n");
            continue;  /* switch로 가지 않고 다시 메뉴부터 */
        }

        switch (menu) {
        case MENU_CREATE:
            /* 캐릭터는 1명만. 이미 있으면 다시 만들지 않는다.
             * (다시 만들면 1번 필드만 새로 채워지고 인벤토리/퀘스트는 예전 것이 남아 꼬이기 때문) */
            if (created) {
                printf("이미 캐릭터가 있습니다. (%s, Lv.%d)\n", player.name, player.level);
                break;
            }
            create_character(&player);  /* 1번: 이름 입력, 직업 선택, 직업별 기본 능력치/골드/포션 */
            initQuests(&player);        /* 3번: 퀘스트 3개를 player.quests에 복사
                                         *      ※ 이걸 안 부르면 퀘스트 목록이 비어 있다. 꼭 생성 직후에! */
            created = 1;
            break;

        case MENU_INFO:
            show_player_info(&player);  /* 1번: 이름/직업/레벨/경험치/HP/공격/방어/골드/포션 */
            showEquipment(&player);     /* 3번: 장착한 무기/방어구와 강화 단계 */
            break;

        case MENU_HUNT:
            /* 2번: 사냥터(초원/어둠의 숲/마왕의 성) 선택 → 일반/보스 선택 → 전투
             * 전투 안에서 경험치(1번 add_exp → 레벨업), 골드, 퀘스트 처치 수(3번 onMonsterKilled)가 처리된다.
             * "0. 마을로 돌아가기"를 고르거나 전투에서 지면 여기로 돌아온다. */
            battle_menu(&player);
            break;

        case MENU_SHOP:
            shopMenu(&player);          /* 3번: 무기/방어구/포션 구매, 장비 판매 */
            break;

        case MENU_INVENTORY:
            inventoryMenu(&player);     /* 3번: 보유 장비 확인, 포션 사용, 장비 버리기 */
            break;

        case MENU_EQUIPMENT:
            equipmentMenu(&player);     /* 3번: 장비 장착 (장착하면 player.atk / player.def가 바로 오름) */
            break;

        case MENU_INN:
            visit_inn(&player);         /* 1번: 골드(30G)를 내고 HP를 최대로 회복 */
            break;

        case MENU_QUEST:
            questMenu(&player);         /* 3번: 퀘스트 목록/수락/진행 상황/보상 받기 */
            break;

        case MENU_ENHANCE:
            enhanceMenu(&player);       /* 3번: 장착한 무기/방어구 강화 (확률, 최대 +5) */
            break;

        case MENU_EXIT:
            showGoodbye(&player, created);
            return 0;                   /* main을 끝내면 프로그램 종료 */

        default:
            /* 0~9가 아닌 숫자 (예: 15, -3) */
            printf("잘못된 선택입니다. 0~9 중에서 고르세요.\n");
            break;
        }
    }
}
