// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#pragma once

#include "SlateSweeperTypes.h"

/*
 * The "state" class for our game a bit similar to the "model" of a MVC pattern
 * Holds both the data and the functions for modifying the grid
 * If we were to implement backend functionality, we'd do it here
 */

class FSlateSweeperGameState
{
public:

	FSlateSweeperGameState(uint8 InMineGridWidth, uint8 InMineGridHeight, int32 InTotalMines);

	ESlateSweeperCellRevealOutcome RevealCell(int32 CellIndex);

	// Called when the game ends
	void RevealAllCells() { GridData->RevealedCells.Init(true, GridData->GridWidth * GridData->GridHeight); } 

	TWeakPtr<const FSlateSweeperGridData> GetGridData() const;
	
private:

	TSharedRef<FSlateSweeperGridData, ESPMode::ThreadSafe> GridData;
};
