#include "pch.h"
#include "MainDataCollection.h"

#include <Main.h>

#include <limits>

MainDataCollection::MainDataCollection()
{
	objectID = std::numeric_limits<size_t>::max();

	is16Bit = false;
	isRBReversed = false;
}

MainDataCollection::~MainDataCollection()
{
}
