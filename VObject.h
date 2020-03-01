// Chunk of the voxel system in a VGrid. The chunk deals with surface computations, generation, and optimizations.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Engine.h"
#include "VObject.generated.h"

USTRUCT()
struct FNestedArray {
	GENERATED_USTRUCT_BODY();

	FNestedArray() {}

	FNestedArray(TArray<int32> Triangles) {
		TriangleList = Triangles;
	}

	UPROPERTY(EditAnywhere)
		TArray<int32> TriangleList;
};

USTRUCT()
struct FVoxel {
	GENERATED_USTRUCT_BODY();

	uint8 block_type;
	uint8 shape_index;
	bool is_active;

};

UCLASS()
class WIZARDBATTLE_API AVObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	UProceduralMeshComponent* Mesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SetParameters(float Resolution, FVector Origin, float Scale, float SurfaceLevel, int VGridID);

	UFUNCTION()
	void CalcSurface();

	UFUNCTION()
	void GenerateSurface();

	UFUNCTION()
	FVector GetVoxelSurfacePoint(float x, float y, float z, bool openPoint);

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void editVoxelVertices(int x, int y, int z, uint8 edit, bool activate);
	void editVoxelVertices_Implementation(int x, int y, int z, uint8 edit, bool activate);
	bool editVoxelVertices_Validate(int x, int y, int z, uint8 edit, bool activate);

	UFUNCTION()
	int getVGridID() { return GridID; }

	FLinearColor getColor(uint8 block_type);

private:

	int GridID;

	float CSurfaceLevel;
	
	float CResolution;
	
	FVector COrigin;

	float CScale;

	UPROPERTY()
	TArray<FVector> EdgeVertexPoint;

	UPROPERTY()
	TArray<FNestedArray> edgeMap;

	UPROPERTY()
	TArray<FVoxel> VoxelVertexData;

	UFUNCTION()
	bool PointCovered(float x, float y, float z);

	UFUNCTION()
	void SingleVoxel_EditSurrounding(int x, int y, int z, uint8 bits);

	UFUNCTION()
	void SingleVoxel_Isolated(int x, int y, int z, uint8 bits);

	UFUNCTION()
	int getArrayIndex(int x, int y, int z);

	FVector CalcFaceNormal(FVector a, FVector b, FVector c);

};
