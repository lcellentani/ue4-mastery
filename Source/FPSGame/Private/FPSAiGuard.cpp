// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAiGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
AFPSAiGuard::AFPSAiGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAiGuard::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAiGuard::OnNoiseHeard);
}

// Called when the game starts or when spawned
void AFPSAiGuard::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPSAiGuard::OnPawnSeen(APawn* Pawn)
{
	if (Pawn == nullptr) {
		return;
	}

	DrawDebugSphere(GetWorld(), Pawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 10.0f);
}

void AFPSAiGuard::OnNoiseHeard(APawn * Instigator, const FVector & Location, float Volume)
{
	DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Green, false, 10.0f);
}

// Called every frame
void AFPSAiGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}