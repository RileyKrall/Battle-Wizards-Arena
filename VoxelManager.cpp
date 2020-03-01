// Voxel Manager is the overall manager of voxel coordinate planes. Each coordinate plane is represented by the VGrid.
// When needing access to a specific grid, you will contact your maps voxel manager.


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

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector loc = FVector(0, 0, 0);
	FRotator rot = FRotator(0, 0, 0);
	
	// Place pointer to this, into the local first player controller, allowing for the player to edit voxels
	AWizardPlayerController* PC = Cast<AWizardPlayerController>(World->GetFirstPlayerController());
	if (IsValid(PC)) {
		PC->VoxMan = this;
	}
	
	// Temporary implementation. This spawns a single instance on the map, will be the map terrain.
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
