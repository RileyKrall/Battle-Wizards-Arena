// UInterface for actors that will need to activate a cooldown or timer.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TimedObject.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTimedObject : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WIZARDBATTLE_API ITimedObject
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void BeginTimerInitialization(float Duration, bool NeedsTimer);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ReduceTimeRemaining(float reduction);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ResetTimer();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	float GetTimeDuration();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	float GetTimeRemaining();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	bool GetNeedsTimeReduction();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetTimeDuration(float NewDuration);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetTimeRemaining(float NewTimeRemaining);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetNeedsTimeReduction(bool IfNeedsTimeReduction);


};
