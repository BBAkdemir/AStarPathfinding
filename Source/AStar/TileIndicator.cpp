#include "TileIndicator.h"
#include "Components/ScaleBox.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetTextLibrary.h"

void UTileIndicator::SetTileCost(float _Cost)
{
	TileCost = _Cost;
	TB_TileCost->SetText(UKismetTextLibrary::Conv_FloatToText(_Cost, ERoundingMode::HalfToEven, false, false, 0, 5, 0, 2));
}

void UTileIndicator::SetTileEndCost(float _Cost)
{
	TileEndCost = _Cost;
	TB_TileEndCost->SetText(UKismetTextLibrary::Conv_FloatToText(_Cost, ERoundingMode::HalfToEven, false, false, 0, 5, 0, 2));
}

void UTileIndicator::SetTileCostVisibility(ESlateVisibility _VisibilityType)
{
	SB_TileCost->SetVisibility(_VisibilityType);
}

void UTileIndicator::SetTileEndCostVisibility(ESlateVisibility _VisibilityType)
{
	SB_TileEndCost->SetVisibility(_VisibilityType);
}
