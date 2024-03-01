# MiniTank
UE5 연습 프로젝트

## 베이스 폰 만들기
터렛과 플레이어 캐릭터인 탱크의 부모가 되는 베이스 폰 클래스 생성하기

컴포넌트
USceneComponent
- Has a transform
- Supports attachment
	- 다른 컴포넌트를 씬 컴포넌트에 연결 가능
- 시각적 표현 없음
파생컴포넌트
UCapsuleComponent
-캡슐 모양의 충돌 지원
UStaticMeshComponent
-메쉬를 가지게 함

폰은 이미 컴포넌트가 있다
RootComponent
- USceneComponent타입

헤더불러오기
헤더는 가능한 작게 설정해야한다
프리프로세서라는 프로그램이 헤더를 불러 올 때 주석 까지 불러와 소를 붙인다. 사용할 때만 포함되게 하는게 좋다.
class UCapsuleComponent* CapsuleComp;
전방 선언하면 헤더가 변수의 타입을 알 수 있다. 다만 불완전 타입이기에 실제로 액세스 하려하면 오류가 생긴다. 즉 부모에 헤더가 있어야 한다.

즉 하위 클래스에서 헤더를 생성하고 직접 사용 안한느 부모 클래스는 전방 선언을 한다.

.cpp에는 사용할 것만, cpp서만 사용하는 헤더만
.h는 가능한 적은 내용
전방 선언사용하면 새 포인터 선언하는데 헤더 필요없음