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


## 컴포넌트 정의하기, Attach하기
```
	UPROPERTY()
	UStaticMeshComponent* BaseMesh;

	UPROPERTY()
	UStaticMeshComponent* TurretMesh;
    
    ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	BaseMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}
```


## Instance vs BPDefatult
### UPROPERTY Specifiers(지정자)
EditAnywhere - 어디서나 편집가능
VisibleAnywhere - 어디서나 보기만 가능
VisibleInstanceOnly - 인스턴스화된 객체만 보기가능
VisibleDefaultOnly - BP에서만 보기 가능
EditDefaultOnly = BP에서만 편집가능
EditInstanceOnly = 인스턴스만 편집가능
### 추가 지정자
BlueprintReadWrite - 이벤트 그래프에서 Get Set 노드로 사용 가능
BlueprintReadOnly - BP노드 Get만 사용 가능
### private에도 노출시키기
위 지정자들은 public에서만 사용가능 private도 사용 하려면 추가 지정자를 설정해 줘야한다.
meta = (AllowPrivateAccess = true)
### 변수 카테고리 묶음
Category = "이름"

## 지정자 사용 주의 사항
Component는 EditAnywher 말고 VisibleAnywhere 사용 포인터는 웬만하면 편집 못하게 하기

## 카메라 지정
![image](https://github.com/mohitto55/mohitto55.github.io/assets/154340583/f979585b-2271-4ec9-9ff0-0ee5b945b41c)
SpringArm 밑에 Camera를 두면 따라다닌다

## Component 코드로 설정(복습)

```cpp
CreateDefaultSubobject<타입>(TEXT("이름"));
포인터->SetupAttachment(RootComponent);
```

## BP의 베이스 코드 설정
BP->상단 ClassSetting-> 디테일패널 Parent Class 설정

## Possess(빙의)시키기
디테일 패널 -> Auto Possess Player

빙의 안하고 플레이 하려면 ALT+S혹은 플레이 버튼 옆 점3개 눌러서 시뮬레이션 누르기

## 객체 움직이기
AddActorLocalOffset(이동거리,Sweep체크) - 로컬좌표계로 이동
### Sweep시 주의사항
오직 루트 컴포넌트 설정만 반응함
벽에 막히려면 콜리젼이 Block됐는지 확인

## 델타타임 구하기
```cpp
#include "Kismet/GameplayStatics.h"
UGameplayStatics::GetWorldDeltaSeconds(this);
```

## 회전하기
struct FRotator
float Pitch - Y축 오른쪽
float Yaw - Z축 위아래
float Roll - x축 전면
AddActorLocalRotation(rorator, true);

## FVector,Rotator 선언시 주의사항
FVector::ZeroVector;
FRotator::ZeroRotator;
최소 이렇게 초기화를 무조건 해줘야함!!

## 컨트롤러 설명
폰은 컨트롤러를 가짐
APlayerController
이 컨트롤러를 APawn::GetController를 통해 가져올 수 있음
GetController는 AController를 반환함
APlayerController는 AController의 자식이기에 캐스팅 해야함

## 마우스 레이 쏘기
```cpp
PlayerControllerRef->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
```

## 특정 메쉬만 회전시키기
```cpp
TurretMesh->SetWorldRotation(FMath::RInterpTo(
	TurretMesh->GetComponentRotation(), 
	LookAtRotation, 
	UGameplayStatics::GetWorldDeltaSeconds(this), 
	5.f));
```
### 부드럽게 보간회전
`FMath::RInterpTo()`

## 타이머 설정
```cpp
FTimerHandle FireRateTimerHandle;
float FireRate = 2.f;
void CheckFireCondition();
```
변수설정

```cpp
GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
```

델리게이트 삽입
```cpp
ToonTanksPlayerController->SetPlayerEnabledState(false);
FTimerHandle PlayerEnableTimerHandle;
FTimerDelegate PlayerEnableTimer`Delegate = FTimerDelegate::CreateUObject(
	ToonTanksPlayerController,
	&AToonTanksPlayerController::SetPlayerEnabledState,
	true
);
GetWorldTimerManager().SetTimer(PlayerEnableTimerHandle,
	PlayerEnableTimerDelegate,
	StartDelay,
	false);
```

## TSubclassOf
UClass 타입을 저장하는 템플릿 클래스, C++와 블프 사이에는 리플렉션을 가능하게 하는 함수가 내장되있다.

## 블프(프리펩) 소환
UWorld::SpawnActor<>()

## 발사체 속도조절
```cpp
UPROPERTY(VisibleAnywhere)
class UProjectileMovementComponent* ProjectileMovementComponent;
```

```cpp
ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
ProjectileMovementComponent->MaxSpeed = 1300.f;
ProjectileMovementComponent->InitialSpeed = 1300.f;
```

## 충돌 콜백 함수 사용법
```cpp
UFUNCTION()
void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
```
UFUNCTION을 붙여야 함
```cpp
UPrimitiveComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit)
```

## ActorComponent
헤더 클래스 위쪽 UCLASS보면
meta=(BlueprintSpawnableComponent) 가 있어야 블프에서 해당 컴포넌트 추가가능

## 소유자 지정
GetOwner는 자기 자신만을 뜻하는게 아님
AActor::SetOwner 을 통해서 주인을 정할 수 있음
그래서 GetOwner를 하면 다른 객체가 나올 수 있음

## Instigator(선동자)란
어떤 액션을 시작한 개체를 나타낸다
- 데미지를 가하는 객체
- 특정 이벤트의 원천이 되는 개체
- 두 개체간 상호 작용에서 시작한 주체

## 테이크 데미지 델리게이트
GetOwner()->OnTakeAnyDamage.AddDynamic(this,함수);


## 빌드오류
------ Building 1 action(s) started ------
  [1/1] Compile [x64] HealthComponent.cpp
  C:\Users\admin\git\UE5\Tank\Source\ToonTanks\HealthComponent.cpp(24): error C3867: 'UHealthComponent::DamageTaken': ???? ??????ϴ? '&'?? ????? ?? ????? ???弼??
  
주소값을 못찾는 오류 레퍼런스 연산자& 붙였는지 확인하기

## 파괴하기
Destroy();

## 게임모드
- AGameModeBase
	- 게임의 전반적인 룰을 다룸
	- 승리 조건 등

- AGameMode
	- 베이스의 자식
	- 상세한 기능들이 있음
	- Match state - 멀티플레이 대진
간단한 게임은 AGameModeBase

### 게임모드 수정법
Edit->Project Setting->Project->Maps&Modes->Default GameMode
### 소유자지정
GameMode BP에서 디폴트 액터를 선택하면 auto possess player 설정을 안해줘도 된다.

## 객체 삭제하기
```
SetActorHiddenInGame(true); // 안보이게 숨기기
SetActorTickEnabled(false); // 틱 정지하기
```

## CustomPlayerController
C++로 PlayerController 상속 받는 클래스 만들고 CustomPlayerController 베이스 BP를 만든다.
BP_GameMode에서 Player Controller Class를 바꾼다.

## 마우스 커서 숨기기
해당 기능은 PlayerController에서 관리한다.
PlayerController->bShowMouseCursor = false; 하면 사라진다.


## Widget
위젯이 UI 마우스 우클릭해서 User Interface->BP 위젯 생성

## 위젯을 블프 변수로
위젯 에디터 창에서 위젯 클릭후 디테일 창에서 IS Variable 체크하면 블프 그래프 창에서 변수로 사용 가능

## 블프에서 텍스트 글자 바꾸기
텍스트 변수를 가져와Get 노드로 바꾸고 연결해서 Set Text 노드 생성하기

## 위젯 블프서 생성
- Create Widget 노드 만들고 WBP_위젯 지정해주기
- Add to viewport 노드 만들고 연결해주기

## Select 노드
- 들어오는 bool 값이 true false 에 따라 반환 값 내용 정하기 가능
- Return Value의 반환 종류를 우클릭 Change pin type로 바꾸기 가능

## 파티클 이펙트소환
```cpp
UPROPERTY(EditAnywhere, Category = "Combat")
class UParticleSystem* Hitparticles;

UGameplayStatics::SpawnEmitterAtLocation(this, Hitparticles, GetActorLocation(), GetActorRotation());
```

## 파티클 지속 소환하는 파티클시스템컴포넌트
```cpp
#include "Particles/ParticleSystemComponent.h"
TrailParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke Trail"));
```

## 사운드 재생 컴포넌트
```cpp
UPROPERTY(EditAnywhere)
class USoundBase* DeathSound;

UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
```

## 카메라 쉐이크
BP로 카메라쉐이크 생성
```cpp
UPROPERTY(EditAnywhere)
TSubclassOf<class UCameraShakeBase> HitCameraShake;

GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShake);
```
## 언리얼 코딩 관행
1. if 단일문도 중괄호를 쓴다
2. auto는 자제한다

## 프로젝트 패키징(빌드) 하는법
플레이버튼 오른쪽 플랫폼 버튼 누르고 윈도우로 패키징

스타일링 프로포션