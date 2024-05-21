#include "Grid.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Obstacle.h"
#include "GridTile.h"

AGrid::AGrid()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GridComp = CreateDefaultSubobject<USceneComponent>(TEXT("GridComp"));
	SetRootComponent(GridComp);
}

void AGrid::OnConstruction(const FTransform& Transform)
{
	World = GetWorld();
}

void AGrid::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	PlayerController->SetShowMouseCursor(true);
	PlayerController->bEnableMouseOverEvents = true;
	PlayerController->bEnableClickEvents = true;

	bIsGameStart = true;
	GenerateTiles();

	SetActorTickEnabled(false);
}

void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (OpenSet.Num() > 0)
	{
		ControlNeighbours();
	}
	else
	{
		SetActorTickEnabled(false);
		StartTile->SetTileIsInaccessible(true);
		TargetTile->SetTileIsInaccessible(true);
		ResetTileProperties();
	}
}

FVector AGrid::GridBottomLeft()
{
	return GridLocation - (GridComp->GetRightVector() * GridSizeWorld.X) - (GridComp->GetForwardVector() * GridSizeWorld.Y);
}

void AGrid::GridTileNumber(int& GridTileNumberX, int& GridTileNumberY)
{
	FVector2D GridTile = GridSizeWorld / TileSize;
	GridTileNumberX = FMath::RoundToInt(GridTile.X);
	GridTileNumberY = FMath::RoundToInt(GridTile.Y);
}

void AGrid::DrawTile()
{
	float RealTileSize = TileSize * 2;
	GridTileNumber(TileCountX, TileCountY);
	Tiles.SetNum(TileCountX * TileCountY, false);
	for (int32 x = 0; x < TileCountX; x++)
	{
		for (int32 y = 0; y < TileCountY; y++)
		{
			FVector NewForwardVector = GridComp->GetForwardVector() * ((RealTileSize * x) + TileSize);
			FVector NewRightVector = GridComp->GetRightVector() * ((RealTileSize * y) + TileSize);
			FVector TilePosition = GridBottomLeft() + NewRightVector + NewForwardVector;
			AActor* HitActor;
			if (!TileSphereTrace(TilePosition, ETraceTypeQuery::TraceTypeQuery3, HitActor))
			{
				continue;
			}
			if (TileSphereTrace(TilePosition, ETraceTypeQuery::TraceTypeQuery4, HitActor))
			{
				AObstacle* ObstacleActor = Cast<AObstacle>(HitActor);
				if (ObstacleActor)
				{
					CreateTile(FVector2D(x, y), ObstacleActor->GetObstacleType(), TilePosition);
					continue;
				}
			}
			CreateTile(FVector2D(x, y), EGroundType::Normal, TilePosition);
		}
	}
}

bool AGrid::TileSphereTrace(FVector Location, ETraceTypeQuery TraceChannel, AActor*& _OutHitActor)
{
	TArray<AActor*> IgnoreActors;
	FHitResult HitResult;
	bool bIsHit = UKismetSystemLibrary::SphereTraceSingle(World, Location, Location, TileSize - TileSizeMinus, TraceChannel, false, IgnoreActors, EDrawDebugTrace::None, HitResult, true, FColor::Red, FColor::Green, 0);
	_OutHitActor = HitResult.GetActor();
	return bIsHit;
}

FTileProperty AGrid::FindGroundProperty(EGroundType _GroundType)
{
	return *GroundProperties.Find(_GroundType);
}

void AGrid::DrawDebugPlane(FVector Location, EGroundType _GroundType)
{
	FTileProperty PlaneProperty = FindGroundProperty(_GroundType);
	DrawDebugSolidPlane(World, FPlane(0, 0, 1, Location.Z), Location, TileSize - TileSizeMinus, PlaneProperty._TileColor.ToFColor(true), false, 100000);
}

void AGrid::CreateTile(FVector2D _GridIndex, EGroundType _GroundType, FVector _WorldLocation)
{
	FTile _Tile;
	_Tile._GridIndex = _GridIndex;
	_Tile._GroundType = _GroundType;
	_Tile._WorldLocation = _WorldLocation;
	FTileProperty TileProperty = FindGroundProperty(_GroundType);
	_Tile._TileColor = TileProperty._TileColor;
	_Tile._TileCost = TileProperty._TileCost;
	AGridTile* GridTile = World->SpawnActor<AGridTile>(GridTileObject, _WorldLocation, FRotator::ZeroRotator);
	float TileScaleZ = 0.1f;
	switch (_Tile._GroundType)
	{
	case EGroundType::Difficult:
		TileScaleZ = 0.2f;
		break;
	case EGroundType::ReallyDifficult:
		TileScaleZ = 0.3f;
		break;
	case EGroundType::Impossible:
		TileScaleZ = 0.8f;
		break;
	default:
		break;
	}
	GridTile->SetActorScale3D(FVector(TileSize / 50.0f - 0.05f, TileSize / 50.0f - 0.05f, TileScaleZ));
	GridTile->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	GridTile->SetTileProperties(this, _Tile, TileSize / 50.0f - 0.05f);
	GridTile->SetTileIndicatortVisibility(bIsGeneratedTileInstigatorVisibility);
	Tiles[_GridIndex.Y * TileCountY + _GridIndex.X] = GridTile;
	if (bIsGameStart)
	{
		GridTile->Play();
	}
}

void AGrid::ResetAllTiles()
{
	if (Tiles.Num() == 0)
	{
		return;
	}
	for (int32 x = 0; x < TileCountX; x++)
		for (int32 y = 0; y < TileCountY; y++)
		{
			AGridTile* tile = Tiles[y * TileCountY + x];
			if (tile)
				tile->Destroy();
		}
	Tiles.Empty();
	FirstSelectedTile = nullptr;
	SecondSelectedTile = nullptr;
}

void AGrid::GenerateTiles()
{
	FlushPersistentDebugLines(World);
	ResetAllTiles();
	GridLocation = GridComp->GetComponentLocation();
	DrawDebugBox(World, GridLocation, FVector(GridSizeWorld.Y, GridSizeWorld.X, 5), GridBoxColor, false, 100000, 0.0f, 10.0f);
	DrawDebugBox(World, GridBottomLeft(), FVector(20.0f, 20.0f, 20.0f), BottomLeftColor, false, 100000, 0.0f, 10.0f);
	DrawTile();
}

void AGrid::GenerateTilesInstigator(float _GridSize, float _TileSize, bool _IsWatchDiscovery,	bool _IsControlledTileInstigatorVisibility,	bool _IsPathTileInstigatorVisibility, bool _IsGeneratedTileInstigatorVisibility, bool _IsColoringControlledTiles)
{
	GridSizeWorld = FVector2D(_GridSize, _GridSize);
	TileSize = _TileSize;
	bIsWatchDiscovery = _IsWatchDiscovery;
	bIsControlledTileInstigatorVisibility = _IsControlledTileInstigatorVisibility;
	bIsPathTileInstigatorVisibility = _IsPathTileInstigatorVisibility;
	bIsColoringControlledTiles = _IsColoringControlledTiles;
	bIsGeneratedTileInstigatorVisibility = _IsGeneratedTileInstigatorVisibility;
	GenerateTiles();
}

void AGrid::AddSelectedTile(AGridTile* _SelectedTile)
{
	if (FirstSelectedTile == nullptr)
	{
		FirstSelectedTile = _SelectedTile;
		FirstSelectedTile->SetTileSelected(true);
		return;
	}
	if (SecondSelectedTile == nullptr)
	{
		if (FirstSelectedTile != _SelectedTile)
		{
			SecondSelectedTile = _SelectedTile;
			SecondSelectedTile->SetTileSelected(true);
			FindPathToTarget(FirstSelectedTile->GetGridIndex(), SecondSelectedTile->GetGridIndex());
			return;
		}
		FirstSelectedTile->SetTileSelected(false);
		FirstSelectedTile = nullptr;
		return;
	}
	FirstSelectedTile->SetTileIsInaccessible(false);
	SecondSelectedTile->SetTileIsInaccessible(false);
	FirstSelectedTile->SetTileSelected(false);
	SecondSelectedTile->SetTileSelected(false);
	SetHighlightPath(false);
	FirstSelectedTile = _SelectedTile;
	SecondSelectedTile = nullptr;
	FirstSelectedTile->SetTileSelected(true);
	ResetTileProperties();
}

TArray<FVector2D> AGrid::GetTileNeighbours(FVector2D _TileIndex)
{
	TArray<FVector2D> OutNeighbours;
	for (int32 i = 0; i < 8; i++)
	{
		int32 ControlXIndex = _TileIndex.X + PossibleNeighbours[i].X;
		int32 ControlYIndex = _TileIndex.Y + PossibleNeighbours[i].Y;
		if (ControlYIndex > TileCountY - 1 || ControlYIndex < 0 || ControlXIndex > TileCountX - 1 || ControlXIndex < 0)
		{
			continue;
		}

		int32 _Index = ControlYIndex * TileCountY + ControlXIndex;
		if (_Index >= Tiles.Num() && _Index < 0)
		{
			continue;
		}

		AGridTile* Tile = Tiles[_Index];
		if (Tile == nullptr && Tile->GetGroundType() == EGroundType::None && Tile->GetGroundType() == EGroundType::Impossible)
		{
			continue;
		}

		OutNeighbours.AddUnique(FVector2D(ControlXIndex, ControlYIndex));
	}
	return OutNeighbours;
}

float AGrid::GetEstimatedCostToTarget(FVector2D _CurrentTile, FVector2D _TargetTile)
{
	FVector2D DifferentVector = _CurrentTile - _TargetTile;
	float x = FMath::Abs(DifferentVector.X);
	float y = FMath::Abs(DifferentVector.Y);
	float Cost;
	if (x > y)
	{
		Cost = (x - y) + (y * 1.41f);
	}
	else if (x < y)
	{
		Cost = (y - x) + (x * 1.41f);
	}
	else
	{
		Cost = x * 1.41f;
	}
	return Cost;
}

void AGrid::FindPathToTarget(FVector2D _StartTile, FVector2D _TargetTile)
{
	SetActorTickEnabled(false);
	StartTile = Tiles[_StartTile.Y * TileCountY + _StartTile.X];
	TargetTile = Tiles[_TargetTile.Y * TileCountY + _TargetTile.X];

	ControlGroundType = TargetTile->GetGroundType();
	if (ControlGroundType == EGroundType::None && ControlGroundType == EGroundType::Impossible)
	{
		return;
	}

	FindingCost = GetEstimatedCostToTarget(_StartTile, _TargetTile);
	StartTile->SetTargetEstimatedCost(FindingCost);
	StartTile->SetFinalCost(FindingCost);

	OpenSet.Empty();
	ClosedSet.Empty();
	CurrentControlTile = FVector2D::ZeroVector;
	OpenSetCheapest = FVector2D::ZeroVector;

	CurrentControlTile = _StartTile;
	OpenSet.AddUnique(_StartTile);

	if (bIsWatchDiscovery)
	{
		SetActorTickEnabled(true);
		return;
	}
	while (OpenSet.Num() > 0)
	{
		ControlNeighbours();
	}
	if (OpenSet.Num() <= 0 && Path.Num() == 0)
	{
		StartTile->SetTileIsInaccessible(true);
		TargetTile->SetTileIsInaccessible(true);
		ResetTileProperties();
	}
}

void AGrid::ControlNeighbours()
{
	OpenSetCheapest = OpenSet[0];
	for (int32 i = 0; i < OpenSet.Num(); i++)
	{
		AGridTile* FindTile = Tiles[OpenSet[i].Y * TileCountY + OpenSet[i].X];
		AGridTile* FindCheapestTile = Tiles[OpenSetCheapest.Y * TileCountY + OpenSetCheapest.X];
		if (FindTile->GetFinalCost() < FindCheapestTile->GetFinalCost())
		{
			OpenSetCheapest = OpenSet[i];
		}
		else if (FindTile->GetFinalCost() == FindCheapestTile->GetFinalCost() && FindTile->GetTargetEstimatedCost() < FindCheapestTile->GetTargetEstimatedCost())
		{
			OpenSetCheapest = OpenSet[i];
		}
	}
	CurrentControlTile = OpenSetCheapest;
	OpenSet.Remove(CurrentControlTile);
	ClosedSet.AddUnique(CurrentControlTile);
	FVector2D CurrentNeighbour;
	float CurrentNeighbourCostFromStart;

	TArray<FVector2D> TileNeighbours = GetTileNeighbours(CurrentControlTile);
	AGridTile* CurrentTileProperties = Tiles[CurrentControlTile.Y * TileCountY + CurrentControlTile.X];
	for (int32 i = 0; i < TileNeighbours.Num(); i++)
	{
		CurrentNeighbour = TileNeighbours[i];
		float MultiplyCost = 1;
		if (CurrentNeighbour.X != CurrentControlTile.X && CurrentNeighbour.Y != CurrentControlTile.Y)
		{
			MultiplyCost = 1.41f;
		}

		if (ClosedSet.Contains(CurrentNeighbour))
		{
			continue;
		}

		AGridTile* NeighbourTileProperties = Tiles[CurrentNeighbour.Y * TileCountY + CurrentNeighbour.X];
		if (NeighbourTileProperties == nullptr)
		{
			continue;
		}

		CurrentNeighbourCostFromStart = CurrentTileProperties->GetStartCost() + ((float)NeighbourTileProperties->GetTileCost() * MultiplyCost);

		if (!OpenSet.Contains(CurrentNeighbour))
		{
			OpenSet.AddUnique(CurrentNeighbour);
			SetTileProperties(NeighbourTileProperties, CurrentNeighbour, CurrentControlTile, TargetTile->GetGridIndex(), CurrentNeighbourCostFromStart);
		}
		else if (CurrentNeighbourCostFromStart < NeighbourTileProperties->GetStartCost())
		{
			SetTileProperties(NeighbourTileProperties, CurrentNeighbour, CurrentControlTile, TargetTile->GetGridIndex(), CurrentNeighbourCostFromStart);
		}
		if (CurrentNeighbour == TargetTile->GetGridIndex())
		{
			Path = RetracePath(StartTile->GetGridIndex().RoundToVector(), TargetTile->GetGridIndex().RoundToVector());
			SetHighlightPath(true);
			SetActorTickEnabled(false);
			OpenSet.Empty();
			return;
		}
	}
}

void AGrid::SetTileProperties(AGridTile* _NeighbourTile, FVector2D _CurrentNeighbour, FVector2D _CurrentTile, FVector2D _TargetTile, float _StartCost)
{
	PropertiesChangedTiles.Add(_NeighbourTile);
	_NeighbourTile->SetStartCost(_StartCost);
	if (bIsColoringControlledTiles)
	{
		_NeighbourTile->SetTileIsControlled(true, bIsControlledTileInstigatorVisibility);
	}
	else if (bIsWatchDiscovery)
	{
		_NeighbourTile->SetTileIsControlled(true, true);
	}
	else
	{
		_NeighbourTile->SetTileIsControlled(false, bIsControlledTileInstigatorVisibility);
	}
	_NeighbourTile->SetTileIndicatortVisibility(bIsControlledTileInstigatorVisibility);
	_NeighbourTile->SetTargetEstimatedCost(GetEstimatedCostToTarget(_CurrentNeighbour, _TargetTile));
	_NeighbourTile->SetFinalCost(_NeighbourTile->GetStartCost() + _NeighbourTile->GetTargetEstimatedCost());
	_NeighbourTile->SetPreviousTile(_CurrentTile);
}

void AGrid::ResetTileProperties()
{
	for (int32 i = 0; i < PropertiesChangedTiles.Num(); i++)
	{
		PropertiesChangedTiles[i]->SetStartCost(0);
		PropertiesChangedTiles[i]->SetTileIsControlled(false, false);
		PropertiesChangedTiles[i]->SetTargetEstimatedCost(0);
		PropertiesChangedTiles[i]->SetFinalCost(0);
		PropertiesChangedTiles[i]->SetTileIndicatortVisibility(false);
	}
}

TArray<FPath> AGrid::RetracePath(FVector2D _StartTile, FVector2D _TargetTile)
{
	FVector2D CurrentTile = _TargetTile;
	TArray<FPath> InvertedPath;
	TArray<FPath> OutPath;
	while (CurrentTile != _StartTile)
	{
		FPath NewPath;
		NewPath._Tile = CurrentTile;
		NewPath._bIsTileAccessible = true;
		if (Tiles[CurrentTile.Y * TileCountY + CurrentTile.X]->GetTileCost() == 999)
		{
			NewPath._bIsTileAccessible = false;
			for (int32 i = 0; i < InvertedPath.Num(); i++)
			{
				InvertedPath[i]._bIsTileAccessible = false;
			}
		}
		InvertedPath.Add(NewPath);
		CurrentTile = Tiles[CurrentTile.Y * TileCountY + CurrentTile.X]->GetPreviousTile();
	}
	for (int32 i = InvertedPath.Num() - 1; i >= 0; i--)
	{
		OutPath.Add(InvertedPath[i]);
	}
	return OutPath;
}

void AGrid::SetHighlightPath(bool bIsHighlight)
{
	for (int32 i = 0; i < Path.Num(); i++)
	{
		int32 _Index = Path[i]._Tile.Y * TileCountY + Path[i]._Tile.X;
		Tiles[_Index]->SetTileIsPath(bIsHighlight, Path[i]._bIsTileAccessible);
		if (bIsControlledTileInstigatorVisibility)
		{
			Tiles[_Index]->SetTileIsControlled(true, true);
			Tiles[_Index]->SetTileIndicatortVisibility(true);
		}
		else
		{
			Tiles[_Index]->SetTileIsControlled(true, bIsPathTileInstigatorVisibility);
			Tiles[_Index]->SetTileIndicatortVisibility(bIsPathTileInstigatorVisibility);
		}
	}
	if (!bIsHighlight)
	{
		Path.Empty();
	}
}

