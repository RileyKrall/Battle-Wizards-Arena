// Fill out your copyright notice in the Description page of Project Settings.


#include "VGrid.h"
#include "Engine.h"
#include "UnrealNetwork.h"
#include "VObject.h"

// Sets default values
AVGrid::AVGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SurfaceLevel = 300.0f;

	Scale = 50.0f;

	Origin = FVector(0, 0, 0);

	ChunkGrid = FVector(16, 16, 4);

	ChunkResolution = 32;

}

// Called when the game starts or when spawned
void AVGrid::BeginPlay()
{
	Super::BeginPlay();
	
	this->SetOwner(GetWorld()->GetFirstPlayerController());

	Chunks.SetNum(ChunkGrid.X*ChunkGrid.Y*ChunkGrid.Z);

	GenerateGrid();
	CalculateVoxels();
	
}

// Called every frame
void AVGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/**
*  Generates Chunk grid before spawning mesh
*
*/
void AVGrid::GenerateGrid()
{
	UWorld* const World = GetWorld();

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (int i = 0; i < ChunkGrid.X; i++) {
		for (int j = 0; j < ChunkGrid.Y; j++) {
			for (int k = 0; k < ChunkGrid.Z; k++) {

				int index = GetChunkIndex(i, j, k);

				FVector loc = FVector(i*(ChunkResolution), j*(ChunkResolution), k*(ChunkResolution));
				FRotator rot = FRotator(0, 0, 0);

				FVector origin = FVector(0, 0, 0);
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("origin X Y Z: %f, %f, %f"), loc.X, loc.Y, loc.Z));

				Chunks[index] = World->SpawnActor<AVObject>(FVector().ZeroVector, rot, ActorSpawnParams);
				Chunks[index]->SetOwner(World->GetFirstPlayerController());
				Chunks[index]->SetParameters(ChunkResolution, loc, Scale, SurfaceLevel, ID);
			}
		}
	}
	
}

void AVGrid::CalculateVoxels() {

	for (auto& Chunk : Chunks) {
		
		Chunk->CalcSurface();
		Chunk->GenerateSurface();
		// Trigger calculations in each chunk
	}
}

void AVGrid::EditPointInGrid_Implementation(int x, int y, int z, bool activatePoint)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("VGrid edit point")));
	// 1. Make a list of vectors that represent each voxel the point touches
	TArray<FVector> voxelCoords = {		FVector(x,		 y,		 z), // 0 
										FVector(x - 1,	 y,		 z), // 1 
										FVector(x - 1,	 y - 1,	 z), //2 
										FVector(x,		 y - 1,	 z), //3 
										FVector(x,		 y,		 z - 1), //4 
										FVector(x - 1,	 y,		 z - 1), //5 
										FVector(x - 1,	 y - 1,	 z - 1), //6
										FVector(x,		 y - 1,	 z - 1) }; //7

	// 2. Make list of objects to edit
	TArray<AVObject*> objectsToEdit;
	

	// 3. For each voxel, edit its associated object (8 voxels)
	for (int i = 0; i < 8; i++) {

		FVector voxel = voxelCoords[i];
		AVObject* toEdit = GetChunkFromVoxelCoord(voxel.X, voxel.Y, voxel.Z);

		if (objectsToEdit.Find(toEdit) == -1) {

			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("Adding chunk to the list of chunks")));
			objectsToEdit.Add(toEdit);

		}

		// check here if x, y, and z are within the grid bounds
		if (0 <= voxel.X && voxel.X < (ChunkResolution*ChunkGrid.X) && 0 <= voxel.Y && voxel.Y < (ChunkResolution*ChunkGrid.Y) && 0 <= voxel.Z && voxel.Z < (ChunkResolution*ChunkGrid.Z)) {
			toEdit->editVoxelVertices(voxel.X, voxel.Y, voxel.Z, i, activatePoint);
		}
		
	}
	// 4. Rebuild each object

	for (auto& chunk : objectsToEdit) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("Should be about to generate a surface")));
		chunk->GenerateSurface();
	}
}

bool AVGrid::EditPointInGrid_Validate(int x, int y, int z, bool activatePoint)
{
	return true;
}


void AVGrid::EditRadiusInGrid(int x, int y, int z, int radius, bool activatePoint)
{
	// 1. Make a list of vectors that represent each voxel the point touches
	TArray<FVector> voxelCoords = { FVector(x,		 y,		 z), // 0 yes
									FVector(x - 1,	 y,		 z), // 1 yes
									FVector(x - 1,	 y - 1,	 z), //2 
									FVector(x,		 y - 1,	 z), //3 
									FVector(x,		 y,		 z - 1), //4 yes
									FVector(x - 1,	 y,		 z - 1), //5 yes
									FVector(x - 1,	 y - 1,	 z - 1), //6
									FVector(x,		 y - 1,	 z - 1) }; //7

	// 2. Make list of objects to edit
	TArray<AVObject*> objectsToEdit;

	// 3. Get center vertex

	
	for (int i = 0; i < radius; i++) {
		for (int j = 0; j < (radius - i); j++) {
			for (int k = 0; k < (radius - i - j); k++) {
				for (int v = 0; v < 8; v++) {

					FVector offset = FVector(i, j, k);
					FVector voxel = voxelCoords[v] + offset;
					AVObject* toEdit = GetChunkFromVoxelCoord(voxel.X, voxel.Y, voxel.Z);


					if (objectsToEdit.Find(toEdit) == -1) {
						objectsToEdit.Add(toEdit);
					}

					if (0 <= voxel.X && voxel.X < (ChunkResolution*ChunkGrid.X) && 0 <= voxel.Y && voxel.Y < (ChunkResolution*ChunkGrid.Y) && 0 <= voxel.Z && voxel.Z < (ChunkResolution*ChunkGrid.Z)) {
						toEdit->editVoxelVertices(voxel.X, voxel.Y, voxel.Z, i, activatePoint);
					}

				}
			}
		}
	}

	for (auto& chunk : objectsToEdit) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("Should be about to generate a surface")));
		chunk->GenerateSurface();
	}
}

void AVGrid::EditWholeVoxelAndSurroundingInGrid(int x, int y, int z, bool activatePoint)
{
	TArray<FVector> vertices = {	FVector(x, y, z),
									FVector(x, y, z + 1),
									FVector(x, y + 1, z),
									FVector(x, y + 1, z + 1),
									FVector(x + 1, y, z),
									FVector(x + 1, y, z + 1),
									FVector(x + 1, y + 1, z),
									FVector(x + 1, y + 1, z + 1) };

	// 1. Make a list of vectors that represent each voxel the point touches
	TArray<FVector> voxelCoords = {		FVector(x,		 y,		 z), // 0 
										FVector(x - 1,	 y,		 z), // 1 
										FVector(x - 1,	 y - 1,	 z), //2 
										FVector(x,		 y - 1,	 z), //3 
										FVector(x,		 y,		 z - 1), //4 
										FVector(x - 1,	 y,		 z - 1), //5 
										FVector(x - 1,	 y - 1,	 z - 1), //6
										FVector(x,		 y - 1,	 z - 1) }; //7

	// 2. Make list of objects to edit
	TArray<AVObject*> objectsToEdit;

	for (auto& vertex : vertices) {
		// 3. For each voxel, edit its associated object (8 voxels)
		for (int i = 0; i < 8; i++) {

			FVector voxel = voxelCoords[i];
			AVObject* toEdit = GetChunkFromVoxelCoord(voxel.X, voxel.Y, voxel.Z);

			if (objectsToEdit.Find(toEdit) == -1) {

				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("Adding chunk to the list of chunks")));
				objectsToEdit.Add(toEdit);

			}

			// check here if x, y, and z are within the grid bounds
			if (0 <= voxel.X && voxel.X < (ChunkResolution*ChunkGrid.X) && 0 <= voxel.Y && voxel.Y < (ChunkResolution*ChunkGrid.Y) && 0 <= voxel.Z && voxel.Z < (ChunkResolution*ChunkGrid.Z)) {
				toEdit->editVoxelVertices(voxel.X, voxel.Y, voxel.Z, i, activatePoint);
			}

		}

	}

	// 4. Rebuild each object
	for (auto& chunk : objectsToEdit) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("Should be about to generate a surface")));
		chunk->GenerateSurface();
	}
}

AVObject * AVGrid::GetChunkFromVoxelCoord(int x, int y, int z)
{
	int index = GetChunkIndex(x/ChunkResolution, y/ChunkResolution, z/ChunkResolution);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("origin X Y Z: %d, %d, %d"), x/ChunkResolution, y/ChunkResolution, z/ChunkResolution));
	//GEngine->UE_LOG(YourLog, Warning, TEXT("MyCharacter's Health is %f, %f, %F"), x%ChunkResolution, y%ChunkResolution, z%ChunkResolution);

	return Chunks[index];
}

void AVGrid::SaveGrid()
{
}

void AVGrid::LoadGrid()
{
}

void AVGrid::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AVGrid, Chunks);
	DOREPLIFETIME(AVGrid, ID);

}

int AVGrid::GetChunkIndex(int x, int y, int z)
{
	return (x*ChunkGrid.Y*ChunkGrid.Z) + (y*ChunkGrid.Z) + (z);
}

