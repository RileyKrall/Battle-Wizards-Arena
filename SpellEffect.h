// Old implementation of what happens when a spellParent gets casted. May remove if prove unneeded.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpellEffect.generated.h"

UCLASS()
class WIZARDBATTLE_API ASpellEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpellEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
