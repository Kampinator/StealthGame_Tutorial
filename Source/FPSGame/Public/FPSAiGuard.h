// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAiGuard.generated.h"

class UPawnSensingComponent;

UENUM(BlueprintType)
enum class EAIState : uint8
{
	idle,
	Suspicious,
	Alert
};

UCLASS()
class FPSGAME_API AFPSAiGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAiGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UFUNCTION() // Remember UFUNCTION ON DELEGATES!!
	void OnPawnSeen(APawn* SeenPawn);
	UFUNCTION() // Remember UFUNCTION ON DELEGATES!!
	void OnNoiseHeard(APawn* PawnInstigator, const FVector& Location, float Volume);

	FRotator OriginalRotation;

	UFUNCTION() // Remember UFUNCTION ON DELEGATES!!
	void ResetOrientation();

	FTimerHandle TimerHandle_ResetOrientation;

	EAIState GuardState;
	void SetGuardState(EAIState Newstate);
	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnStateChanged(EAIState NewState);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	
	
};
