// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#pragma once
#include "Widgets/Layout/SBox.h"
#include "SlateSweeperViewData.h"

/*
 * todo describe
 *
 *
 *
 */

class SSlateSweeperMinefieldView : public SBox
{
public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnCellPressed, int32 /*CellIndex*/);
	
	SLATE_BEGIN_ARGS(SSlateSweeperMinefieldView)
	{}
	SLATE_ARGUMENT(FSlateSweeperViewData, ViewData)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	void Update(const FSlateSweeperViewData& NewData);
	
	// We don't want anyone editing the delegate or calling it from outside, but we do want to accept listeners
	FDelegateHandle RegisterOnCellPressed(const FOnCellPressed::FDelegate& InDelegate)
	{
		return OnCellPressedDelegate.Add(InDelegate);
	}

private:

	// This is basically the only way the view "talks" to anything else
	// otherwise its job is just to sit there and be pretty
	FOnCellPressed OnCellPressedDelegate;
	
	TSharedRef<SWidget> CraftGridCellButton(int32 InCellIndex) const;
	TSharedPtr<class SUniformGridPanel> GridPanel;

	FSlateSweeperViewData ViewData = FSlateSweeperViewData(); //todo reference to data or brutal copy every time?
	
};