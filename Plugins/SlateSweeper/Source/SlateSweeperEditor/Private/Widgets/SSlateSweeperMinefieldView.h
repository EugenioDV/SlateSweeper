// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#pragma once
#include "Widgets/Layout/SBox.h"
#include "SlateSweeperGridData.h"


/*
 * Widget for rendering the SlateSweeper grid, similar to the "view" of an MVC pattern
 * Responsible only for visuals and passing on user input
 * If we were to implement a new visual aspect of the experience, we'd do it here
 */
class SSlateSweeperMinefieldView : public SBox
{
public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnCellPressed, int32 /*CellIndex*/)
	
	SLATE_BEGIN_ARGS(SSlateSweeperMinefieldView)
	{}
	SLATE_ARGUMENT(TWeakPtr<const FSlateSweeperGridData>, ViewData)
	SLATE_END_ARGS()

	void PopulateGrid();
	void Construct(const FArguments& InArgs);
	void Update(const TWeakPtr<const FSlateSweeperGridData>& NewData);
	
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

	TWeakPtr<const FSlateSweeperGridData> ViewData;
	
};