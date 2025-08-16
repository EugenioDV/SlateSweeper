// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#pragma once

/*
 * This class's sole purpose is for everyone to agree on
 * What a SlateSweeper grid should look like
 * Without having to know each other.
 */

struct FSlateSweeperGridData
{
	TBitArray<> RevealedCells;
	TArray<uint8> CellNeighbourCounts;
	TBitArray<> MineCells;
	uint8 GridWidth, GridHeight;
};