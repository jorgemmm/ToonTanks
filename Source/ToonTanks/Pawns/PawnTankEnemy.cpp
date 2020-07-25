// Víctor J. Pasín Rguez All Rights Reserved


#include "PawnTankEnemy.h"
#include "PawnTank.h"





#include "GameFramework/Pawn.h"

#include "Components/BoxComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
//#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"

//#include "Blueprint/AIBlueprintHelperLibrary.h"

#include "Components/SceneComponent.h"
//#include "Components/StaticMeshComponent.h" 
#include "GameFramework/Controller.h"



APawnTankEnemy::APawnTankEnemy()
{
	
}

void APawnTankEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void APawnTankEnemy::Tick(float DeltaTime)
{

	if (!PlayerPawn) {
		UE_LOG(LogTemp, Error, TEXT("Player Pawn Not Loaded"));
		return;

	}
	Super::Tick(DeltaTime);	
	
	GetBaseMesh()->AddWorldOffset(
		PlayerPawn->GetActorLocation()
	);
	
	UE_LOG(LogTemp, Warning, TEXT("Enemy Pawn tank Move to: %s "), 
		*PlayerPawn->GetActorLocation().ToString());
	
	 
	//MoveTank(PlayerPawn->GetActorLocation());



}


void APawnTankEnemy::CheckDistance(APawn* PawnTank)
{

}

void APawnTankEnemy::MoveForward(float Value)
{

}
