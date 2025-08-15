// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#pragma once
#include "Widgets/Layout/SBox.h"


//todo this is our actual view for the game

class SSlateSweeperMinefield : public SBox //todo figure out if sbox is the best, sstackablebox seems like best but it's actually hard, sboxpanel is what vertical and horizontal boxes are derived from so perhaps I could use that
{
public:
	
	SLATE_BEGIN_ARGS(SSlateSweeperMinefield)
	{}
	SLATE_ARGUMENT(class FSlateSweeperGame*, Controller) //todo verify if I can force a reference to controller to be created somehow
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

	// In this architecture, the controller mustn't ever die if the view is alive and vide versa
	// Would love to use TSharedRef but it would be hard to handle the Construct pattern
	class FSlateSweeperGame* Controller = nullptr; //todo fix this mess
};