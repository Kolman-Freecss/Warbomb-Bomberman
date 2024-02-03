// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoreGameGameMode.h"
#include "CoreGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACoreGameGameMode::ACoreGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
