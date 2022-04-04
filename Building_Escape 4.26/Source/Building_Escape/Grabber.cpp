// Copyright Michael Bridges 2019

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT // This is a macro that is used to define an out parameter.

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();

}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
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

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>(); // Find the physics handle component 
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab()
{
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();

	// if we hit something then attach a physics handle
	if (ActorHit){
		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None,
			GetPlayersReach()
		);
	}
}

void UGrabber::Release()
{
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}

// Called every frame (hot loop)
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if the physics handle is attached
	if (!PhysicsHandle) { return; }
	if (PhysicsHandle->GrabbedComponent)
	{
		// move the object that we're holding
		PhysicsHandle->SetTargetLocation(GetPlayersReach());
	}

}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{

#pragma region DebugDrawing
	/*
	// draw a line from the player view point to the end of the reach

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
	*/
#pragma endregion

	// Seeing what we hit
	FHitResult Hit;

	// TraceParams is a struct that contains parameters for the trace.
	// we dont want any complex object, so we set complex object to false.
	// what to we need to ignore, we set it to the owner of the component. the first object that we want to ignore is the player. Because it hits itself. we need to ignore the "us". 
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner()); 


	//Ray-cast out to reach distance
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayersWorldPos(),
		GetPlayersReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), //collision channel to look for (the word "physics" is a bit misleading here, but it's the channel that the physics engine uses to detect collisions)
		TraceParams 
	);

	return Hit;
}

FVector UGrabber::GetPlayersWorldPos() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	return PlayerViewPointLocation;
}

FVector UGrabber::GetPlayersReach() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}