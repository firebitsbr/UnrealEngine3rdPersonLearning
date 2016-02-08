// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "ThridPersonLearning.h"
#include "ThridPersonLearningGameMode.h"
#include "ThridPersonLearningCharacter.h"
#include "Kismet/GameplayStatics.h"

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


void AThridPersonLearningGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UE_LOG(LogClass, Log, TEXT("GameModeTick %f"), DeltaTime);
	AThridPersonLearningCharacter* MyCharacter = Cast<AThridPersonLearningCharacter>(UGameplayStatics::GetPlayerPawn(this, 9));
	if(MyCharacter && MyCharacter->GetCurrentPowerLevel() < 0.0f)
	{
		MyCharacter->UpdatePower(-(DeltaTime*DecayRate*(MyCharacter->GetInitialPowerLevel())));
	}
}
