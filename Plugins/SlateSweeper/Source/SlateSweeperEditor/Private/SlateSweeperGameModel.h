// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#pragma once

#include "SlateSweeperTypes.h"

/*
 * In this architecture, the model is similar to the MVC's, but owned by the controller
 * Holds both the data and the functions for modifying the grid
 * If we were to implement backend functionality, we'd do it here
 */

class FSlateSweeperGameModel
{
public:

	FSlateSweeperGameModel(const FSlateSweeperNewGameSettings& InGameSettings);

	ESlateSweeperCellRevealOutcome RevealCell(int32 CellIndex);

	// Called when the game ends
	void RevealAllCells() { GridData->RevealedCells.Init(true, GridData->GridWidth * GridData->GridHeight); } 

	TWeakPtr<const FSlateSweeperGridData> GetGridData() const;
	
private:

	TSharedRef<FSlateSweeperGridData, ESPMode::ThreadSafe> GridData;
};
