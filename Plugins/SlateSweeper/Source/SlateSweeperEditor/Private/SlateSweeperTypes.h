// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#pragma once

#include "CoreMinimal.h"
#include "SlateSweeperTypes.generated.h"

/*
 * Shared data definitions for SlateSweeper.
 *
 * These structs and enums define the common "contract" between
 * all game entities so everyone can agree on what things look like
 * without directly depending on each other.
 */


// Delegate for listeners who want to know when a cell is pressed
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSlateSweeperCellPressed, int32 /*CellIndex*/)


// Possible results of attempting to reveal or interact with a cell. Flag toggles and so on would be implemented here.
enum class ESlateSweeperCellRevealOutcome : uint8
{
	// The cell was empty and got revealed successfully.
	RevealedSafe,

	// The cell contained a mine: game over.
	HitMine,

	// The action revealed a safe cell and triggered a flood-fill of neighbours.
	FloodReveal,
	
	// An invalid action (e.g. out-of-bounds or bad state).
	Invalid
};

// Complete description of the current minefield grid state.
struct FSlateSweeperGridData
{
	TBitArray<> RevealedCells;
	TArray<uint8> CellNeighbourCounts;
	TBitArray<> MineCells;

	uint8 GridWidth = 0;
	uint8 GridHeight = 0;
};

// Want to start a new game? This is what your request data will look like
USTRUCT()
struct FSlateSweeperNewGameSettings
{
	GENERATED_BODY()
	UPROPERTY() uint8 GridWidth = 20;
	UPROPERTY() uint8 GridHeight = 20;
	UPROPERTY() int32 TotalMines = 50;
};
