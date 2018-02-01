#pragma once

#include "Camera/CameraShake.h"
#include "BossCameraShake.generated.h"


UCLASS()
class GAMEPROJECT_API UBossCameraShake : public UCameraShake
{
	GENERATED_BODY()

	UBossCameraShake();

	UPROPERTY(EditAnywhere, Category = "CameraShake")
	UCameraShake* CameraShake;
};
