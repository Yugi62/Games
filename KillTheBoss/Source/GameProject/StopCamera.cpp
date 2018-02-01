#include "GameProject.h"
#include "StopCamera.h"

AStopCamera::AStopCamera()
{
	PrimaryActorTick.bCanEverTick = false;

	Layer = CreateDefaultSubobject<UBoxComponent>(TEXT("Layer"));
	RootComponent = Layer;

}
