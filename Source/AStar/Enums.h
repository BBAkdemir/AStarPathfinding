#pragma once

#include "CoreMinimal.h"
//#include "GridTile.h"
#include "Enums.generated.h"

UENUM(BlueprintType)
enum EGroundType
{
	Normal,
	Difficult,
	ReallyDifficult,
	Impossible,
	None,
};

USTRUCT(BlueprintType)
struct FPath
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D _Tile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool _bIsTileAccessible;
};

USTRUCT(BlueprintType)
struct FTileProperty
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor _TileColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 _TileCost;
};

USTRUCT(BlueprintType)
struct FTile
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D _GridIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EGroundType> _GroundType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor _TileColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector _WorldLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 _TileCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _FinalCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _StartCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _TargetEstimatedCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D _PreviousTile;

	FTile()
	{
		_GridIndex = FVector2D::ZeroVector;
		_GroundType = EGroundType::Normal;
		_TileColor = FLinearColor::White;
		_WorldLocation = FVector::ZeroVector;
		_TileCost = 0;
		_FinalCost = 0;
		_StartCost = 0;
		_TargetEstimatedCost = 0;
		_PreviousTile = FVector2D::ZeroVector;
	}
};

class ASTAR_API Enums
{
public:
	Enums();
	~Enums();
};
