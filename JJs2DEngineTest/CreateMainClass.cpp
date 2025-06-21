#include "pch.h"
#include "CreateMainClass.h"

#include "MainDataCollection.h"

#include <MainInitializationData.h>
#include <Main.h>

void CreateMainClass(MainDataCollection& data)
{
	JJ2DE::MainInitializationData mainInitData;

	data.main = std::make_unique<JJ2DE::Main>(mainInitData);
}
