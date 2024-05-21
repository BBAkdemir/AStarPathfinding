#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums.h"
#include "GridTile.generated.h"

class AGrid;
class UWidgetComponent;
class UTileIndicator;

UCLASS()
class ASTAR_API AGridTile : public AActor
{
	GENERATED_BODY()
	
public:

	AGridTile();

protected:

	virtual void BeginPlay() override;

public:
	
	void Play();
	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UStaticMeshComponent* TileMesh;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UWidgetComponent* TileIndicator;

	UPROPERTY(VisibleAnywhere)
	FTile TileProperties;

	UPROPERTY(VisibleAnywhere)
	bool TileSelected = false;

	UPROPERTY()
	AGrid* Grid;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UTileIndicator* TileIndicatorRef;

public:

	UFUNCTION(BlueprintCallable)
	FVector2D GetGridIndex() { return TileProperties._GridIndex; }
	UFUNCTION(BlueprintCallable)
	EGroundType GetGroundType() { return TileProperties._GroundType; }
	UFUNCTION(BlueprintCallable)
	FLinearColor GetTileColor() { return TileProperties._TileColor; }
	UFUNCTION(BlueprintCallable)
	FVector GetWorldLocation() { return TileProperties._WorldLocation; }
	UFUNCTION(BlueprintCallable)
	int32 GetTileCost() { return TileProperties._TileCost; }
	UFUNCTION(BlueprintCallable)
	float GetFinalCost() { return TileProperties._FinalCost; }
	UFUNCTION(BlueprintCallable)
	float GetStartCost() { return TileProperties._StartCost; }
	UFUNCTION(BlueprintCallable)
	float GetTargetEstimatedCost() { return TileProperties._TargetEstimatedCost; }
	UFUNCTION(BlueprintCallable)
	FVector2D GetPreviousTile() { return TileProperties._PreviousTile; }

	void SetTargetEstimatedCost(float _TargetEstimatedCost) { TileProperties._TargetEstimatedCost = _TargetEstimatedCost; }
	void SetFinalCost(float _FinalCost) { TileProperties._FinalCost = _FinalCost; }
	void SetStartCost(float _StartCost) { TileProperties._StartCost = _StartCost; }
	void SetPreviousTile(FVector2D _PreviousTile) { TileProperties._PreviousTile = _PreviousTile; }

	UFUNCTION(BlueprintCallable)
	void SetTileProperties(AGrid* _Grid, FTile _TileProperties, float SizeY);

	UFUNCTION(BlueprintCallable)
	void SetTileSelected(bool bIsSelected);

	UFUNCTION(BlueprintCallable)
	void SetTileIsPath(bool bIsPath, bool bIsAccessible);

	UFUNCTION(BlueprintCallable)
	void SetTileIsControlled(bool bIsControlled, bool bIsIndicatorVisibility);

	UFUNCTION(BlueprintCallable)
	void SetTileIsInaccessible(bool bIsInaccessible);

	UFUNCTION(BlueprintCallable)
	void SetTileIndicatortVisibility(bool bIsVisible);

protected:

	UFUNCTION(BlueprintCallable)
	void OnBeginMouseOver(UPrimitiveComponent* TouchedComponent);

	UFUNCTION(BlueprintCallable)
	void OnEndMouseOver(UPrimitiveComponent* TouchedComponent);

	UFUNCTION(BlueprintCallable)
	void OnMouseClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);
};
