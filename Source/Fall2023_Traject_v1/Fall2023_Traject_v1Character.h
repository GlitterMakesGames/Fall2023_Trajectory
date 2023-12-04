// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Fall2023_Traject_v1Character.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;

UCLASS(config=Game)
class AFall2023_Traject_v1Character : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USceneComponent* FP_MuzzleLocation;

	/** Gun mesh: VR view (attached to the VR controller directly, no arm, just the actual gun) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* VR_Gun;

	/** Location on VR gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USceneComponent* VR_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Motion controller (right hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UMotionControllerComponent* R_MotionController;

	/** Motion controller (left hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UMotionControllerComponent* L_MotionController;

public:
	AFall2023_Traject_v1Character();

protected:
	virtual void BeginPlay();

public:
	////////-----------PROJECTILE SPEED-----------////////

	UFUNCTION(BlueprintCallable, Category = "Character|Projectile")
	float GetProjectileSpeed() const { return projectileSpeed; }

	UFUNCTION(BlueprintCallable, Category = "Character|Projectile")
	void SetProjectileSpeed(float NewSpeed) { projectileSpeed = NewSpeed; }

	UFUNCTION(BlueprintCallable, Category = "Character|Projectile")
	void PrintProjectileSpeed();

	////////-----------CONTINUE CALCULATION-----------////////

	UFUNCTION(BlueprintCallable, Category = "Character|Projectile")
	void DisableCalculation() { continueCalculation = false; }

	UFUNCTION(BlueprintCallable, Category = "Character|Projectile")
	void EnableCalculation() { continueCalculation = true; }

	////////-----------TOTAL ELAPSED TIME-----------////////

	UFUNCTION(BlueprintCallable, Category = "Character|Projectile")
	float GetTotalElapsedTime() const { return totalElapsedTime; }

	UFUNCTION(BlueprintCallable, Category = "Character|Projectile")
	void SetTotalElapsedTime(float NewTime) { totalElapsedTime = NewTime; }

	////////----------------PRECISION----------------////////

	UFUNCTION(BlueprintCallable, Category = "Character|Projectile")
	int GetPrecision() const { return precision; }

	////////-----------DRAW NEXT LINE-----------////////

	UFUNCTION(BlueprintCallable, Category = "Character|Projectile")
	void DrawNextLine(FVector start);

	UFUNCTION(BlueprintCallable, Category = "Character|Projectile")
	FVector GetLocation() const { return nextStart; }

	UFUNCTION(BlueprintCallable, Category = "Character|Projectile")
	void SetLocation(FVector start) { nextStart = start; }

	////////----------------------------------------////////

	UPROPERTY(BlueprintReadWrite, EditAnywhere)	
	float projectileSpeed = 3000.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool continueCalculation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float totalElapsedTime = 0.0f;

	// This is the number of frames/ticks the program will wait
	// before generating the next raycast.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int precision = 10;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector nextStart;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AFall2023_Traject_v1Projectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	uint8 bUsingMotionControllers : 1;

protected:
	
	/** Fires a projectile. */
	void OnFire();

	/** Resets HMD orientation and position in VR. */
	void OnResetVR();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	struct TouchData
	{
		TouchData() { bIsPressed = false;Location=FVector::ZeroVector;}
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/* 
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so 
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

