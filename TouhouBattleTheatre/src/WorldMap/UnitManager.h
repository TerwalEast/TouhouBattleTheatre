#pragma once

#include <vector>
#include <memory>
#include "Unit.h"

class UnitManager
{
public:
    UnitManager(int mapWidth, int mapHeight);

    UnitID AddUnit(int x, int y);

    void RemoveUnit(UnitID id);

    void MoveUnit(UnitID id, int newX, int newY);

    const std::vector<UnitID>& GetUnitsAt(int x, int y) const;

    Unit& GetUnit(UnitID id);
    const Unit& GetUnit(UnitID id) const;

    void UpdateAllUnits();
	void RenderAllUnits();

private:
    std::vector<Unit> _units;

    std::vector<std::vector<UnitID>> _spatialGrid;
    int _mapWidth;
    int _mapHeight;

    std::vector<UnitID> _freeIDs;
};