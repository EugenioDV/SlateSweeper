// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#pragma once

#include "Widgets/Docking/SDockTab.h"

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
	// We can go simple as the module always outlives the tab
	class FSlateSweeperEditor* SlateSweeperModule = nullptr;
	
	TWeakPtr<class FSlateSweeperGameController, ESPMode::ThreadSafe> CurrentGame;
	
	TSharedPtr<SBox> MinefieldContainer;
	void OnStartGamePressed();
};
