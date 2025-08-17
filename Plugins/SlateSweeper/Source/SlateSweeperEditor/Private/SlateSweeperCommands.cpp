// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#include "SlateSweeperCommands.h"

#define LOCTEXT_NAMESPACE "FSlateSweeperModule"

void FSlateSweeperCommands::RegisterCommands()
{
	UI_COMMAND(OpenSlateSweeperWindow, "SlateSweeper", "Summon the Mighty SlateSweeper Window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
