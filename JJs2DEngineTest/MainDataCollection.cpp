#include "pch.h"
#include "MainDataCollection.h"

#include <Main.h>

#include <limits>

MainDataCollection::MainDataCollection()
{

	objectID = std::numeric_limits<size_t>::max();
}

MainDataCollection::~MainDataCollection()
{
}
