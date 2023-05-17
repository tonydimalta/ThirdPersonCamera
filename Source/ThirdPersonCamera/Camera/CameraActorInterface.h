#pragma once

#include "ThirdPersonCamera.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Interface.h"
#include "CameraActorInterface.generated.h"

class AHoatCameraModificationVolume;

/** Interface for actors that may enter camera modification volumes. */
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UCameraActorInterface : public UInterface
{
	GENERATED_BODY()
};

class ICameraActorInterface
{
	GENERATED_BODY()

public:
	virtual AHoatCameraModificationVolume* GetCurrentCameraModificationVolume() const = 0;
	virtual void SetCurrentCameraModificationVolume(AHoatCameraModificationVolume* CurrentCameraModificationVolume) = 0;
};
