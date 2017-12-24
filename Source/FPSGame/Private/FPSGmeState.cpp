// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSGmeState.h"
#include "FPSPlayerController.h"

void AFPSGmeState::MulticastOnMissionComplete_Implementation(APawn* InstigatorPawn, bool bMissionSuccess)
{
	for (FConstPlayerControllerIterator it = GetWorld()->GetPlayerControllerIterator(); it; it++)
	{
		AFPSPlayerController* PC = Cast<AFPSPlayerController>(it->Get());
		if (PC && PC->IsLocalPlayerController())
		{
			PC->OnMissionCompleted(InstigatorPawn, bMissionSuccess);
			APawn* MyPawn = PC->GetPawn();
			if (MyPawn)
			{
				MyPawn->DisableInput(PC);
			}
		}		
	}	
}



