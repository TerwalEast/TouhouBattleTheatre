#include "UnitManager.h"

#include <algorithm> 

UnitManager::UnitManager(int mapWidth, int mapHeight)
    : _mapWidth(mapWidth), _mapHeight(mapHeight)
{
    _spatialGrid.resize(mapWidth * mapHeight);
	AddUnit(0, 0); // 添加一个测试单位
}

UnitID UnitManager::AddUnit(int x, int y)
{
    UnitID id;
    if (!_freeIDs.empty())
    {
        // 重用已释放的ID
        id = _freeIDs.back();
        _freeIDs.pop_back();
        _units[id] = Unit(x, y);
    }
    else
    {
        // 分配新ID
        id = static_cast<UnitID>(_units.size());
        _units.emplace_back(x, y);
    }

    // 更新空间索引
    _spatialGrid[y * _mapWidth + x].push_back(id);
    return id;
}

void UnitManager::RemoveUnit(UnitID id)
{
    // 从空间索引中移除
    const auto& unit = _units[id];
    auto& unitsOnTile = _spatialGrid[unit.GetY() * _mapWidth + unit.GetX()];
    unitsOnTile.erase(std::remove(unitsOnTile.begin(), unitsOnTile.end(), id), unitsOnTile.end());

    // 将ID添加到freelist以供重用
    _freeIDs.push_back(id);
}

void UnitManager::MoveUnit(UnitID id, int newX, int newY)
{
    Unit& unit = _units[id];
    int oldX = unit.GetX();
    int oldY = unit.GetY();

    // 从旧格子的空间索引中移除
    auto& oldTileUnits = _spatialGrid[oldY * _mapWidth + oldX];
    oldTileUnits.erase(std::remove(oldTileUnits.begin(), oldTileUnits.end(), id), oldTileUnits.end());

    // 更新单位位置
    unit.SetPosition(newX, newY);

    // 添加到新格子的空间索引中
    _spatialGrid[newY * _mapWidth + newX].push_back(id);
}

std::span<const UnitID> UnitManager::GetUnitsAt(int x, int y) const
{
    if (x < 0 || x >= _mapWidth || y < 0 || y >= _mapHeight)
    {
        return {}; // 返回一个空的 span
    }
    const auto& unitIDs = _spatialGrid[y * _mapWidth + x];
    return { unitIDs.data(), unitIDs.size() };
}

const Unit& UnitManager::GetUnit(UnitID id) const
{
    return _units[id];
}

void UnitManager::UpdateAllUnits()
{
    for (auto& unit : _units)
    {
        unit.Update();
    }
}