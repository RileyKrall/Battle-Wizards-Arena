// Fill out your copyright notice in the Description page of Project Settings.


#include "VoxelManager.h"
#include "Engine.h"
#include "FFAGM.h"
#include "WizardPlayerController.h"
#include "UnrealNetwork.h"
#include "VGrid.h"

// Sets default values
AVoxelManager::AVoxelManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVoxelManager::BeginPlay()
{
	Super::BeginPlay();

	UWorld* const World = GetWorld();

	this->SetOwner(GetWorld()->GetFirstPlayerController());
	// if the server put manager pointer on game mode

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector loc = FVector(0, 0, 0);
	FRotator rot = FRotator(0, 0, 0);

	AWizardPlayerController* PC = Cast<AWizardPlayerController>(World->GetFirstPlayerController());
	if (IsValid(PC)) {
		PC->VoxMan = this;
	}
	

	Grids.Add(World->SpawnActor<AVGrid>(loc, rot, ActorSpawnParams));
	Grids[0]->SetID(0);

}

// Called every frame
void AVoxelManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AVGrid * AVoxelManager::GetGrid(int GridID)
{
	return Grids[GridID];
}

void AVoxelManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AVoxelManager, Grids);

}