# Isaac_Imitation
winAPI 를 활용한 2D 게임 The binding of Isaac 게임 모작 프로젝트

## 개요
| 개요 | 내용 |
|:--------:|:--------|
| **프로젝트 기간** | 2024/10 ~ 2024/11 |
| **개발 환경, 언어** | C++ </br> WinAPI </br> Visual Studio 2022|

## 게임 정보
![image](https://github.com/user-attachments/assets/548b3589-6f24-47ec-9706-9487e55fcf90)

| 개요 | 내용 |
|:--------:|:--------|
| **게임명** | The Binding of Isaac (아이작의 번제) |
| **출시일** | 2011년 9월 29일 |
| **장르** | 탑뷰 슈팅, 로그라이트 |
---
## 리소스 참고
https://www.spriters-resource.com/pc_computer/bindingofisaacrebirth/

---
## 프로젝트 개발 로드맵
https://docs.google.com/spreadsheets/d/1AmnNOEQIEIc_h4LOT36UaBcONyJCIVY-a6rVudFqnBI/edit?usp=sharing

### 1주차 (10/23 ~ 10/29)
- 에디터 구현
  - 맵
  - 스프라이트 및 콜라이더
  - 애니메이션
- Player Script
  - 기본 이동
  - 공격
- Rigidbody
  - 플레이어 이동
  - 총알 중력 영향
- Main Menu
  - 메인 화면
  - 게임 진입
  - 에디터 진입
  - 게임 종료
    
### 2주차 (10/30 ~ 11/05)
- Player Script
- Monster Script
  - Charger (가로, 세로 방향에 플레이어가 있을 시 일직선 돌진)
  - Trite (플레이어 방향으로 점프)
  - Pooter (플레이어 인식 시 플레이어 방향 핏방울 발사)
- Bullet Script
- Camera Script
  - 플레이어의 방 이동에 따른 카메라 움직임

### 3주차 (11/06 ~ 11/12)
- Boss Script - Monstro
  - 플레이어 방향으로 무작위 핏방울들 분출
  - 점프를 반복하며 플레이어에게 접근
  - 도약 후 플레이어 위치에 강하, 강하하면서 핏방울 주변으로 분출
- UI

### 4주차 (11/13 ~ 11/19)
- UI
- Intro & Epilogue

### 기타
시간적 여유가 된다면 BFS를 이용한 랜덤 방 생성 알고리즘을 적용하기
