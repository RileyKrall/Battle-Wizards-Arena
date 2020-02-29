// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VoxelManager.h"
#include "WizardBattleProjectile.generated.h"

UCLASS(config=Game)
class AWizardBattleProjectile : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	class USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

public:
	AWizardBattleProjectile();

	UPROPERTY()
	class USpellDamageInstance* Damage;

	AVoxelManager* VoxMan;

	/** called when projectile hits something */
	UFUNCTION(Server, unreliable, WithValidation)
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	void OnHit_Implementation(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	bool OnHit_Validate (UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(NetMulticast, reliable, WithValidation)
	void DestroyVoxel(int x, int y, int z, int gridID, bool change);
	void DestroyVoxel_Implementation(int x, int y, int z, int gridID, bool change);
	bool DestroyVoxel_Validate(int x, int y, int z, int gridID, bool change);

	/** Returns CollisionComp subobject **/
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};

