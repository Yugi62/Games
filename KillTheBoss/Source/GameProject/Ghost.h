#pragma once

#include "GameFramework/Actor.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "Ghost.generated.h"

UCLASS()
class GAMEPROJECT_API AGhost : public AActor
{
	GENERATED_BODY()
	
public:	

	AGhost();
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(VisibleAnywhere, Category = "Sprite")
	class UPaperSpriteComponent* SpriteComponent;

	UPROPERTY(VisibleAnywhere, Category = "Sprite")
	class UPaperSprite* Sprite;
};
