#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TileIndicator.generated.h"

class UTextBlock;
class UScaleBox;

UCLASS()
class ASTAR_API UTileIndicator : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	UScaleBox* SB_TileCost;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TB_TileCost;
	UPROPERTY(meta = (BindWidget))
	UScaleBox* SB_TileEndCost;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TB_TileEndCost;

	UPROPERTY()
	float TileCost;
	UPROPERTY()
	float TileEndCost;

public:

	UFUNCTION(BlueprintCallable)
	void SetTileCost(float _Cost);
	UFUNCTION(BlueprintCallable)
	void SetTileEndCost(float _Cost);
	UFUNCTION(BlueprintCallable)
	void SetTileCostVisibility(ESlateVisibility _VisibilityType);
	UFUNCTION(BlueprintCallable)
	void SetTileEndCostVisibility(ESlateVisibility _VisibilityType);

};
