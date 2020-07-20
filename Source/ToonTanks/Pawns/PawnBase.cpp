// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnBase.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ToonTanks/Actors/ProjectileBase.h"
#include "ToonTanks/Components/HealthComponent.h"
//#include "ProjectileBase.h"

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"
#include "Particles/ParticleSystem.h"
//#include "Engine/EngineTypes.h"
//#include "Components/PrimitiveComponent.h"

#define OUT

// Sets default values
APawnBase::APawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;
	
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);
	
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);
	
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

	HealthComponent= CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));

}

// Called when the game starts or when spawned
void APawnBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

}



// -- PawnTank - Inform GameMode Player died -> Then Hide() all components && stop movement Input.

void APawnBase::PawnDestroyed()
{
	HandleDestruction();
}


// Called to bind functionality to input
//void APawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}



void APawnBase::RotateTurret(FVector LookAtTarget)
{
	//update TurretMesh to face the LookAtTarget passed in from the child Class
	// TurretMesh->SetWorldRotation()...
	// Find Rotation value to look at. Rot Start  pos x e y del target y z de la torreta
	
	FVector StartLocation = TurretMesh->GetComponentLocation();
	FRotator TurretRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation,
		FVector(LookAtTarget.X, LookAtTarget.Y, TurretMesh->GetComponentLocation().Z));

	// Rotate Turret.
	TurretMesh->SetWorldRotation(TurretRotation);
	//TurretMesh->SetWorldRotation(TurretRotation,true,nullptr,ETeleportType::ResetPhysics);

	

}

void APawnBase::Fire()
{
	// Get ProjectileSpawnPoint Location && Rotation -> Spawn Projectile class
	//at Location towards Rotation. 



	if (ProjectileClass)
	{
		FVector  ProjectileSpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		FRotator ProjectileSpawnRotator  = ProjectileSpawnPoint->GetComponentRotation();

		UE_LOG(LogTemp, Warning, TEXT("FIRE!!"));
		

		AProjectileBase* TempProjectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, ProjectileSpawnLocation, ProjectileSpawnRotator);

		TempProjectile->SetOwner(this);
	}
	else
	{
		
		UE_LOG(LogTemp, Error, TEXT("Could not Fire!!... Projectile Base is NONE"));

		UE_LOG(LogTemp, Error, TEXT("in Pawn: %s "), *GetName());
	}

}

void APawnBase::HandleDestruction()
{
	// Universal functionality --- 
	// Play death effects particle, sound and camera shake. 	
	
	UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticle, GetActorLocation());
	
	
	/*UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(DeathShake);*/
		


	// ... Then do unique child overrides:

	// -- PawnTurret - Inform GameMode Turret died -> Then Destroy() self. 

	// -- PawnTank - Inform GameMode Player died -> Then Hide() all components && stop movement Input.

}