// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "SlateSweeperStyle.h"

class FSlateSweeperCommands : public TCommands<FSlateSweeperCommands>
{
public:

	FSlateSweeperCommands()
		: TCommands<FSlateSweeperCommands>(TEXT("SlateSweeper"), NSLOCTEXT("Contexts", "SlateSweeper", "SlateSweeper Plugin"), NAME_None, FSlateSweeperStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenSlateSweeperWindow;
};
