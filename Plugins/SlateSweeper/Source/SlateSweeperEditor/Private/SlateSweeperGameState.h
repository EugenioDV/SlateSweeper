// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#pragma once

class FSlateSweeperGameState
{
	friend class FSlateSweeperGameController; //todo scrutiny this I kinda like it but never used actually not needed
public:

	FSlateSweeperGameState(uint8 InMineGridWidth, uint8 InMineGridHeight, int32 InTotalMines); //todo int for total mines?
	
	FORCEINLINE uint8 GetMineGridWidth() const { return MineGridWidth; }
	FORCEINLINE uint8 GetMineGridHeight() const { return MineGridHeight; }
	FORCEINLINE const TBitArray<>& GetMineCells() const { return MineCells; }
	FORCEINLINE const TBitArray<>& GetRevealedCells() const { return RevealedCells; }
	FORCEINLINE const TArray<uint8>& GetCellNeighbourCounts() const { return CellNeighbourCounts; }

private:

	void PressCell(int32 CellIndex)//todo this is all GPT
	{
		if (!RevealedCells.IsValidIndex(CellIndex) || RevealedCells[CellIndex])
		{
			return;
		}

		// Reveal the initial cell
		RevealedCells[CellIndex] = true;

		// If this cell has neighbours, just reveal it — no flood fill
		if (CellNeighbourCounts[CellIndex] > 0 || MineCells[CellIndex])
		{
			return;
		}

		// Flood fill: reveal all connected empty cells and bordering numbers
		TQueue<int32> CellsToProcess;
		CellsToProcess.Enqueue(CellIndex);

		while (!CellsToProcess.IsEmpty())
		{
			int32 CurrentIndex;
			CellsToProcess.Dequeue(CurrentIndex);

			const int32 X = CurrentIndex % MineGridWidth;
			const int32 Y = CurrentIndex / MineGridWidth;

			// Check all 8 neighbours
			for (int32 dy = -1; dy <= 1; ++dy)
			{
				for (int32 dx = -1; dx <= 1; ++dx)
				{
					if (dx == 0 && dy == 0)
					{
						continue; // Skip self
					}

					const int32 NX = X + dx;
					const int32 NY = Y + dy;

					// Bounds check
					if (NX < 0 || NX >= MineGridWidth || NY < 0 || NY >= MineGridHeight)
					{
						continue;
					}

					const int32 NeighbourIndex = NY * MineGridWidth + NX;

					// Skip if already revealed or is a mine
					if (RevealedCells[NeighbourIndex] || MineCells[NeighbourIndex])
					{
						continue;
					}

					// Reveal neighbour
					RevealedCells[NeighbourIndex] = true;

					// If neighbour is empty, queue it for further expansion
					if (CellNeighbourCounts[NeighbourIndex] == 0)
					{
						CellsToProcess.Enqueue(NeighbourIndex);
					}
				}
			}
		}
	}
	
	//todo maybe to a struct to separate game settings that don't change between turns from those who do, and also separate things nicely?
	uint8 MineGridWidth, MineGridHeight;
	TBitArray<> MineCells;
	TBitArray<> RevealedCells;
	TArray<uint8> CellNeighbourCounts;
};
