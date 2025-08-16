// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#include "SlateSweeperGameState.h"
#include "SlateSweeperEditor.h"
#include "SlateSweeperViewData.h"


void AllocateMines(TBitArray<>& OutBombCells, int32 TotalMines, int32 TotalCells)
{
	if (TotalMines > TotalCells)
	{
		UE_LOG(LogSlateSweeper, Error, TEXT("Failed to allocate mines. TotalMines > TotalCells"));
		return;
	}
	
	// Reserve and initialize the bomb map before adding bombs
	OutBombCells.Init(false, TotalCells);
	
	// Place bombs uniquely in an inefficient fashion, will optimize later if needed
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
		OutBombCells[Indices[i]] = true;
	}
}

void ComputeBombNeighbourCounts(TArray<uint8>& OutCellNeighbourCounts, const TBitArray<>& BombCells, uint8 MineGridWidth, uint8 MineGridHeight)
{
	const int32 InTotalCells = MineGridWidth*MineGridHeight;
	
	// We'll write increments on each index, so we need a zeroed baseline
	OutCellNeighbourCounts.SetNumZeroed(InTotalCells);

	//todo blatant paste from ChadGpt to be formatted
	auto IsValid = [&](int32 X, int32 Y)
	{
		return (X >= 0 && Y >= 0 && X < MineGridWidth && Y < MineGridHeight);
	};

	for (TConstSetBitIterator<> It(BombCells); It; ++It)
	{
		const int32 I = It.GetIndex();       // bomb index
		const int32 X = I % MineGridWidth;
		const int32 Y = I / MineGridWidth;

		for (int32 dy = -1; dy <= 1; ++dy)
		{
			for (int32 dx = -1; dx <= 1; ++dx)
			{
				if (dx == 0 && dy == 0) continue;           // skip self
				const int32 NX = X + dx, NY = Y + dy;
				if (!IsValid(NX, NY)) continue;

				const int32 NIdx = NY * MineGridWidth + NX;
				if (BombCells[NIdx]) continue;                  // keep bomb cells at 0

				++OutCellNeighbourCounts[NIdx];                 // safe: max is 8
			}
		}
	}
}

FSlateSweeperGameState::FSlateSweeperGameState(uint8 InMineGridWidth, uint8 InMineGridHeight, int32 InTotalMines)
	:FieldGridWidth(InMineGridWidth), FieldGridHeight(InMineGridHeight)
{
	const int32 TotalCells = InMineGridHeight*InMineGridWidth;
	
	RevealedCells.Init(false, TotalCells);
	
	AllocateMines(MineCells, InTotalMines, TotalCells);
	ComputeBombNeighbourCounts(CellNeighbourCounts, MineCells, InMineGridWidth, InMineGridHeight);
}

FSlateSweeperViewData FSlateSweeperGameState::GetViewData() const //todo actually construct view data or return pointers
{
	FSlateSweeperViewData ViewData;
	ViewData.FieldGridWidth = FieldGridWidth;
	ViewData.FieldGridHeight = FieldGridHeight;
	ViewData.MineCells = MineCells;
	ViewData.RevealedCells = RevealedCells;
	ViewData.RevealedCellNeighbourCounts = CellNeighbourCounts;
	return ViewData;
}
