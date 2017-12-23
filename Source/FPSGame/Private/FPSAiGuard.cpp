// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAiGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"

// Sets default values
AFPSAiGuard::AFPSAiGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	

}

// Called when the game starts or when spawned
void AFPSAiGuard::BeginPlay()
{
	Super::BeginPlay();
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAiGuard::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAiGuard::OnNoiseHeard);
	OriginalRotation = GetActorRotation();

	
}

void AFPSAiGuard::OnPawnSeen(APawn * SeenPawn)
{
	if (SeenPawn == nullptr)
		return;
	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Yellow, false, 10);
}

void AFPSAiGuard::OnNoiseHeard(APawn* PawnInstigator, const FVector& Location, float Volume)
{
	DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Cyan, false, 10);
	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();
	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0.0f;
	NewLookAt.Roll = 0.0f;
	SetActorRotation(NewLookAt);
	
	GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AFPSAiGuard::ResetOrientation, 3.0f);
}

void AFPSAiGuard::ResetOrientation()
{
	SetActorRotation(OriginalRotation);
}

// Called every frame
void AFPSAiGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}





