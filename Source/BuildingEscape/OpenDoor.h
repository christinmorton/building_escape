// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UOpenDoor();

	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

private:
	float initialYaw;
	float currentYaw;
	float doorLastOpened;

	UPROPERTY(EditAnywhere)
	float doorCloseDelay;

	UPROPERTY(EditAnywhere)
	int32 doorOpenSpeed;

	UPROPERTY(EditAnywhere)
	int32 doorCloseSpeed;

	UPROPERTY(EditAnywhere)
	float targetYaw = 90.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	AActor* doorUser = nullptr;
};
