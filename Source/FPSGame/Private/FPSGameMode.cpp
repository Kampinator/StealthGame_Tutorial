// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn* InstigatorPawn, bool bMissionSuccess)
{
	if (InstigatorPawn)
	{
		InstigatorPawn->DisableInput(nullptr);
		OnMissionCompleted(InstigatorPawn, bMissionSuccess);
		
		TArray<AActor*> OutActors;
		if (SpectatingViewPointClass)
		{
			UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewPointClass, OutActors);
			// Change viewtarget if any valid actor found
			if (OutActors.Num() > 0)
			{
				AActor* NewViewTarget = OutActors[0];
				APlayerController* PCC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
				if (PCC)
				{
					PCC->SetViewTargetWithBlend(NewViewTarget, 0.5f, EViewTargetBlendFunction::VTBlend_Linear);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("LogTemp, Warning, SpectatingViewpointClass is nullptr"));
		}
	}
}
