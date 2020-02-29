// Fill out your copyright notice in the Description page of Project Settings.


#include "FFAGM.h"
#include "Engine.h"
#include "Wizard.h"

AFFAGM::AFFAGM() {
}

void AFFAGM::BeginPlay()
{

}

void AFFAGM::RespawnPlayer(APlayerController* PC)
{
	if (IsValid(PC)) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("PC Valid")));
		//Will need to change for spawn points
		const FRotator SpawnRotation = FRotator(0, 0, 0);
		const FVector SpawnLocation = FVector(15305.0, 11310.0, 1182.0);

		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		AWizard* spawnedWizard = GetWorld()->SpawnActor<AWizard>(this->DefaultPawnClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		if (IsValid(spawnedWizard)) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("Wizard spawned and valid")));
			PC->Possess(spawnedWizard);
		}
	}
}
