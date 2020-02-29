// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "UnrealNetwork.h"
#include "Online.h"
#include "Engine/GameInstance.h"
#include "WizardBattleGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class WIZARDBATTLE_API UWizardBattleGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UWizardBattleGameInstance(const FObjectInitializer& ObjectInitializer);

	/*HOSTING SESSIONS*/

	bool HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);

	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;

	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;

	FDelegateHandle OnCreateSessionCompleteDelegateHandle;

	FDelegateHandle OnStartSessionCompleteDelegateHandle;

	TSharedPtr<class FOnlineSessionSettings> SessionSettings;

	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);

	/*FINDING SESSIONS*/

	void FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence);

	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;

	FDelegateHandle OnFindSessionsCompleteDelegateHandle;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	void OnFindSessionsComplete(bool bWasSuccessful);

	/*JOINING SESSIONS*/

	bool JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);

	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

	FDelegateHandle OnJoinSessionCompleteDelegateHandle;

	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	/*DESTROYING SESSIONS*/

	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;

	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

	virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	/*BLUEPRINT CALLABLE FUNCTIONS*/

	UFUNCTION(BlueprintCallable)
	void StartOnlineGame();

	UFUNCTION(BlueprintCallable)
	void FindOnlineGames();

	UFUNCTION(BlueprintCallable)
	void JoinOnlineGame();

	UFUNCTION(BlueprintCallable)
	void DestroySessionAndLeaveGame();

	/*PLAYER LOADOUT MANAGEMENT*/

};
