// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"

/**
 * 
 */
struct FInputActionValue;
UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	class UInputMappingContext* MoveMappingContext;

	UPROPERTY(EditAnywhere)
	class UInputAction* MoveInput;

	UPROPERTY(EditAnywhere)
	class UInputAction* AttackInput;

	UPROPERTY(EditAnywhere)
	float Speed = 400.f;

	UPROPERTY(EditAnywhere)
	float TurnRate = 45.f;

	APlayerController* GetTankPlayerController() const { return TankPlayerController; }
private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* Camera;

	void Move(const FInputActionValue& Value);
	void Turn(const FInputActionValue& Value);

	APlayerController* TankPlayerController;
protected:
	virtual void BeginPlay() override;

public:
	ATank();
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Tick(float DeltaTime);
	void HandleDestruction();

	bool bAlive = true;
};
