#include "ThirdPersonCameraGameMode.h"
#include "ThirdPersonCameraCharacter.h"
#include "UObject/ConstructorHelpers.h"

AThirdPersonCameraGameMode::AThirdPersonCameraGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
