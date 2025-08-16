// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

/*
 * The style isn't really used as aesthetics aren't requested for this tech test
 * Admittedly, my opinion on this class is biased as my job used to be making it redundant
 * We do have a custom icon
 */

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