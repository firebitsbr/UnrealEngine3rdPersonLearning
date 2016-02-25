// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "ThridPersonLearning.h"
#include "ThridPersonLearningGameMode.h"
#include "ThridPersonLearningCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "SpawnVolume.h"

AThridPersonLearningGameMode::AThridPersonLearningGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	DecayRate = 0.01f;
}

void AThridPersonLearningGameMode::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundActors);
	for(auto Actor: FoundActors)
	{
		ASpawnVolume* SpawnVolumeActor = Cast<ASpawnVolume>(Actor);
		if(SpawnVolumeActor)
		{
			SpawnVolumeActors.AddUnique(SpawnVolumeActor);
		}
	}
	SetCurrentState(EBatteryPlayState::EPlaying);
	AThridPersonLearningCharacter* MyCharacter = Cast<AThridPersonLearningCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if(MyCharacter)
	{
		PowerToWin = (MyCharacter->GetInitialPowerLevel()) * 1.25f;
	}

	if(HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if(CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}


}


void AThridPersonLearningGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UE_LOG(LogClass, Log, TEXT("GameModeTick %f"), DeltaTime);
	//UE_LOG(LogClass, Log, TEXT("GameModeTick %f"), DeltaTime);
	AThridPersonLearningCharacter* MyCharacter = Cast<AThridPersonLearningCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if(MyCharacter)
	{
		if(MyCharacter->GetCurrentPowerLevel() > PowerToWin)
		{
			SetCurrentState(EBatteryPlayState::EWon);
		}
		else if(MyCharacter->GetCurrentPowerLevel() > 0.0f)
		{
			//UE_LOG(LogClass, Log, TEXT("Character Powerl level %f"), MyCharacter->GetCurrentPowerLevel());
			MyCharacter->UpdatePower(-DeltaTime*DecayRate*(MyCharacter->GetInitialPowerLevel()));
		}
		else
		{
			SetCurrentState(EBatteryPlayState::EGameOver);
		}
	}
}

float AThridPersonLearningGameMode::GetPowerToWin() const
{
	return PowerToWin;
}

EBatteryPlayState AThridPersonLearningGameMode::GetCurrentState() const
{
	return CurrentState;
}

void AThridPersonLearningGameMode::SetCurrentState(EBatteryPlayState ps)
{
	CurrentState = ps;
	AThridPersonLearningGameMode::HandleNewState(ps);
}

void AThridPersonLearningGameMode::HandleNewState(EBatteryPlayState NewState)
{
	switch (NewState) {
		case EBatteryPlayState::EPlaying:
		{
			for(ASpawnVolume* Volumn: SpawnVolumeActors)
			{
				Volumn->SetSpawningActive(true);
			}
		}
			break;
		case EBatteryPlayState::EWon:
		{
			for(ASpawnVolume* Volumn: SpawnVolumeActors)
			{
				Volumn->SetSpawningActive(false);
			}
		}
			break;
		case EBatteryPlayState::EGameOver:
		{
			for(ASpawnVolume* Volumn: SpawnVolumeActors)
			{
				Volumn->SetSpawningActive(false);
			}
			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
			if(PlayerController)
			{
				PlayerController->SetCinematicMode(true, false, false, true, true);
			}
			ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
			if(MyCharacter)
			{
				MyCharacter->GetMesh()->SetSimulatePhysics(true);
				MyCharacter->GetMovementComponent()->MovementState.bCanJump = false;
			}
		}
			break;
		case EBatteryPlayState::EUnknown:
		{

		}
			break;
		default:
			break;
	}
}
