// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAiGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "FPSGameMode.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AFPSAiGuard::AFPSAiGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	GuardState = EAIState::idle;

}

void AFPSAiGuard::GetLifetimeReplicatedProps(TArray < FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFPSAiGuard, GuardState);
	
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
	AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->CompleteMission(SeenPawn, false);
	}
	SetGuardState(EAIState::Alert);
}

void AFPSAiGuard::OnNoiseHeard(APawn* PawnInstigator, const FVector& Location, float Volume)
{
	if (GuardState != EAIState::Alert)
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
	
		SetGuardState(EAIState::Suspicious);
	}


}

void AFPSAiGuard::ResetOrientation()
{
	if (GuardState == EAIState::Alert)
		return;
	SetActorRotation(OriginalRotation);
	SetGuardState(EAIState::idle);
}

void AFPSAiGuard::SetGuardState(EAIState Newstate)
{
	if (GuardState == Newstate)
		return;
	GuardState = Newstate;
	if(Role==ROLE_Authority)
		OnRep_GuardState(); // In case we are server. 
	
}

// Called every frame
void AFPSAiGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFPSAiGuard::OnRep_GuardState()
{
	// This will only be called on client.
	OnStateChanged(GuardState);
}





