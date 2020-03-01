// One of the first spells, heals another player when casted

#pragma once

#include "CoreMinimal.h"
#include "SpellParent.h"
#include "SpellDamageInstance.h"
#include "Wizard.h"
#include "MinorHeal.generated.h"

/**
 * 
 */
UCLASS()
class WIZARDBATTLE_API AMinorHeal : public ASpellParent
{
	GENERATED_BODY()
	
public:

	AMinorHeal();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void CastSpell_Implementation(FRotator spawnRotation, FVector spawnLocation, FVector forwardVector) override;
	virtual bool CastSpell_Validate(FRotator spawnRotation, FVector spawnLocation, FVector forwardVector) override;
	
	AActor* LineTraceActor(FRotator spawnRotation, FVector spawnLocation, FVector forwardVector);

	virtual void BeginSpellCast(FRotator spawnRotation, FVector spawnLocation, FVector forwardVector, float AvailableResource) override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	UPROPERTY(Replicated)
	AActor* Target;
};

