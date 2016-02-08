// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "ThridPersonLearningGameMode.generated.h"

UCLASS(minimalapi)
class AThridPersonLearningGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AThridPersonLearningGameMode();

	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power")
	float DecayRate;
};



