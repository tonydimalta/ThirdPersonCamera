#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActorInterface.h"
#include "Camera/PlayerCharacterInterface.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "ThirdPersonCameraCharacter.generated.h"

class AHoatCameraModificationVolume;
class UCameraComponent;
class UInputAction;
class UInputComponent;
class UInputMappingContext;
class USpringArmComponent;

UCLASS(config=Game)
class AThirdPersonCameraCharacter : public ACharacter, public ICameraActorInterface, public IPlayerCharacterInterface
{
	GENERATED_BODY()

public:
	AThirdPersonCameraCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual AHoatCameraModificationVolume* GetCurrentCameraModificationVolume() const override;
	virtual void SetCurrentCameraModificationVolume(AHoatCameraModificationVolume* InCurrentCameraModificationVolume) override;
	virtual bool GotMovementInput() const override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const;
	/** Returns FollowCamera subobject **/
	FORCEINLINE UCameraComponent* GetFollowCamera() const;

	/** @return true if this character is currently able to sprint (and is not currently sprinting) */
	UFUNCTION(BlueprintCallable, Category = Character)
	bool CanSprint() const;

	/** Set by character movement to specify that this Character is currently sprinting. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_IsSprinting, Category = Character)
	bool bIsSprinting{ false };

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

private:
	/** Handle bIsSprinting replicated from server */
	UFUNCTION()
	virtual void OnRep_IsSprinting();

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	//Sets Character Movement Speed to Sprint values.
	void Sprint();

	//Sets Character Movement Speed back to default (Walk) values.
	void StopSprinting();

	//Request for Character to Crouch
	void RequestCrouch();

	//Request for Character to UnCrouch
	void RequestUnCrouch();

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	/** Input Mapping Context */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SprintAction;

	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> CrouchAction;

	TObjectPtr<AHoatCameraModificationVolume> CurrentCameraModificationVolume;
};
