// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldPosition.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UWorldPosition::UWorldPosition()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWorldPosition::BeginPlay()
{
	Super::BeginPlay();

	//referance material
	// FString Log = TEXT("hello");
	// FString* PtrLog = &Log; // the address of Log

	// Log.Len();
	// (*PtrLog).Len(); // the same as Log.Len() , dereferencinng the pointer
	// PtrLog->Len(); // the same as (*PtrLog).Len()  indirect operator , indirection on the left side of the operator




	// UE_LOG(LogTemp, Warning, TEXT("%s"), **PtrLog); // dereferencing the pointer, using overload operator

	FString ObjectName =	GetOwner()->GetName();
	UE_LOG(LogTemp, Warning, TEXT("This component is atteched to %s"), *GetOwner()->GetName()); // same as above

	FString ObjectPosition = GetOwner()->GetTransform().GetLocation().ToString(); 
	FString ObjectActorPos = GetOwner()->GetActorLocation().ToString();

	UE_LOG(LogTemp, Warning, TEXT("%s position is %s"), *ObjectName, *ObjectPosition); 
	UE_LOG(LogTemp, Warning, TEXT("%s position in world %s"), *ObjectName, *ObjectActorPos); 
	
	
}


// Called every frame
void UWorldPosition::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

