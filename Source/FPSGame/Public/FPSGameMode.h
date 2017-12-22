// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameMode.generated.h"

UCLASS()
class AFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, Category = "Spectator")
	TSubclassOf<AActor> SpectatingViewPointClass;

public:

	AFPSGameMode();
	void CompleteMission(APawn* InstigatorPawn);
	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void OnMissionCompleted(APawn* InstigatorPawn);
<<<<<<< HEAD
	//
=======
>>>>>>> 10e6eb3000b9af3836b9cf22644719fa221325e6
};




