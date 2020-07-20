// Fill out your copyright notice in the Description page of Project Settings.


#include "TankGameModeBase.h"
#include "ToonTanks/Pawns/PawnTank.h"
#include "ToonTanks/Pawns/PawnTurret.h"
#include "ToonTanks/PlayerControllers/PlayerControllerBase.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

#define OUT

void ATankGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	// Get references and game win/lose conditions.
	TargetTurrets = GetTargetTurretCount();
	PlayerTank = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));

	// Call HandleGameStart to initialise the start countdown, turret activation, pawn check etc.
	HandleGameStart();

}


void ATankGameModeBase::ActorDied(AActor* DeadActor)
{
	// Check what type of Actor died.  
	UE_LOG(LogTemp, Warning, TEXT("A Pawn Died"));
	//If Player -> go to lose condition.
	if (DeadActor == PlayerTank)
	{
		

		UE_LOG(LogTemp, Warning, TEXT("A PlayerPawn Died"));
		PlayerTank->PawnDestroyed();
		HandleGameOver(false);

		/*if (PlayerControllerRef)
		{
			PlayerControllerRef->SetPlayerEnabledState(false);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("GM.ActorDied PlayerController not Found or none"));
		}*/
	}
	  //If Turret, tally.
	else if (APawnTurret* DestroyedTurret = Cast<APawnTurret>(DeadActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("A EnemyPawn Died"));
		
		DestroyedTurret->PawnDestroyed();

		TargetTurrets--;

		if (TargetTurrets == 0)
		{
			HandleGameOver(true);
		}
	}

}


int32 ATankGameModeBase::GetTargetTurretCount()
{
	// Get references and game win/lose conditions.
	TSubclassOf<APawnTurret> ClassToFind;
	ClassToFind = APawnTurret::StaticClass();
	
	TArray<AActor*> TurretActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, OUT TurretActors);
	return TurretActors.Num();

}

void ATankGameModeBase::HandleGameStart()
{
	//in beginplay
	/*TargetTurrets = GetTargetTurretCount();
	PlayerTank = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));*/

	// initialise the start countdown, turret activation, pawn check etc.
	PlayerControllerRef = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(this, 0));//En online, 0 es el controller local

	// Call Blueprint version GameStart();
	GameStart();

	if (PlayerControllerRef)
	{
		
		DelayToStart();

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GM.HandleGameStart PlayerController not Found or none"));
	}
}

void ATankGameModeBase::HandleGameOver(bool PlayerWon)
{
	// See if the player has destroyed all the turrets, show win result.
   
	// else if turret destroyed player, show lose result. 
   
   // Call blueprint version GameOver();
	GameOver(PlayerWon);

	if (PlayerControllerRef)
	{
		DelayToStart();
		
		//En Delay to start
		//PawnTank->GetHealthcomponent->Health = PawnTank->GetHealthComponent->DefaultHealth;

		
		//GameStart();	
		
		//Después de Gamestart
		//Disable Pawn HandleDestruction (PawnTank->  Visibilitity, Tick , Setbisalive) 

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GM.HandleGameOver PlayerController not Found or none"));
	}

}



void ATankGameModeBase::DelayToStart() 
{
	
	
	PlayerControllerRef->SetPlayerEnabledState(false);



	//Vamos a utilizar un timer delegado
	//porque dispara una función en otro objeto
	FTimerHandle PlayerEnableHandle;

	//FTimerDelegate PlayerEnableDelegate = FTimerDelegate::CreateUObject(PlayerControllerRef, &APlayerControllerBase::SetPlayerEnabledState,true);		
	//GetWorldTimerManager().SetTimer(PlayerEnableHandle, PlayerEnableDelegate, StartDelay, false);

	GetWorldTimerManager().ClearTimer(PlayerEnableHandle);
	GetWorldTimerManager().SetTimer(
		PlayerEnableHandle,
		this,
		&ATankGameModeBase::EnableController,
		StartDelay,
		false,
		0.f
	);
}

void ATankGameModeBase::EnableController()
{

	
	if (PlayerControllerRef)
	{
		PlayerControllerRef->SetPlayerEnabledState(true);


	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GM.EnableContoller PlayerController not Found or none"));
	}


}