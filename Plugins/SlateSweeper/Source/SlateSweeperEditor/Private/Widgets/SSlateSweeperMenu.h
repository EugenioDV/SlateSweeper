// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#pragma once
#include "Widgets/SCompoundWidget.h"
#include "SlateSweeperTypes.h"

class SSlateSweeperMenu : public SCompoundWidget
{
public:
	
	DECLARE_DELEGATE_OneParam(FOnStartGameClicked, const FSlateSweeperNewGameSettings& /*New Game Settings*/);
	
	SLATE_BEGIN_ARGS(SSlateSweeperMenu)
	{}
	SLATE_EVENT(FOnStartGameClicked, OnStartGameClicked)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	
	FOnStartGameClicked OnStartGameClicked;
};