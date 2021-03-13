// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	doorCloseDelay = 2.f;
	doorLastOpened = 0.f;
	doorOpenSpeed = 40;
	doorCloseSpeed = 40;
}

void UOpenDoor::OpenDoor(float DeltaTime) 
{
	// 	UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString());
	// UE_LOG(LogTemp, Warning, TEXT("Yaw is: %f"), GetOwner()->GetActorRotation().Yaw);

	currentYaw = FMath::FInterpConstantTo(currentYaw, targetYaw, DeltaTime, doorOpenSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = currentYaw;
	// Set Actor Rotation
	GetOwner()->SetActorRotation(DoorRotation);

	// float currentYaw = GetOwner()->GetActorRotation().Yaw;
	// FRotator openDoor(0.f, targetYaw, 0.f);
	// // openDoor.Yaw = FMath::Lerp(currentYaw, targetYaw, DeltaTime * .002f);
	// openDoor.Yaw = FMath::FInterpConstantTo(currentYaw, targetYaw, DeltaTime, 40);
	// // openDoor.Yaw = FMath::FInterpTo(currentYaw, targetYaw, DeltaTime, 2);
	// GetOwner()->SetActorRotation(openDoor);
}

void UOpenDoor::CloseDoor(float DeltaTime) 
{
	currentYaw = FMath::FInterpConstantTo(currentYaw, initialYaw, DeltaTime, doorCloseSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = currentYaw;
	// Set Actor Rotation
	GetOwner()->SetActorRotation(DoorRotation);
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	initialYaw = GetOwner()->GetActorRotation().Yaw;
	currentYaw = initialYaw;
	targetYaw += initialYaw;

	if(!PressurePlate)
	{
		// ActorName has the opendoor component on it but no pressureplate set.
		UE_LOG(LogTemp, Error, TEXT("%s has the opendoor component on it but no pressureplate set."), *GetOwner()->GetName())
	}

	doorUser = GetWorld()->GetFirstPlayerController()->GetPawn();

}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(PressurePlate && PressurePlate->IsOverlappingActor(doorUser))
	{
		OpenDoor(DeltaTime);

		// Time door was opened
		doorLastOpened = GetWorld()->GetTimeSeconds();
	} else 
	{
		
		// If the door has been open longer than x seconds
		if((GetWorld()->GetTimeSeconds() - doorLastOpened) > doorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}
}
