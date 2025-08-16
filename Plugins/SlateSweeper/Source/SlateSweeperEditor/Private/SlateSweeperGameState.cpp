// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#include "SlateSweeperGameState.h"
#include "SlateSweeperEditor.h"

/* Minesweeper helper functions START */

// these functions aren't strongly bound to the module and could very well be in their own class, but there is no need

void AllocateMines(TBitArray<>& OutMineCells, int32 TotalMines, int32 TotalCells)
{
	if (TotalMines > TotalCells)
	{
		UE_LOG(LogSlateSweeper, Error, TEXT("Failed to allocate mines. TotalMines > TotalCells"));
		return;
	}
	
	// Reserve and initialise the Mine map before adding Mines
	OutMineCells.Init(false, TotalCells);
	
	// Place Mines uniquely in an inefficient fashion, will optimise later if needed
	TArray<int32> Indices;
	Indices.SetNumUninitialized(TotalCells);
	
	for (int32 i = 0; i < TotalCells; ++i)
	{
		Indices[i] = i;
	}
	
	for (int32 i = 0; i < TotalMines; ++i)
	{
		// indexes < i are used to store already-picked values, avoiding resizes with swap
		const int32 SwapWith = FMath::RandRange(i, TotalCells - 1);
		if (SwapWith != i)
		{
			Swap(Indices[i], Indices[SwapWith]);
		}
		OutMineCells[Indices[i]] = true;
	}
}

/*
 * Computes the neighbour mine counts for every cell in the grid.
 * Used as a one-off toll to set up the grid
 */
void ComputeMineNeighbourCounts(
	TArray<uint8>& OutCellNeighbourCounts,
	const TBitArray<>& MineCells,
	uint8 GridWidth,
	uint8 GridHeight)
{
	const int32 TotalCells = GridWidth * GridHeight;

	// We'll write increments on each index, so we need a zeroed baseline
	OutCellNeighbourCounts.SetNumZeroed(TotalCells);

	// Iterate over all mine locations
	for (TConstSetBitIterator<> MineIt(MineCells); MineIt; ++MineIt)
	{
		const int32 MineIndex = MineIt.GetIndex();
		const int32 MineX = MineIndex % GridWidth;
		const int32 MineY = MineIndex / GridWidth;

		// Visit all 8 neighbours around this mine
		for (int32 OffsetY = -1; OffsetY <= 1; ++OffsetY)
		{
			for (int32 OffsetX = -1; OffsetX <= 1; ++OffsetX)
			{
				// Skip self
				if (OffsetX == 0 && OffsetY == 0)
				{
					continue;
				}

				const int32 NeighbourX = MineX + OffsetX;
				const int32 NeighbourY = MineY + OffsetY;

				// Bounds check
				if (NeighbourX < 0 || NeighbourX >= GridWidth || NeighbourY < 0 || NeighbourY >= GridHeight)
				{
					continue;
				}

				const int32 NeighbourIndex = NeighbourY * GridWidth + NeighbourX;

				// Avoid mines marking other mines
				if (MineCells[NeighbourIndex])
				{
					continue;
				}

				++OutCellNeighbourCounts[NeighbourIndex];
			}
		}
	}
}

/*
 * Computes the neighbour mine counts for every cell in the grid.
 * Used as a one-off toll to set up the grid
 */
void FloodRevealCells(int32 CellIndex, uint8 GridWidth, uint8 GridHeight, const TArray<uint8>& CellNeighbourCounts,  TBitArray<>& OutGridRevealedCells)
{
	// Flood fill: reveal all connected empty cells and bordering numbers
	TQueue<int32> CellsToProcess;
	CellsToProcess.Enqueue(CellIndex);
	
	while (!CellsToProcess.IsEmpty())
	{
		int32 CurrentIndex;
		CellsToProcess.Dequeue(CurrentIndex);

		const int32 X = CurrentIndex % GridWidth;
		const int32 Y = CurrentIndex / GridWidth;
		
		// Check all 8 neighbours 
		for (int32 OffsetY = -1; OffsetY <= 1; ++OffsetY)
		{
			for (int32 OffsetX = -1; OffsetX <= 1; ++OffsetX)
			{
				// Skip self
				if (OffsetX == 0 && OffsetY == 0)
				{
					continue;
				}
				
				const int32 NeighbourX = X + OffsetX;
				const int32 NeighbourY = Y + OffsetY;

				// Bounds check
				if (NeighbourX < 0 || NeighbourX >= GridWidth || NeighbourY < 0 || NeighbourY >= GridHeight)
				{
					continue;
				}

				const int32 NeighbourIndex = NeighbourY * GridWidth + NeighbourX;

				// Skip if already revealed (can't be a mine as we reach neighbours first)
				if (OutGridRevealedCells[NeighbourIndex])
				{
					continue;
				}

				// Reveal neighbour
				OutGridRevealedCells[NeighbourIndex] = true;

				// If a neighbour is empty, queue it for further expansion
				if (CellNeighbourCounts[NeighbourIndex] == 0)
				{
					CellsToProcess.Enqueue(NeighbourIndex);
				}
			}
		}
	}
}

/* Minesweeper helper functions END */

FSlateSweeperGameState::FSlateSweeperGameState(uint8 InMineGridWidth, uint8 InMineGridHeight, int32 InTotalMines)
	: GridData (MakeShared<FSlateSweeperGridData>())
{
	
	GridData->GridWidth = InMineGridWidth;
	GridData->GridHeight = InMineGridHeight;
	
	const int32 TotalCells = InMineGridHeight*InMineGridWidth;
	
	GridData->RevealedCells.Init(false, TotalCells);
	
	AllocateMines(GridData->MineCells, InTotalMines, TotalCells);
	ComputeMineNeighbourCounts(GridData->CellNeighbourCounts, GridData->MineCells, InMineGridWidth, InMineGridHeight);
}

void FSlateSweeperGameState::RevealCell(int32 CellIndex)
{
	if (!GridData->RevealedCells.IsValidIndex(CellIndex) || GridData->RevealedCells[CellIndex])
	{
		UE_LOG(LogSlateSweeper, Error, TEXT("Failed to reveal cell. Cell index invalid or already revealed: %d"), CellIndex);
		return;
	}

	// Reveal the initial cell
	GridData->RevealedCells[CellIndex] = true;

	// If this cell has neighbours, no need to do anything else
	if (GridData->CellNeighbourCounts[CellIndex] > 0)
	{
		return;
	}

	// For this project scope, revealing all cells is enough to end the game when the user picks a mined cell
	if (GridData->MineCells[CellIndex])
	{
		GridData->RevealedCells.Init(true, GridData->GridWidth * GridData->GridHeight);
	}

	FloodRevealCells(CellIndex, GridData->GridWidth, GridData->GridHeight, GridData->CellNeighbourCounts, GridData->RevealedCells);
}

TWeakPtr<const FSlateSweeperGridData> FSlateSweeperGameState::GetGridData() const
{
	return GridData;
}