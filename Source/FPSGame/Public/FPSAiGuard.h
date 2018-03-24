// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAiGuard.generated.h"

class UPawnSensingComponent;

UENUM(BlueprintType)
enum class  EAIState : uint8 {
	Idle,
	Suspicious,
	Alerted
};

UCLASS()
class FPSGAME_API AFPSAiGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAiGuard();

protected:
	FRotator OriginalRotation;
	FTimerHandle TimerHandle_ResetOrientation;
	EAIState GuardState;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void ResetOrientation();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void OnNoiseHeard(APawn* Pawn, const FVector& Location, float Volume);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnStateChange(EAIState NewState);

	void SetGuardState(EAIState NewState);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
