// Copyright Michael Bridges 2019

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
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

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>(); // Find the physics handle component 
	if (PhysicsHandle != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has physics handle component!"), *GetOwner()->GetName());
	}

	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent != nullptr)
	{
		// Bind the grab action to the Grab function
		// name has to be exactly the same as the function name
		// Input event  = IE 
		// the object = this -> it is a pointer to this object
		// the function = Grab -> addres to the function 
		// "Grab" is the name of the function that will be called when the action is pressed
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab); 
		
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release); 
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab released"));
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle == nullptr)
	{
		return;
	}

	// Get the players viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint( 
		OUT PlayerViewPointLocation, 
		OUT PlayerViewPointRotation
	);

	// draw a line from the player view point to the end of the reach
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.f,
		0.f,
		10.f
	);

	// Seeing what we hit
	FHitResult Hit;

	// TraceParams is a struct that contains parameters for the trace.
	// we dont want any complex object, so we set complex object to false.
	// what to we need to ignore, we set it to the owner of the component. the first object that we want to ignore is the player. Because it hits itself. we need to ignore the "us". 
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner()); 


	//Ray-cast out to reach distance
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), //collision channel to look for (the word "physics" is a bit misleading here, but it's the channel that the physics engine uses to detect collisions)
		TraceParams 
	);

	AActor* ActorHit = Hit.GetActor(); 

	// // Log out to test
	// if (ActorHit != nullptr	)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Hit something! %s"), *ActorHit->GetName());
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Hit nothing!"));
	// }



	// ...
}

