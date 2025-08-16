// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#pragma once

#include "Widgets/Docking/SDockTab.h"

/*
 * Main tab
 * todo describe this
 *
 * holds settings 
 */

class SSlateSweeperTab : public SDockTab
{
public:
	
	SLATE_BEGIN_ARGS(SSlateSweeperTab)
	{}
	SLATE_ARGUMENT(class FSlateSweeperEditor*, Module)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	
	// Since the module creates the tab in this architecture,
	// We don't risk a dangling pointer as the module always outlives the tab
	class FSlateSweeperEditor* SlateSweeperModule = nullptr;

	// The Game Controller kindly allows us to grab the view at any time and display it on the tab
	TWeakPtr<class FSlateSweeperGameController, ESPMode::ThreadSafe> CurrentGame;

	// The "slot" for the game view
 	TSharedPtr<SBox> MinefieldContainer;
	
	void OnStartNewGamePressed(uint8 GridWidth, uint8 GridHeight, int32 TotalMines);
};
