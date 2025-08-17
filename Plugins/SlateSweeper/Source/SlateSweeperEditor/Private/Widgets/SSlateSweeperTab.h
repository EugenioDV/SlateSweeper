// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#pragma once

#include "SlateSweeperTypes.h"
#include "Widgets/Docking/SDockTab.h"

/*
 * Main tab for our module
 * In this stage of the project it holds the game view and the settings menu to start a game
 */

class SSlateSweeperTab : public SDockTab
{
public:

	SLATE_BEGIN_ARGS(SSlateSweeperTab)
	{}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	
	// The Game Controller kindly allows us to grab the view at any time and display it on the tab
	TWeakPtr<class FSlateSweeperGameController, ESPMode::ThreadSafe> CurrentGame;

	// The "slot" for the game view
 	TSharedPtr<SBox> MinefieldContainer;
	
	void OnStartNewGamePressed(const FSlateSweeperNewGameSettings& GameSettings);
};
