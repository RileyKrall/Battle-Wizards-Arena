// Actor that represents the player and its input. Keeps track of spells and other important player information
// that occurs in a game. Will also call gamemode to respawn player when necessary.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Engine.h"
#include "SpellParent.h"
#include "VoxelManager.h"
#include "WizardPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class WIZARDBATTLE_API AWizardPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	AWizardPlayerController();

	virtual void BeginPlay() override;

	AVoxelManager* VoxMan;

	UFUNCTION(BlueprintCallable, Server, WithValidation, reliable)
	void RequestRespawn();
	void RequestRespawn_Implementation();
	bool RequestRespawn_Validate();

	//
	//	SPELL MANAGEMENT
	//

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* MasterSpellList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TArray<FName> KnownSpellList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TArray<TSubclassOf<class ASpellParent>> MemorizedSpellClasses;

	
	UFUNCTION(BlueprintCallable, Server, WithValidation, reliable)
	void ManageSpells(const TArray<FName>& KnownSpells, const TArray<TSubclassOf<class ASpellParent>>& MemSpellClasses);
	void ManageSpells_Implementation(const TArray<FName>& KnownSpells, const TArray<TSubclassOf<class ASpellParent>>& MemSpellClasses);
	bool ManageSpells_Validate(const TArray<FName>& KnownSpells, const TArray<TSubclassOf<class ASpellParent>>& MemSpellClasses);

	UFUNCTION(BlueprintCallable, Server, WithValidation, reliable)
	void SendMemorizedSpells();
	void SendMemorizedSpells_Implementation();
	bool SendMemorizedSpells_Validate();

	UFUNCTION(BlueprintCallable, Server, WithValidation, unreliable)
	void ChangeSpellInSlot(int slot, TSubclassOf<class ASpellParent> NewSpellClass);
	void ChangeSpellInSlot_Implementation(int slot, TSubclassOf<class ASpellParent> NewSpellCLass);
	bool ChangeSpellInSlot_Validate(int slot, TSubclassOf<class ASpellParent> NewSpellCLass);

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	UFUNCTION(Exec)
	void AddSpellToKnownSpells(FName SpellName);

	UFUNCTION(Exec)
	void AddAllSpellsToKnownList();
};
