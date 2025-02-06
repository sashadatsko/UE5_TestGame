// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeRestart.generated.h"

/**
 * 
 */
UCLASS()
class UE5_TESTGAME_API AGameModeRestart : public AGameModeBase
{
	GENERATED_BODY()
public:
	void RestartGame();
};
