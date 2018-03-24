// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAiGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "FPSGameMode.h"

#include "DrawDebugHelpers.h"

// Sets default values
AFPSAiGuard::AFPSAiGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAiGuard::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAiGuard::OnNoiseHeard);

	GuardState = EAIState::Idle;
}

// Called when the game starts or when spawned
void AFPSAiGuard::BeginPlay()
{
	Super::BeginPlay();

	OriginalRotation = GetActorRotation();
}

void AFPSAiGuard::ResetOrientation()
{
	if (GuardState == EAIState::Alerted) {
		return;
	}

	SetActorRotation(OriginalRotation);

	SetGuardState(EAIState::Idle);
}

void AFPSAiGuard::OnPawnSeen(APawn* Pawn)
{
	if (Pawn == nullptr) {
		return;
	}

	DrawDebugSphere(GetWorld(), Pawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 10.0f);

	AFPSGameMode* gameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (gameMode) {
		gameMode->CompleteMission(Pawn, false);
	}

	SetGuardState(EAIState::Alerted);
}

void AFPSAiGuard::OnNoiseHeard(APawn * Pawn, const FVector & Location, float Volume)
{
	if (GuardState == EAIState::Alerted) {
		return;
	}

	DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Green, false, 10.0f);

	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();

	FRotator NewLoookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLoookAt.Pitch = 0.0f;
	NewLoookAt.Roll = 0.0f;
	SetActorRotation(NewLoookAt);

	GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AFPSAiGuard::ResetOrientation, 3.0f);

	SetGuardState(EAIState::Suspicious);
}

void AFPSAiGuard::SetGuardState(EAIState NewState)
{
	if (GuardState == NewState) {
		return;
	}

	GuardState = NewState;

	OnStateChange(GuardState);
}

// Called every frame
void AFPSAiGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
