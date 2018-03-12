// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSExtractionZone.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"

// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapComp->SetBoxExtent(FVector(200.0f));
	RootComponent = OverlapComp;

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::HandleOverlap);
	OverlapComp->SetHiddenInGame(false);

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->DecalSize = FVector(200.0f, 200.0f, 200.0f);
	DecalComp->SetupAttachment(RootComponent);
}

void AFPSExtractionZone::HandleOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AFPSCharacter* myCharacter = Cast<AFPSCharacter>(OtherActor);
	if (!myCharacter) {
		return;
	}
	
	UE_LOG(LogTemp, Log, TEXT("Overlapped with extraction zone!"));
	if (myCharacter->bIsCarryObjective) {
		AFPSGameMode* gameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
		if (gameMode) {
			gameMode->CompleteMission(myCharacter);
		}
	} else {
		UGameplayStatics::PlaySound2D(this, ObjectiveMissingSound);
	}
}

