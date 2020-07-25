// Víctor J. Pasín Rguez All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "PawnTurret.h"  
#include "PawnTankEnemy.generated.h"





/**
 * 
 */

UCLASS()
class TOONTANKS_API APawnTankEnemy: public APawnTurret //public APawnBase
{
	GENERATED_BODY()
	
public:
	APawnTankEnemy();

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CheckDistance(APawn* PawnTank);


private:

	void MoveForward(float Value);

	FVector Velocity;

protected:


};
