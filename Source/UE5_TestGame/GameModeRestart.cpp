// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeRestart.h"
#include "Engine/Console.h"

void AGameModeRestart::RestartGame()
{
	GEngine->Exec(GetWorld(), TEXT("RestartLevel"));
}

