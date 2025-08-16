// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#pragma once

/*
 * Utility struct for communication between state and view
 * The view does not need to know the whole truth, only what to reveal
 * This can also be the whole truth in case of the game end
 */

struct FSlateSweeperViewData
{
	TBitArray<> RevealedCells;
	TArray<uint8> RevealedCellNeighbourCounts;
	TBitArray<> MineCells;
	uint8 FieldGridWidth, FieldGridHeight;
};