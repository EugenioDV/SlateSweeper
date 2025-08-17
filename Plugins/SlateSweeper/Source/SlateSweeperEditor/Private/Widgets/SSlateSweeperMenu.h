// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#pragma once

class SSlateSweeperMenu : public SCompoundWidget
{
public:
	DECLARE_DELEGATE_ThreeParams(FOnStartGameClicked, uint8 /*GridWidth*/, uint8 /*GridHeight*/, int32 /*NumMines*/);
	
	SLATE_BEGIN_ARGS(SSlateSweeperMenu)
	{}
	SLATE_EVENT(FOnStartGameClicked, OnStartGameClicked)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	FOnStartGameClicked OnStartGameClicked;

};