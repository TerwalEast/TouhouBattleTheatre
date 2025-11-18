#pragma once

#include "../../TouhouBattleTheatre.h"
#include "../EnumFlags.h"

// 基础地形类型。一个格子只有一个基础地形。
enum class TerrainType : uint8_t
{
	Void,
	Plain,
	Mountain,
	Hill,
	Water,
	Lava,
	Swamp,
	CaveFloor,
	SolidWall
};

// 地形地貌特征。一个格子可以有多个特征（例如，森林覆盖的山丘）。
// 使用位掩码来组合它们。
enum class TerrainFeature : uint16_t
{
	None = 0,
	Forest = 1 << 0,    // 森林
	Grass = 1 << 1,     // 草地
	River = 1 << 2,     // 河流
	Sea = 1 << 3,       // 海洋
	Cave = 1 << 4      // 洞穴入口
};
ENABLE_BITMASK_OPERATORS(TerrainFeature);

// 生物群系。一个格子通常属于一个生物群系。
enum class Biome : uint16_t
{
	Void,
	MakaiNormal,
	MakaiHot,
	MakaiCold,

	ModernOutsideworld,
	FutureOutsideworld,

	Gensokyo,
	Netherworld,
	CelestialRealm,
	OldHell,
	LunarCapital,
};


class TerrainTile
{
public:
	TerrainTile();
	~TerrainTile();

	// --- 类型安全的地形访问接口 ---

	// 设置/获取基础地形
	void SetTerrainType(TerrainType type) { _terrainType = type; }
	TerrainType GetTerrainType() const { return _terrainType; }

	// 添加/移除/检查地貌特征
	void AddFeature(TerrainFeature feature) { _features |= feature; }
	void RemoveFeature(TerrainFeature feature) { _features &= ~feature; }
	bool HasFeature(TerrainFeature feature) const { return HasFlag(_features, feature); }
	TerrainFeature GetAllFeatures() const { return _features; }

	// 设置/获取生物群系
	void SetBiome(Biome biome) { _biome = biome; }
	Biome GetBiome() const { return _biome; }

protected:
	// ------------------------------------------------
	//+Y
	// Ʌ
	// |
	// |T+X+1  T+X+2  T+X+3
	// |
	// | T-1     T     T+1
	// |
	// |T-X-3  T-X-2  T-X-1
	// O---------------------> +X
	// -----------------------------------------------
	TerrainType _terrainType = TerrainType::Void;
	TerrainFeature _features = TerrainFeature::None;
	Biome _biome = Biome::Void;
};


class TileMap
{
public:
	TileMap();
	void Render();

private:
	int _walkableMapSizeX; //Beware there is a circle of false tiles around the map
	int _walkableMapSizeY; //So actual size of map is X+2 * Y+2
	TerrainTile* _tiles; //Dynamic allocate memory
	GLuint _tilesheetTexture;
	GLuint _vao;
	//void _updateMap();
	void _initFogOfWar();

};