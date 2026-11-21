# C언어 RPG 전투 게임

C언어로 만든 **메뉴 선택형 텍스트 RPG**입니다.
캐릭터를 만들고 몬스터와 싸워 경험치와 골드를 얻은 뒤, 장비를 사고 강화해서 더 강한 몬스터와 보스에 도전합니다.

## 주요 기능

| 번호 | 메뉴 | 설명 |
|---|---|---|
| 1 | 캐릭터 생성 | 이름 입력, 직업 선택 (전사 / 마법사 / 궁수) |
| 2 | 캐릭터 정보 | 능력치, 골드, 장착 장비 확인 |
| 3 | 사냥터 | 초급 / 중급 / 고급 / 보스전 |
| 4 | 상점 | 포션, 장비 구매 |
| 5 | 인벤토리 | 보유 아이템 확인 및 사용 |
| 6 | 장비 관리 | 장비 장착 / 해제 |
| 7 | 여관 | HP 회복 |
| 8 | 퀘스트 | 퀘스트 확인 및 보상 받기 |
| 9 | 장비 강화 | 골드로 장비 강화 |
| 0 | 게임 종료 | 최종 정보 출력 후 종료 |

## 폴더 구조

```
.
├── main.c           # 메인 메뉴 (각 기능을 연결)
├── Player/          # 1번: 캐릭터 생성, 정보, 여관
├── Rpgmonster2/     # 2번: 몬스터 생성, 전투
└── Item_contents/   # 3번: 상점, 인벤토리, 장비, 강화, 퀘스트
```

## 빌드 및 실행

```bash
gcc -IPlayer -IItem_contents -IRpgmonster2 main.c Player/character.c \
    Item_contents/item.c Item_contents/quest.c \
    "Rpgmonster2/battle selection.c" "Rpgmonster2/combat function.c" \
    "Rpgmonster2/monster generation.c" -o rpg

./rpg
```

- **Visual Studio**: 프로젝트 속성 → C/C++ → 일반 → 추가 포함 디렉터리에 `Player`, `Item_contents`, `Rpgmonster2`를 추가하세요.
- 각 폴더의 테스트용 `main` 파일(`Player/main.c`, `Item_contents/test_main.c` 등)은 빌드에서 빼야 합니다.

## 팀원

| 담당 | 역할 |
|---|---|
| 1번 | 캐릭터 (Player) |
| 2번 | 몬스터 / 전투 (Rpgmonster2) |
| 3번 | 아이템 / 콘텐츠 (Item_contents) |
