
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums.h"
#include "Obstacle.generated.h"

UCLASS()
class ASTAR_API AObstacle : public AActor
{
	GENERATED_BODY()
	
public:

	AObstacle();

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<EGroundType> ObstacleType;

public:

	UFUNCTION(BlueprintCallable)
	EGroundType GetObstacleType() { return ObstacleType; }
};
