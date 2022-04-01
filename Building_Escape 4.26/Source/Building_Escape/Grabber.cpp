// Copyright Michael Bridges 2019

#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Grabber.h"

#define OUT // This is a macro that is used to define an out parameter.

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get the players viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint( 
		OUT PlayerViewPointLocation, 
		OUT PlayerViewPointRotation
		);


	UE_LOG(LogTemp, Warning, TEXT("Player ViewPoint Location: %s"), *PlayerViewPointLocation.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Player ViewPoint Rotation: %s"), *PlayerViewPointRotation.ToString());

	//Ray-cast out to reach distance
	

	// See what we hit
	FHitResult Hit;


	// ...
}

