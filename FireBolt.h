// One of the first spells to be cast by a wizard. When casted, it spawns a projectile

#pragma once

#include "CoreMinimal.h"
#include "SpellParent.h"
#include "SpellDamageInstance.h"
#include "FireBolt.generated.h"

/**
 * 
 */
UCLASS()
class WIZARDBATTLE_API AFireBolt : public ASpellParent
{
	GENERATED_BODY()
	
public:

	AFireBolt();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void BeginSpellCast(FRotator spawnRotation, FVector spawnLocation, FVector forwardVector, float AvailableResource) override;

	virtual void CastSpell_Implementation(FRotator spawnRotation, FVector spawnLocation, FVector forwardVector) override;
	virtual bool CastSpell_Validate(FRotator spawnRotation, FVector spawnLocation, FVector forwardVector) override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	UFUNCTION(Server, WithValidation, unreliable)
	void StartCooldown();
	void StartCooldown_Implementation();
	bool StartCooldown_Validate();

	UFUNCTION(Server, WithValidation, unreliable)
	void SetOffCooldown();
	void SetOffCooldown_Implementation();
	bool SetOffCooldown_Validate();

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AWizardBattleProjectile> bolt;
};
