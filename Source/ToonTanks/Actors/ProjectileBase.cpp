// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"

#include "CollisionQueryParams.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundBase.h"
#include "Camera/CameraShake.h"
#include "Kismet/GameplayStatics.h"

#include "DrawDebugHelpers.h"


// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
	RootComponent = ProjectileMesh;
	
	ParticleTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Trail"));
	ParticleTrail->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->InitialSpeed = MovementSpeed;
	ProjectileMovement->MaxSpeed = MovementSpeed;

	InitialLifeSpan = 3.0f;


}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
	UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());

}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Try to get a reference to the owning class. 
	AActor* MyOwner = GetOwner();

	// If for some reason we can't get a valid reference, return as we need to check against the owner. 
	if (!MyOwner)
	{
		return;
	}

	// If the OtherActors isn't self or owner && exists, then apply damage.
	//And Particle system
	TArray<AActor*> IgnorethisActors;// [] = { this , MyOwner };
	IgnorethisActors.AddUnique(this);
	IgnorethisActors.AddUnique(MyOwner);

	if (OtherActor != NULL && OtherActor != this && OtherActor != MyOwner)
	{
		//Dannage
		UGameplayStatics::ApplyDamage(
			OtherActor, 
			PointDamage,
			MyOwner->GetInstigatorController(), 
			this, 
			DamageType
		);

		UGameplayStatics::ApplyRadialDamage(
			OtherActor,
			RadialDamage,
			Hit.ImpactPoint,//GetActorLocation(),
			RadialRange,
			DamageType,
			IgnorethisActors,
			this,
			MyOwner->GetInstigatorController()		
			
		);

		DrawDebugSphere(GetWorld(), Hit.ImpactPoint , 20.f, 8.f, FColor::Yellow,false,3.0f,0,1.0f);

		
		//ProjectileMesh->AddForce(this->GetVelocity() * 100);

		// Particle System
		UGameplayStatics::SpawnEmitterAtLocation(
			this,
			HitParticle,
			GetActorLocation()
		);

		// Audio FX
		UGameplayStatics::PlaySoundAtLocation(
			this, 
			HitSound,
			GetActorLocation()
		);

		//Camera Shake
		GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(HitShake);

	}


	// Do a bunch of effects here during polish phase.
	
	
	
	//Destroy proyectile after Hit
	Destroy(); 
}



