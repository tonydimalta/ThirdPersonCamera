#pragma once

#include "ThirdPersonCamera.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Interface.h"
#include "PlayerCharacterInterface.generated.h"

/** Interface for actors that may receive player input. */
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UPlayerCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

class IPlayerCharacterInterface
{
	GENERATED_BODY()

public:
	virtual bool GotMovementInput() const = 0;
};
