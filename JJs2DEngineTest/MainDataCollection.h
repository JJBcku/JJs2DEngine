#pragma once

#include <MainDef.h>

struct MainDataCollection
{
	std::unique_ptr<JJ2DE::Main> main;

	MainDataCollection();
	~MainDataCollection();
};