// Fill out your copyright notice in the Description page of Project Settings.


#include "WizardBattleGameInstance.h"
#include "Engine.h"
#include "UnrealNetwork.h"
#include "Online.h"

UWizardBattleGameInstance::UWizardBattleGameInstance(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer) {
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UWizardBattleGameInstance::OnCreateSessionComplete);

	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &UWizardBattleGameInstance::OnStartOnlineGameComplete);

	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UWizardBattleGameInstance::OnFindSessionsComplete);

	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UWizardBattleGameInstance::OnDestroySessionComplete);
}

bool UWizardBattleGameInstance::HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers) {
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub) {
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid()) {
			/*This is where Session settings are applied*/

			SessionSettings = MakeShareable(new FOnlineSessionSettings());
			
			SessionSettings->bIsLANMatch = bIsLAN;
			SessionSettings->bUsesPresence = bIsPresence;
			SessionSettings->NumPublicConnections = MaxNumPlayers;
			SessionSettings->NumPrivateConnections = 0;
			SessionSettings->bAllowInvites = true;
			SessionSettings->bAllowJoinInProgress = true;
			SessionSettings->bShouldAdvertise = true;
			SessionSettings->bAllowJoinViaPresence = true;
			SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;

			SessionSettings->Set(SETTING_MAPNAME, FString("NewMap"), EOnlineDataAdvertisementType::ViaOnlineService);

			OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

			return Sessions->CreateSession(*UserId, SessionName, *SessionSettings);
		}
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("No OnlineSubSystem"));
	}
	return false;
}

void UWizardBattleGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful) {
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnCreateSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));
	
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	
	if (OnlineSub) {
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid()) {
			Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
			if (bWasSuccessful) {
				OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);

				Sessions->StartSession(SessionName);
			}
		}
	}
}

void UWizardBattleGameInstance::OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful) {
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnStartSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub) {
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid()) {
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		}
	}

	if (bWasSuccessful) {

		FString levelOptions = "listen";
		if (SessionSettings->bIsLANMatch) {
			levelOptions.Append("?bIsLanMatch=1");
		}

		UGameplayStatics::OpenLevel(GetWorld(), "GameLobby", true, levelOptions);
	}
}

void UWizardBattleGameInstance::FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence) {
	
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	
	if (OnlineSub) {

		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid()) {
			/*Search Settings*/

			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			SessionSearch->bIsLanQuery = bIsLAN;
			SessionSearch->MaxSearchResults = 200000;
			SessionSearch->PingBucketSize = 50;

			if (bIsPresence) {
				SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, bIsPresence, EOnlineComparisonOp::Equals);
			}

			TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SessionSearch.ToSharedRef();

			OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

			Sessions->FindSessions(*UserId, SearchSettingsRef);
		}

	}
	else {
		OnFindSessionsComplete(false);
	}
}

void UWizardBattleGameInstance::OnFindSessionsComplete(bool bWasSuccessful) {

	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OFindSessionsComplete bSuccess: %d"), bWasSuccessful));

	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub) {
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid()) {
			
			Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Num Search Results: %d"), SessionSearch->SearchResults.Num()));

			if (SessionSearch->SearchResults.Num() > 0) {
				for (int32 SearchIdx = 0; SearchIdx < SessionSearch->SearchResults.Num(); SearchIdx++) {
					GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Session Number: %d | Sessionname: %s "), SearchIdx + 1, *(SessionSearch->SearchResults[SearchIdx].Session.OwningUserName)));
				}
			}

		}
	}
}

bool UWizardBattleGameInstance::JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult) {
	bool bSuccessful = false;

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub) {
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid()) {

			OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

			bSuccessful = Sessions->JoinSession(*UserId, SessionName, SearchResult);
		}
	}
	return bSuccessful;
}

void UWizardBattleGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result) {
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnJoinSessionComplete %s, %d"), *SessionName.ToString(), static_cast<int32>(Result)));

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub) {
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid()) {
			Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

			APlayerController* const PlayerController = GetFirstLocalPlayerController();

			FString TravelURL;

			if (PlayerController && Sessions->GetResolvedConnectString(SessionName, TravelURL)) {
				PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
			}
		}
	}
}

void UWizardBattleGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful) {

	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnDestroySessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub) {
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid()) {
			Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);

			if (bWasSuccessful) {

				UGameplayStatics::OpenLevel(GetWorld(), "MainMenu", true);
			}
		}
	}
}

void UWizardBattleGameInstance::StartOnlineGame() {

	ULocalPlayer* const Player = GetFirstGamePlayer();

	HostSession(Player->GetPreferredUniqueNetId().GetUniqueNetId(), GameSessionName, true, true, 4);
}

void UWizardBattleGameInstance::FindOnlineGames() {

	ULocalPlayer* const Player = GetFirstGamePlayer();

	FindSessions(Player->GetPreferredUniqueNetId().GetUniqueNetId(), true, true);

}

void UWizardBattleGameInstance::JoinOnlineGame() {

	ULocalPlayer* const Player = GetFirstGamePlayer();

	FOnlineSessionSearchResult SearchResult;

	if (SessionSearch->SearchResults.Num() > 0) {
		for (int32 i = 0; i < SessionSearch->SearchResults.Num(); i++) {
			if (SessionSearch->SearchResults[i].Session.OwningUserId != Player->GetPreferredUniqueNetId().GetUniqueNetId()) {
				SearchResult = SessionSearch->SearchResults[i];

				JoinSession(Player->GetPreferredUniqueNetId().GetUniqueNetId(), GameSessionName, SearchResult);
				break;
			}
		}
	}
}

void UWizardBattleGameInstance::DestroySessionAndLeaveGame() {

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub) {
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid()) {
			Sessions->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);

			Sessions->DestroySession(GameSessionName);
		}
	}
}