// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.
#pragma once

class FSlateSweeperGameState
{
public:

	FSlateSweeperGameState(uint8 InMineGridWidth, uint8 InMineGridHeight, int32 InTotalMines); //todo int for total mines?
	
	FORCEINLINE uint8 GetMineGridWidth() const { return MineGridWidth; }
	FORCEINLINE uint8 GetMineGridHeight() const { return MineGridHeight; }
	FORCEINLINE const TBitArray<>& GetMineCells() const { return MineCells; }
	FORCEINLINE const TBitArray<>& GetRevealedCells() const { return RevealedCells; }
	FORCEINLINE const TArray<uint8>& GetCellNeighbourCounts() const { return CellNeighbourCounts; }

private:

	//todo maybe to a struct to separate game settings that don't change between turns from those who do, and also separate things nicely?
	uint8 MineGridWidth, MineGridHeight;
	TBitArray<> MineCells;
	TBitArray<> RevealedCells;
	TArray<uint8> CellNeighbourCounts;
};
