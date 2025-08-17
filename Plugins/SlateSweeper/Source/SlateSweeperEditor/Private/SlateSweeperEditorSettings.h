// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "SlateSweeperTypes.h"
#include "SlateSweeperEditorSettings.generated.h"

/*
 * Settings for starting a single SlateSweeper game
 * Stores mine grid values and persists across editor instances
 * I like the idea of the user going back to the settings they like to play
 */

UCLASS(config=EditorPerProjectUserSettings)
class USlateSweeperGameSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(config) FSlateSweeperNewGameSettings CurrentGameSettings;
};


/*
 * Editor-only general settings for Slate Sweeper.
 * Regulates maximum grid size, cell/mine ratio and so forth
 * Visible in the editor settings like any other plugin settings.
 */

UCLASS(config=Editor, defaultconfig, meta = (DisplayName="Slate Sweeper Editor Settings"))
class USlateSweeperEditorSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, config, Category="Aesthetics", meta = (ClampMin="10", ClampMax="50"))
	float DesiredCellSize = 25.f;

	// Grid size constraints
	UPROPERTY(EditAnywhere, config, Category="Grid Limits", meta = (ClampMin="4", ClampMax="100"))
	uint8 MinGridWidth = 4;

	UPROPERTY(EditAnywhere, config, Category="Grid Limits", meta = (ClampMin="4", ClampMax="100"))
	uint8 MaxGridWidth = 50;

	UPROPERTY(EditAnywhere, config, Category="Grid Limits", meta = (ClampMin="4", ClampMax="100"))
	uint8 MinGridHeight = 4;

	UPROPERTY(EditAnywhere, config, Category="Grid Limits", meta = (ClampMin="4", ClampMax="100"))
	uint8 MaxGridHeight = 50;

	// Allowed mines are just hard limits for now, so we can test the boundaries of the system
	int32 GetMinAllowedMines(int32 GridWidth, int32 GridHeight) const
	{
		return 0;
	}
	
	int32 GetMaxAllowedMines(int32 GridWidth, int32 GridHeight) const
	{
		return GridWidth * GridHeight;
	}

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
	{
		Super::PostEditChangeProperty(PropertyChangedEvent);

		const FName PropName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

		if (PropName == GET_MEMBER_NAME_CHECKED(USlateSweeperEditorSettings, MinGridWidth))
		{
			if (MinGridWidth > MaxGridWidth)
			{
				MaxGridWidth = MinGridWidth;
			}
		}
		else if (PropName == GET_MEMBER_NAME_CHECKED(USlateSweeperEditorSettings, MaxGridWidth))
		{
			if (MaxGridWidth < MinGridWidth)
			{
				MinGridWidth = MaxGridWidth;
			}
		}

		if (PropName == GET_MEMBER_NAME_CHECKED(USlateSweeperEditorSettings, MinGridHeight))
		{
			if (MinGridHeight > MaxGridHeight)
			{
				MaxGridHeight = MinGridHeight;
			}
		}
		else if (PropName == GET_MEMBER_NAME_CHECKED(USlateSweeperEditorSettings, MaxGridHeight))
		{
			if (MaxGridHeight < MinGridHeight)
			{
				MinGridHeight = MaxGridHeight;
			}
		}
	}

};