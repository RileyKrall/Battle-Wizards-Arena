// Not fully implemented gamemode. Currently allows the player to respawn after death, and holds pointer to voxel manager
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/GameMode.h"
#include "VoxelManager.h"
#include "FFAGM.generated.h"

/**
 * 
 */
UCLASS()
class WIZARDBATTLE_API AFFAGM : public AGameMode
{
	GENERATED_BODY()
	
public:

	AFFAGM();

	virtual void BeginPlay() override;

	bool voxelsActive;

	class AVoxelManager* VoxMan;

	UFUNCTION(BlueprintCallable)
	void RespawnPlayer(APlayerController* PC);

};
