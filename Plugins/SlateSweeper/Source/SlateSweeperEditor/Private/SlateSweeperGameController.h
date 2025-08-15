// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#pragma once

/* This would be our controller for a minesweeper game session
 * In this architecture, it owns the "view" (state) 
 * It is the only gateway to access the view and the model
 * todo check out this comment, rework it and make it nice
 */

class FSlateSweeperGameController
{
public:
	FSlateSweeperGameController(uint8 InMineGridWidth, uint8 InMineGridHeight, int32 InTotalMines); //todo perhaps pack this in game settings? 
	const TSharedRef<class SSlateSweeperMinefield>& GetSlateSweeperMinefield() const; //todo should name view?
	const TSharedRef<class FSlateSweeperGameState>& GetSlateSweeperGameState() const; //todo should name view?
	void OnCellPressed(int32 CellIndex);

private:
	TSharedRef<class FSlateSweeperGameState> GameState; //todo sharedptr? todo definitive name is minesweeper or slatesweeper?
	TSharedRef<class SSlateSweeperMinefield> GameView; //todo shared ? really? and what if I use a box placeholder and then this dies? THE CONTROLLER CANNOT DIE IF THE VIEW IS ALIVE
};
