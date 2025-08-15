// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "SlateSweeperSettings.generated.h"

/* Settings for a single SlateSweeper game
 * Tiny utility to store mine grid values that persists across editor instances
 * I just like the idea of the user going back to the settings they like to play
*/
UCLASS(config=EditorPerProjectUserSettings)
class USlateSweeperGameSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(config) uint8 GridWidth = 20;
	UPROPERTY(config) uint8 GridHeight = 20;
	UPROPERTY(config) int32 TotalMines = 50;
};


/** todo this is all AI generated, review
 * Editor-only settings for Slate Sweeper.
 * Visible under Editor Preferences in the Unreal Editor.
 */
UCLASS(config=Editor, defaultconfig, meta = (DisplayName="Slate Sweeper Editor Settings"))
class USlateSweeperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	// ----- Grid Limits -----
	UPROPERTY(EditAnywhere, config, Category="Grid Limits", meta = (ClampMin="4", ClampMax="100"))
	uint8 MinGridWidth = 4;

	UPROPERTY(EditAnywhere, config, Category="Grid Limits", meta = (ClampMin="4", ClampMax="100"))
	uint8 MaxGridWidth = 50;

	UPROPERTY(EditAnywhere, config, Category="Grid Limits", meta = (ClampMin="4", ClampMax="100"))
	uint8 MinGridHeight = 4;

	UPROPERTY(EditAnywhere, config, Category="Grid Limits", meta = (ClampMin="4", ClampMax="100"))
	uint8 MaxGridHeight = 50;
	
	// Difficulty thresholds to help the user understand what they're going to face
	UPROPERTY(EditAnywhere, config, Category="Difficulty Thresholds", meta = (ClampMin="0.0", ClampMax="1.0"))
	float MediumThreshold = 0.15f;

	UPROPERTY(EditAnywhere, config, Category="Difficulty Thresholds", meta = (ClampMin="0.0", ClampMax="1.0"))
	float HardThreshold = 0.25f;

	TTuple<FText, FSlateColor> GetDifficultyLabel(int32 GridWidth, int32 GridHeight, int32 TotalMines) const
	{
		const float MineRatio = static_cast<float>(TotalMines) / (GridWidth * GridHeight);

		if (MineRatio < MediumThreshold)
		{
			return MakeTuple(FText::FromString(TEXT("Easy")), FSlateColor(FLinearColor::Green));
		}
		else if (MineRatio < HardThreshold)
		{
			return MakeTuple(FText::FromString(TEXT("Medium")), FSlateColor(FLinearColor::Yellow));
		}
		else
		{
			return MakeTuple(FText::FromString(TEXT("Hard")), FSlateColor(FLinearColor::Red));
		}
	}

	// Calculates max mines allowed based on current grid todo idk if I should actually enforce a mine ratio
	int32 GetMinAllowedMines(int32 GridWidth, int32 GridHeight) const
	{
		return 0;
	}
	
	// Calculates max mines allowed based on current grid todo idk if I should actually enforce a mine ratio
	int32 GetMaxAllowedMines(int32 GridWidth, int32 GridHeight) const
	{
		return GridWidth * GridHeight;
	}

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
	{
		Super::PostEditChangeProperty(PropertyChangedEvent);

		const FName PropName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

		if (PropName == GET_MEMBER_NAME_CHECKED(USlateSweeperSettings, MinGridWidth))
		{
			if (MinGridWidth > MaxGridWidth)
			{
				MaxGridWidth = MinGridWidth;
			}
		}
		else if (PropName == GET_MEMBER_NAME_CHECKED(USlateSweeperSettings, MaxGridWidth))
		{
			if (MaxGridWidth < MinGridWidth)
			{
				MinGridWidth = MaxGridWidth;
			}
		}

		if (PropName == GET_MEMBER_NAME_CHECKED(USlateSweeperSettings, MinGridHeight))
		{
			if (MinGridHeight > MaxGridHeight)
			{
				MaxGridHeight = MinGridHeight;
			}
		}
		else if (PropName == GET_MEMBER_NAME_CHECKED(USlateSweeperSettings, MaxGridHeight))
		{
			if (MaxGridHeight < MinGridHeight)
			{
				MinGridHeight = MaxGridHeight;
			}
		}

		if (PropName == GET_MEMBER_NAME_CHECKED(USlateSweeperSettings, MediumThreshold))
		{
			if (MediumThreshold > HardThreshold)
			{
				HardThreshold = MediumThreshold;
			}
		}
		else if (PropName == GET_MEMBER_NAME_CHECKED(USlateSweeperSettings, HardThreshold))
		{
			if (HardThreshold < MediumThreshold)
			{
				MediumThreshold = HardThreshold;
			}
		}
	}

};