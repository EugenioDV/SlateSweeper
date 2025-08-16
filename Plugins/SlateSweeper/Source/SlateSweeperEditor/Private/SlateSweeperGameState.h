// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.
#pragma once

#include "SlateSweeperGridData.h"


//todo comment state
// it's as backend as it gets
// 
class FSlateSweeperGameState
{
public:

	FSlateSweeperGameState(uint8 InMineGridWidth, uint8 InMineGridHeight, int32 InTotalMines); //todo int for total mines?

	void RevealCell(int32 CellIndex);

	TWeakPtr<const FSlateSweeperGridData> GetGridData() const;
	
private:

	TSharedRef<FSlateSweeperGridData, ESPMode::ThreadSafe> GridData;
};
