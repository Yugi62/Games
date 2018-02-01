#pragma once

#include "GameFramework/Actor.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "Elevator.generated.h"

UCLASS()
class GAMEPROJECT_API AElevator : public AActor
{
	GENERATED_BODY()
	
public:	

	AElevator();
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(VisibleAnywhere, Category = "Sprite")
	class UPaperSpriteComponent* SpriteComponent;

	UPROPERTY(VisibleAnywhere, Category = "Sprite")
	class UPaperSprite* Sprite;
	
	UPROPERTY(EditAnywhere, Category = "Value")
	bool ElevatorAxis;

	UPROPERTY(EditAnywhere, Category = "Value")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, Category = "Value")
	float MoveTime;

	FTimerHandle TimerHandle;
	void MoveElevator();
};
