#include "Obstacle.h"

AObstacle::AObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);
}

void AObstacle::BeginPlay()
{
	Super::BeginPlay();
}

void AObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

