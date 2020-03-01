// Voxel coordinate grid that keeps track of all chunks in the grid. Also manages cross chunk editing.
// Chunks are represented by a VObject.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VObject.h"
#include "VGrid.generated.h"

UCLASS()
class WIZARDBATTLE_API AVGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	float SurfaceLevel;

	UPROPERTY(EditAnywhere)
	float Scale;

	UPROPERTY(EditAnywhere)
	FVector Origin;

	UPROPERTY(EditAnywhere)
	FVector ChunkGrid;

	UPROPERTY(EditAnywhere)
	int ChunkResolution;

	UPROPERTY(EditAnywhere, Replicated)
	TArray<AVObject*> Chunks;

	int GetID() { return ID; }

	void SetID(int newID) { ID = newID; }

	void GenerateGrid();

	void CalculateVoxels();

	UFUNCTION(NetMulticast, reliable, WithValidation)
	void EditPointInGrid(int x, int y, int z, bool activatePoint);
	void EditPointInGrid_Implementation(int x, int y, int z, bool activatePoint);
	bool EditPointInGrid_Validate(int x, int y, int z, bool activatePoint);

	UFUNCTION()
	void EditRadiusInGrid(int x, int y, int z, int radius, bool activatePoint);

	UFUNCTION()
	void EditWholeVoxelAndSurroundingInGrid(int x, int y, int z, bool activatePoint);

	AVObject* GetChunkFromVoxelCoord(int x, int y, int z);

	void SaveGrid();
	void LoadGrid();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

private:

	UPROPERTY(Replicated)
	int ID;

	int GetChunkIndex(int x, int y, int z);
};
