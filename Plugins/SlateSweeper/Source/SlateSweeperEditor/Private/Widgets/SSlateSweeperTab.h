// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#pragma once

#include "Widgets/Docking/SDockTab.h"

class SSlateSweeperTab : public SDockTab
{
public:
	
	SLATE_BEGIN_ARGS(SSlateSweeperTab)
	{}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);	
};
