// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

// todo low style is not used, should we nuke it?
class FSlateSweeperStyle
{
public:

	static void Initialize();

	static void Shutdown();

	/** reloads textures used by slate renderer */
	static void ReloadTextures();

	/** @return The Slate style set for the Shooter game */
	static const ISlateStyle& Get();

	static FName GetStyleSetName();

private:

	static TSharedRef< class FSlateStyleSet > Create();

private:

	static TSharedPtr< class FSlateStyleSet > StyleInstance;
};