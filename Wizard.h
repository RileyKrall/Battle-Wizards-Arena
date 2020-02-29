// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpellParent.h"
#include "ResistanceVector.h"
#include "VoxelManager.h"
#include "WizardPlayerController.h"
//#include "WizardBattleProjectile.h"	//TEMPORARY. NEED TO REMOVE
#include "Wizard.generated.h"



UCLASS()
class WIZARDBATTLE_API AWizard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWizard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//
	//SPELL STORAGE AND MANAGEMENT
	//
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* MasterSpellList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TArray<ASpellParent*> MemorizedSpells;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TArray<TSubclassOf<class ASpellParent>> MemorizedSpellClasses;

	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TArray<FName> KnownSpellList;
	*/
	UFUNCTION(BlueprintCallable, Server, WithValidation, unreliable)
	void SpellSetup();
	void SpellSetup_Implementation();
	bool SpellSetup_Validate();
	
	/*
	UFUNCTION(BlueprintCallable, Server, WithValidation, unreliable)
	void GetSpellFromDataTable(FName SpellName);
	void GetSpellFromDataTable_Implementation(FName SpellName);
	bool GetSpellFromDataTable_Validate(FName SpellName);
	*/

	UFUNCTION(BlueprintCallable, Server, WithValidation, unreliable)
	void SelectSlot1();
	void SelectSlot1_Implementation();
	bool SelectSlot1_Validate();

	UFUNCTION(BlueprintCallable, Server, WithValidation, unreliable)
	void SelectSlot2();
	void SelectSlot2_Implementation();
	bool SelectSlot2_Validate();

	UFUNCTION(BlueprintCallable, Server, WithValidation, unreliable)
	void SelectSlot3();
	void SelectSlot3_Implementation();
	bool SelectSlot3_Validate();

	UFUNCTION(BlueprintCallable, Server, WithValidation, unreliable)
	void SelectSlot4();
	void SelectSlot4_Implementation();
	bool SelectSlot4_Validate();

	UFUNCTION(BlueprintCallable, Server, WithValidation, unreliable)
	void SelectSlot5();
	void SelectSlot5_Implementation();
	bool SelectSlot5_Validate();

	//
	//SPELL CASTING
	//

	UFUNCTION(Server, WithValidation, unreliable)
	void CastBegin();
	void CastBegin_Implementation();
	bool CastBegin_Validate();

	UFUNCTION(Server, WithValidation, unreliable)
	void CastRelease();
	void CastRelease_Implementation();
	bool CastRelease_Validate();

	//
	//STAT MANAGEMENT
	//

	UFUNCTION(Server, WithValidation, unreliable)
	void Damage(USpellDamageInstance* Damage);
	void Damage_Implementation(USpellDamageInstance* Damage);
	bool Damage_Validate(USpellDamageInstance* Damage);

	UFUNCTION(Server, WithValidation, unreliable)
	void Heal(float heal);
	void Heal_Implementation(float heal);
	bool Heal_Validate(float heal);

	UFUNCTION(BlueprintCallable, Server, WithValidation, unreliable)
	void ReduceResource(float cost);
	void ReduceResource_Implementation(float cost);
	bool ReduceResource_Validate(float cost);


	UFUNCTION(BlueprintCallable, NetMulticast, WithValidation, reliable)
	void KillPlayer();
	void KillPlayer_Implementation();
	bool KillPlayer_Validate();

	void PlayerDeath();


	//
	//MOVEMENT
	//

	void MoveForward(float value);

	void MoveRight(float value);

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	UFUNCTION(Server, WithValidation, unreliable)
	void blinkRight();
	void blinkRight_Implementation();
	bool blinkRight_Validate();

	UFUNCTION(Server, WithValidation, unreliable)
	void blinkForward();
	void blinkForward_Implementation();
	bool blinkForward_Validate();

	UFUNCTION(Server, WithValidation, unreliable)
	void blinkLeft();
	void blinkLeft_Implementation();
	bool blinkLeft_Validate();

	UFUNCTION(Server, WithValidation, unreliable)
	void blinkBack();
	void blinkBack_Implementation();
	bool blinkBack_Validate();

	UFUNCTION(Server, WithValidation, unreliable)
	void StartCooldown();
	void StartCooldown_Implementation();
	bool StartCooldown_Validate();

	UFUNCTION(Server, WithValidation, unreliable)
	void SetOffCooldown();
	void SetOffCooldown_Implementation();
	bool SetOffCooldown_Validate();

	//
	//MAP INTERACTION
	//

	bool bVoxelEdit = false;
	bool bVoxelTypeEdit = false;

	UFUNCTION()
	void toggleEdit();

	UFUNCTION()
	void toggleTypeEdit();

	UPROPERTY()
	class AVoxelManager* VoxMan;

	UFUNCTION(Server, WithValidation, unreliable)
	void InteractWith();
	void InteractWith_Implementation();
	bool InteractWith_Validate();

	UFUNCTION()
	void DestroyVoxel();

	UFUNCTION(Server, WithValidation, reliable)
	void CallVoxelChangeToServer(int x, int y, int z, int gridID, bool change);
	void CallVoxelChangeToServer_Implementation(int x, int y, int z, int gridID, bool change);
	bool CallVoxelChangeToServer_Validate(int x, int y, int z, int gridID, bool change);

	UFUNCTION(NetMulticast, WithValidation, reliable)
	void EditVoxel(int x, int y, int z, int gridID, bool change);
	void EditVoxel_Implementation(int x, int y, int z, int gridID, bool change);
	bool EditVoxel_Validate(int x, int y, int z, int gridID, bool change);

	UFUNCTION()
	void SetSpellDataTable(UDataTable* input);

	//
	//CONSOLE COMMANDS
	//
	/*
	UFUNCTION(Exec)
	void AddSpellToKnownSpells(FName SpellName);

	UFUNCTION(Exec)
	void AddAllSpellsToKnownList();
	*/
	UFUNCTION(Exec)
	void AddResource(float input);
	

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	//
	//MISCELLANEOUS PROPERTIES (TO BE SORTED)
	//

	UPROPERTY(Replicated)
	FVector SpellSource;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* Mesh1P;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ASpellParent> SpellClass;

	UPROPERTY(replicated)
	float Health;

	UPROPERTY(replicated)
	float Resource;

	UPROPERTY(replicated)
	bool CanBlink;

	UPROPERTY()
	float BlinkCooldown;

	UPROPERTY(replicated)
	UResistanceVector* BaseResistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	class ASpellParent* Spell;

	UPROPERTY(BlueprintReadOnly, Replicated)
	int CurrentSpellSlot;

	UFUNCTION(BlueprintCallable)
	ASpellParent* GetSpell();

	UFUNCTION(BlueprintCallable)
	float GetHealth();

	UFUNCTION(BlueprintCallable)
	float GetResource();


};
