#pragma once

#include "ThirdPersonCamera.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Interface.h"
#include "CombatActorInterface.generated.h"

/** Interface for actors that may participate in combat. */
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UCombatActorInterface : public UInterface
{
	GENERATED_BODY()
};

class ICombatActorInterface
{
	GENERATED_BODY()

public:
	virtual bool IsInCombat() const = 0;
};
