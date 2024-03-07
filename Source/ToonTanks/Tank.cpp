// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"

ATank::ATank() {
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}
void ATank::Move(const FInputActionValue& Value) {
	FVector Movement = Value.Get<FVector>();
	Movement.Y = 0;
	Movement *= Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalOffset(Movement, true);
}

void ATank::Turn(const FInputActionValue& Value) {
	FRotator DeltaRotation = FRotator::ZeroRotator;
	FVector Movement = Value.Get<FVector>();
	float RotateValue = Movement.Y;
	DeltaRotation.Yaw = RotateValue * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalRotation(DeltaRotation, true);
}

void ATank::BeginPlay() {
	Super::BeginPlay();
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(MoveMappingContext, 0);
		}

		TankPlayerController = PlayerController;
	}
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(MoveInput, ETriggerEvent::Triggered, this, &ATank::Move);
		EnhancedInputComponent->BindAction(MoveInput, ETriggerEvent::Triggered, this, &ATank::Turn);
		EnhancedInputComponent->BindAction(AttackInput, ETriggerEvent::Started, this, &ATank::Fire);
	}
}

void ATank::Tick(float DeltaTile) {
	Super::Tick(DeltaTile);
	if (TankPlayerController) {
		FHitResult HitResult;
		TankPlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 25.f, 12, FColor::Red, false, -1.f);

		RotateTurret(HitResult.ImpactPoint);
	}
}

void ATank::HandleDestruction() {
	Super::HandleDestruction();
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	bAlive = false;
}