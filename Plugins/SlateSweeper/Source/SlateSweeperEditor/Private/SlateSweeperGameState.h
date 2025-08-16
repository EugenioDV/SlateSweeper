// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#pragma once

class FSlateSweeperGameState
{
public:

	FSlateSweeperGameState(uint8 InMineGridWidth, uint8 InMineGridHeight, int32 InTotalMines); //todo int for total mines?
	
	// No need to force inline in current architecture, but if one wants to call this every frame we are covered
	FORCEINLINE uint8 GetMineGridWidth() const { return FieldGridWidth; }
	FORCEINLINE uint8 GetMineGridHeight() const { return FieldGridHeight; }
	FORCEINLINE const TBitArray<>& GetMineCells() const { return MineCells; }
	FORCEINLINE const TBitArray<>& GetRevealedCells() const { return RevealedCells; }
	FORCEINLINE const TArray<uint8>& GetCellNeighbourCounts() const { return CellNeighbourCounts; }

	void RevealCell(int32 CellIndex);

	struct FSlateSweeperViewData GetViewData() const;
	
private:
	
	uint8 FieldGridWidth, FieldGridHeight;
	TBitArray<> MineCells;
	TBitArray<> RevealedCells;
	TArray<uint8> CellNeighbourCounts;
};
