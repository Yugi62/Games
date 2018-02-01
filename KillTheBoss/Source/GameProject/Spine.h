#pragma once

#include "GameFramework/Actor.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "Spine.generated.h"

UCLASS()
class GAMEPROJECT_API ASpine : public AActor
{
	GENERATED_BODY()
	
public:	

	ASpine();

	UPROPERTY(VisibleAnywhere, Category = "Sprite")
	class UPaperSpriteComponent* SpriteComponent;

	UPROPERTY(VisibleAnywhere, Category = "Sprite")
	class UPaperSprite* Sprite;

	UFUNCTION()
	void OverlapStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
