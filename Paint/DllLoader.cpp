#include "../Include/WinManager.h"
#include <filesystem>
#include <cassert>

namespace fs = std::experimental::filesystem;

sf::DllLoader::DllLoader(Palette * palette1) :
	palette_(palette1)
{
}

sf::DllLoader::~DllLoader()
{
	for (unsigned int i = 0; i < palette_->GetPalette().size(); i++)
		delete(palette_->GetPalette()[i]->GetInstData().icon_released_);

	for (unsigned int i = 0; i < palette_->GetPalette().size(); i++)
		delete(palette_->GetPalette()[i]);
}

void sf::DllLoader::loadFromFolder(const char * path)
{
	for (auto & p : fs::directory_iterator(path))
	{
		if (p.path ().extension () != ".dll") continue;
		HMODULE dll = LoadLibrary(p.path().string().c_str());
		auto objectGetter = reinterpret_cast<AbstInst*(*)()> (GetProcAddress(dll, "CreateInstObject"));
		if (objectGetter) palette_->AddInst(objectGetter());
		else continue;
	}
}

void sf::DllLoader::loadFromFile(const char * path)
{
	HMODULE dll = LoadLibrary(path);
	auto objectGetter = reinterpret_cast<AbstInst*(*)()> (GetProcAddress(dll, "CreateInstObject"));
	assert(objectGetter);
	palette_->AddInst(objectGetter());
}
