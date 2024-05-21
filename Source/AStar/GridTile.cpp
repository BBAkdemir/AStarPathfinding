#include "GridTile.h"
#include "Grid.h"
#include "Components/WidgetComponent.h"
#include "TileIndicator.h"

AGridTile::AGridTile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	SetRootComponent(TileMesh);
	TileIndicator = CreateDefaultSubobject<UWidgetComponent>(TEXT("TileIndicator"));
	TileIndicator->SetupAttachment(TileMesh);
	TileIndicator->SetWorldLocation(FVector(0, 0, 60));
	TileIndicator->SetWorldRotation(FRotator(90, 90, 0));
	TileIndicator->SetWorldScale3D(FVector(4, 4, 8));
	TileIndicator->SetDrawSize(FVector2D(25, 25));
	TileIndicator->SetTickMode(ETickMode::Automatic);
}

void AGridTile::BeginPlay()
{
	Super::BeginPlay();
}

void AGridTile::Play()
{
	TileMesh->OnBeginCursorOver.AddDynamic(this, &AGridTile::OnBeginMouseOver);
	TileMesh->OnEndCursorOver.AddDynamic(this, &AGridTile::OnEndMouseOver);
	TileMesh->OnClicked.AddDynamic(this, &AGridTile::OnMouseClicked);
	TileIndicatorRef = Cast<UTileIndicator>(TileIndicator->GetWidget());
	TileIndicatorRef->SetTileCost(TileProperties._TileCost);
	TileIndicatorRef->SetTileEndCost(TileProperties._StartCost);
}

void AGridTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGridTile::SetTileProperties(AGrid* _Grid, FTile _TileProperties, float SizeY)
{
	Grid = _Grid;
	TileProperties = _TileProperties;
	TileMesh->SetVectorParameterValueOnMaterials("Color", FVector(TileProperties._TileColor));
	TileIndicatorRef = Cast<UTileIndicator>(TileIndicator->GetWidget());
	TileIndicatorRef->SetTileCost(TileProperties._TileCost);
	TileIndicator->SetWorldScale3D(FVector(1.0f, SizeY * 2, SizeY * 2));
}

void AGridTile::SetTileSelected(bool bIsSelected)
{
	TileSelected = bIsSelected;
	TileMesh->SetScalarParameterValueOnMaterials("OnSelect", bIsSelected);
}

void AGridTile::SetTileIsPath(bool bIsPath, bool bIsAccessible)
{
	TileMesh->SetScalarParameterValueOnMaterials("OnPath", bIsPath);
	if (!bIsPath)
	{
		bIsAccessible = true;
	}
	TileMesh->SetScalarParameterValueOnMaterials("OnAccessible", !bIsAccessible);
}

void AGridTile::SetTileIsControlled(bool bIsControlled, bool bIsIndicatorVisibility)
{
	TileMesh->SetScalarParameterValueOnMaterials("OnControlled", bIsControlled);
	TileIndicatorRef->SetTileEndCost(TileProperties._StartCost);
	if (bIsIndicatorVisibility)
	{
		TileIndicatorRef->SetTileEndCostVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		TileIndicatorRef->SetTileEndCostVisibility(ESlateVisibility::Hidden);
	}
}

void AGridTile::SetTileIsInaccessible(bool bIsInaccessible)
{
	TileMesh->SetScalarParameterValueOnMaterials("OnInaccessible", bIsInaccessible);
}

void AGridTile::SetTileIndicatortVisibility(bool bIsVisible)
{
	TileIndicator->SetVisibility(bIsVisible);
}

void AGridTile::OnBeginMouseOver(UPrimitiveComponent* TouchedComponent)
{
	//UE_LOG(LogTemp, Warning, TEXT("OnBeginHover"));
	TileMesh->SetScalarParameterValueOnMaterials("OnHover", 1);
}

void AGridTile::OnEndMouseOver(UPrimitiveComponent* TouchedComponent)
{
	//UE_LOG(LogTemp, Warning, TEXT("OnEndHover"));
	TileMesh->SetScalarParameterValueOnMaterials("OnHover", 0);
}

void AGridTile::OnMouseClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	if (TileProperties._TileCost == 999)
	{
		return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("OnMouseClicked"));
	Grid->AddSelectedTile(this);
}

