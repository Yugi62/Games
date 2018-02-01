#pragma once

#include "GameFramework/Actor.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "JumpPanel.generated.h"

UCLASS()
class GAMEPROJECT_API AJumpPanel : public AActor
{
	GENERATED_BODY()
	
public:	

	AJumpPanel();
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(VisibleAnywhere, Category = "Sprite")
	class UPaperSpriteComponent* SpriteComponent;

	UPROPERTY(VisibleAnywhere, Category = "Sprite")
	class UPaperSprite* Sprite;

	UPROPERTY(VisibleAnywhere, Category = "Collider")
	UBoxComponent* OverlapLayer;

	UPROPERTY(EditAnywhere, Category = "Value")
	float JumpValue;


	UFUNCTION()
	void OverlapStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
