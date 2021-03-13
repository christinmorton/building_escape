// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// #define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// LineTraceEnd = FVector(0.f, 0.f, 0.f);
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	InitInputComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Get player's viewpoint
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(playerViewPointLocation, playerViewPointRotation);
	FVector LineTraceEnd = playerViewPointLocation + playerViewPointRotation.Vector() * reach;

	// If the physics handle is attached
	if(PhysicsHandle->GrabbedComponent)
	{
		// move the object we are holding.
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

void UGrabber::Grab() 
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber pressed!"))

	possessedItem = GetFirstPhysicsBodyInReach();

	//Get player's viewpoint
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(playerViewPointLocation, playerViewPointRotation);
	FVector LineTraceEnd = playerViewPointLocation + playerViewPointRotation.Vector() * reach;
	UPrimitiveComponent* ComponentToGrab = possessedItem.GetComponent();

	// If we hit something then attach the physics handle
	if(possessedItem.GetActor()){
			PhysicsHandle->GrabComponentAtLocation(
		ComponentToGrab,
		NAME_None,
		LineTraceEnd
	);
	}
}

void UGrabber::Release() 
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber released!"))
	PhysicsHandle->ReleaseComponent();
}

void UGrabber::FindPhysicsHandle() 
{
	// Check for the physics handle component
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(PhysicsHandle){

	}	else {
		UE_LOG(LogTemp, Warning, TEXT("No physics handle component found on %s!"), *GetOwner()->GetName()) 
	}
}

void UGrabber::InitInputComponent() 
{
	inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if(inputComponent){
		// UE_LOG(LogTemp, Warning, TEXT("Input component found %s"), *GetOwner()->GetName())
		inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		inputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	} 
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	//Get player's viewpoint
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;

	// get the players viewpoint
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(playerViewPointLocation, playerViewPointRotation);

	// Logging out to test variables
	// UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"), *playerViewPointLocation.ToString(), *playerViewPointRotation.ToString());
	
	// Draw a line from player showing the reach
	// FVector LineTraceEnd = playerViewPointLocation + FVector(0.f, 0.f, 100.f);
	FVector LineTraceEnd = playerViewPointLocation + playerViewPointRotation.Vector() * reach;

	// DrawDebugLine(
	// 	GetWorld(),
	// 	playerViewPointLocation,
	// 	LineTraceEnd,
	// 	FColor(255, 0, 0),
	// 	false,
	// 	0.f,
	// 	0,
	// 	5.f
	// );

	// Ray-cast out to a certain distance (Reach)
	FHitResult Hit;

	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		Hit,
		playerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	// identify what the ray-cast is hitting
	AActor* ActorHit = Hit.GetActor();

	if(ActorHit)
	{
			// Logging out to test
	UE_LOG(LogTemp, Warning, TEXT("Line trace has hit: %s"), *(ActorHit->GetName()));
	}

	return Hit;
}

