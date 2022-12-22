#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "PlayerTeam.generated.h"

enum ETeamID : uint8
{
    PlayersTeam
};

USTRUCT()
struct EDUCATIONALPROJECT_API FPlayerTeamID : public FGenericTeamId
{
    GENERATED_USTRUCT_BODY()

    FPlayerTeamID() : FGenericTeamId(PlayersTeam) {}
};