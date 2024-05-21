#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums.h"
#include "Grid.generated.h"

class AGridTile;

UCLASS()
class ASTAR_API AGrid : public AActor
{
	GENERATED_BODY()

public:

	AGrid();

protected:

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY()
	UWorld* World;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	USceneComponent* GridComp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector GridLocation;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector2D GridSizeWorld = FVector2D(800.0f, 800.0f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FColor GridBoxColor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FColor BottomLeftColor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TileSize = 25.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TileSizeMinus = 5.0f;
	UPROPERTY()
	int32 TileCountX;
	UPROPERTY()
	int32 TileCountY;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<AGridTile> GridTileObject;

	//If set to true, it will noticeably slow down the calculations of the tiles.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsWatchDiscovery;
	//If checked true, it makes the Cost values of all controlled tiles visible.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsControlledTileInstigatorVisibility;
	//If marked true, it makes the Cost values of the tiles selected as path visible. If bIsVisibilityOfTheCostOfControlledTiles is true, this value automatically behaves as true.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsPathTileInstigatorVisibility;
	//If marked true, there will be no instigator on the tile unless any tile is marked for path creation. This option will provide a lot of optimization if too many tiles are created.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsGeneratedTileInstigatorVisibility;
	// If set to false, it does not color tiles that are controlled but not selected as paths. If bIsWatchDiscovery is true, any coloring is done.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsColoringControlledTiles;

	UPROPERTY(VisibleAnywhere)
	TArray<AGridTile*> Tiles;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TMap<TEnumAsByte<EGroundType>, FTileProperty> GroundProperties;

	UPROPERTY(VisibleAnywhere)
	AGridTile* FirstSelectedTile;

	UPROPERTY(VisibleAnywhere)
	AGridTile* SecondSelectedTile;

	UPROPERTY()
	bool bIsGameStart = false;

protected:

	UFUNCTION(BlueprintCallable)
	FVector GridBottomLeft();
	UFUNCTION(BlueprintPure, BlueprintCallable)
	void GridTileNumber(int& GridTileNumberX, int& GridTileNumberY);
	UFUNCTION(BlueprintCallable)
	void DrawTile();

	UFUNCTION(BlueprintCallable)
	bool TileSphereTrace(FVector Location, ETraceTypeQuery TraceChannel, AActor*& _OutHitActor);
	UFUNCTION(BlueprintCallable)
	FTileProperty FindGroundProperty(EGroundType _GroundType);
	UFUNCTION(BlueprintCallable)
	void DrawDebugPlane(FVector Location, EGroundType _GroundType);
	UFUNCTION(BlueprintCallable)
	void CreateTile(FVector2D _GridIndex, EGroundType _GroundType, FVector _WorldLocation);

	UFUNCTION(BlueprintCallable)
	void ResetAllTiles();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void GenerateTiles();

public:

	UFUNCTION(BlueprintCallable, CallInEditor)
	void GenerateTilesInstigator(
		float _GridSize,
		float _TileSize,
		bool _IsWatchDiscovery,
		bool _IsControlledTileInstigatorVisibility,
		bool _IsPathTileInstigatorVisibility,
		bool _IsGeneratedTileInstigatorVisibility,
		bool _IsColoringControlledTiles);

	UFUNCTION(BlueprintCallable)
	void SetIsWatchDiscovery(bool _IsWatchDiscovery) { bIsWatchDiscovery = _IsWatchDiscovery; }
	UFUNCTION(BlueprintCallable)
	void SetIsControlledTileInstigatorVisibility(bool _IsControlledTileInstigatorVisibility) { bIsControlledTileInstigatorVisibility = _IsControlledTileInstigatorVisibility; }
	UFUNCTION(BlueprintCallable)
	void SetIsPathTileInstigatorVisibility(bool _IsPathTileInstigatorVisibility) { bIsPathTileInstigatorVisibility = _IsPathTileInstigatorVisibility; }
	UFUNCTION(BlueprintCallable)
	void SetIsColoringControlledTiles(bool _IsColoringControlledTiles) { bIsColoringControlledTiles = _IsColoringControlledTiles; }

	UFUNCTION(BlueprintPure, BlueprintCallable)
	float GetGridSize() { return GridSizeWorld.X; }
	UFUNCTION(BlueprintPure, BlueprintCallable)
	float GetTileSize() { return TileSize; }
	UFUNCTION(BlueprintPure, BlueprintCallable)
	bool GetIsGeneratedTileInstigatorVisibility() { return bIsGeneratedTileInstigatorVisibility; }
	UFUNCTION(BlueprintPure, BlueprintCallable)
	bool GetIsWatchDiscovery() { return bIsWatchDiscovery; }
	UFUNCTION(BlueprintPure, BlueprintCallable)
	bool GetIsControlledTileInstigatorVisibility() { return bIsControlledTileInstigatorVisibility; }
	UFUNCTION(BlueprintPure, BlueprintCallable)
	bool GetIsPathTileInstigatorVisibility() { return bIsPathTileInstigatorVisibility; }
	UFUNCTION(BlueprintPure, BlueprintCallable)
	bool GetIsColoringControlledTiles() { return bIsColoringControlledTiles; }

	UFUNCTION(BlueprintCallable)
	void AddSelectedTile(AGridTile* _SelectedTile);

protected:

	FVector2D PossibleNeighbours[8]
	{
		FVector2D(1,0),
		FVector2D(0,1),
		FVector2D(-1,0),
		FVector2D(0,-1),
		FVector2D(1,1),
		FVector2D(-1,1),
		FVector2D(1,-1),
		FVector2D(-1,-1)
	};

	UPROPERTY()
	AGridTile* StartTile;
	UPROPERTY()
	AGridTile* TargetTile;
	UPROPERTY()
	TEnumAsByte<EGroundType> ControlGroundType;
	UPROPERTY()
	float FindingCost;
	UPROPERTY()
	TArray<FVector2D> OpenSet;
	UPROPERTY()
	TArray<FVector2D> ClosedSet;
	UPROPERTY()
	FVector2D CurrentControlTile;
	UPROPERTY()
	FVector2D OpenSetCheapest;

	UPROPERTY(VisibleAnywhere)
	TArray<FPath> Path;

	UPROPERTY()
	TArray<AGridTile*> PropertiesChangedTiles;

protected:

	UFUNCTION(BlueprintCallable)
	TArray<FVector2D> GetTileNeighbours(FVector2D _TileIndex);

	UFUNCTION(BlueprintCallable)
	float GetEstimatedCostToTarget(FVector2D _CurrentTile, FVector2D _TargetTile);

	UFUNCTION(BlueprintCallable)
	void FindPathToTarget(FVector2D _StartTile, FVector2D _TargetTile);

	UFUNCTION(BlueprintCallable)
	void ControlNeighbours();

	UFUNCTION(BlueprintCallable)
	void SetTileProperties(AGridTile* _NeighbourTile, FVector2D _CurrentNeighbour, FVector2D _CurrentTile, FVector2D _TargetTile, float _StartCost);

	UFUNCTION(BlueprintCallable)
	void ResetTileProperties();

	UFUNCTION(BlueprintCallable)
	TArray<FPath> RetracePath(FVector2D _StartTile, FVector2D _TargetTile);

	UFUNCTION(BlueprintCallable)
	void SetHighlightPath(bool bIsHighlight);
};
