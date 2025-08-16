// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#pragma once
#include "Widgets/Layout/SBox.h"
#include "SlateSweeperGridData.h"


/*
 * Widget for rendering the SlateSweeper grid.
 * Displays cells based on provided view data
 * Responsible only for visuals and user input
 * Currently recreates itself at every turn
 */
class SSlateSweeperMinefieldView : public SBox
{
public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnCellPressed, int32 /*CellIndex*/)
	
	SLATE_BEGIN_ARGS(SSlateSweeperMinefieldView)
	{}
	SLATE_ARGUMENT(FSlateSweeperGridData, ViewData)
	SLATE_END_ARGS()

	void PopulateGrid();
	void Construct(const FArguments& InArgs);
	void Update(const FSlateSweeperGridData& NewData);
	
	// We don't want anyone editing the delegate or calling it from outside, but we do want to accept listeners
	FDelegateHandle RegisterOnCellPressed(const FOnCellPressed::FDelegate& InDelegate)
	{
		return OnCellPressedDelegate.Add(InDelegate);
	}

private:

	// This is basically the only way the view "talks" to anything else
	// otherwise its job is just to sit there and look pretty (allegedly)
	FOnCellPressed OnCellPressedDelegate;
	
	TSharedRef<SWidget> CraftGridCell(int32 InCellIndex) const;
	TSharedPtr<class SUniformGridPanel> GridPanel;

	FSlateSweeperGridData ViewData = FSlateSweeperGridData(); //todo reference to data or brutal copy every time?
	
};