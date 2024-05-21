#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GridSettingsPanel.generated.h"

class USpinBox;
class UCheckBox;
class UButton;

UCLASS()
class ASTAR_API UGridSettingsPanel : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(meta = (BindWidget))
	USpinBox* SB_GridSize;
	UPROPERTY(meta = (BindWidget))
	USpinBox* SB_TileSize;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* CB_IsWatchDiscovery;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* CB_ControlledTileVisibility;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* CB_PathTileVisibility;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* CB_ColoringControlledTiles;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* CB_GeneratedTileDefaultVisibility;
	UPROPERTY(meta = (BindWidget))
	UButton* B_Generate;
};
