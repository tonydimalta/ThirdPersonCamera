#include "ThirdPersonCameraGameMode.h"
#include "ThirdPersonCameraCharacter.h"
#include "GameFramework/GameUserSettings.h"
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

void AThirdPersonCameraGameMode::BeginPlay()
{
	GEngine->bSmoothFrameRate = true;
	GEngine->bUseFixedFrameRate = false;

	constexpr float minRange = 20.0f;
	constexpr float maxRange = 250.0f;

	GEngine->SmoothedFrameRateRange = FFloatRange(minRange, maxRange);

	const auto settings = GEngine->GetGameUserSettings();

	settings->SetVSyncEnabled(true);
	settings->SetFrameRateLimit(0);
	settings->ApplyNonResolutionSettings();
}
