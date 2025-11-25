#pragma once

#include "Unit.h"
#include "UnitRenderer.h"

#include <vector>
#include <memory>
#include <span>

class UnitManager
{
public:
    UnitManager(int mapWidth, int mapHeight);

    UnitID AddUnit(int x, int y);
    void RemoveUnit(UnitID id);
    void MoveUnit(UnitID id, int newX, int newY);
    std::span<const UnitID> GetUnitsAt(int x, int y) const;
    const Unit& GetUnit(UnitID id) const;
	const std::vector<Unit>& GetAllUnits() const { return _units; }

    void UpdateAllUnits();

private:
    std::vector<Unit> _units;

    std::vector<std::vector<UnitID>> _spatialGrid;
    int _mapWidth;
    int _mapHeight;

    // 用于管理单位ID的池
    std::vector<UnitID> _freeIDs;
};