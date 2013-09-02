#include "pch.h"
#include "World.h"

#include "sf_tilemap.h"
#include "sf_tileset.h"
#include "sf_tilemap_loader.h"

using namespace sftile;

World::World()
: current_id("null")
{
	cout << "World created" << endl;
}

World::World(const World& copy)
: loader(copy.loader)
, tilemaps(copy.tilemaps)
, current_id(copy.current_id)
{}

World& World::operator=(const World& copy)
{
	using std::swap;

	if (this != &copy) {
		World temp(copy);
		swap(loader, temp.loader);
		swap(tilemaps, temp.tilemaps);
		swap(current_id, temp.current_id);
	}

	return *this;
}

SfTilemap* World::LoadTilemap(string id, string path)
{
	SfTilemap tilemap;
  
	// Check to make sure the SfTilemap doesn't already exists.
	// If not, then attempt to parse the data for it.
	if (!MapExists(id) && !loader.LoadTilemap(path, tilemap)) {
		cout << "Failed to load SfTilemap from path: " << path << endl;
		return nullptr;
	}
  
	tilemaps.emplace(id, std::move(tilemap));

	cout << "Loaded SfTilemap from path: " << path << endl;

	current_id = id;
	return GetTilemap(id);
}

SfTilemap* World::GetTilemap(string _id)
{
	if (tilemaps.find(_id) == tilemaps.end()) {
		cout << "Failed to find SfTilemap with ID: " << _id << endl;
		return nullptr;
	}
	else {
		return &tilemaps.at(_id);
	}
}

void World::HandleEvents(sf::Event _evt)
{
	GetTilemap(current_id)->HandleEvents(_evt);
}

void World::Update()
{
	GetTilemap(current_id)->Update();
}

void World::Render(sf::RenderWindow& _window)
{
	GetTilemap(current_id)->Render(_window);
}

bool World::MapExists(string id)
{
	return tilemaps.find(id) != tilemaps.end();
}
