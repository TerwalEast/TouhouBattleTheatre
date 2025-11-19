#pragma once

#include <cstdint>
#include <string>

using UnitID = uint32_t;

class Unit
{
public:
    Unit(int x, int y) : _x(x), _y(y) {}

    void Update() {}
	void Render() {}

    int GetX() const { return _x; }
    int GetY() const { return _y; }
    void SetPosition(int x, int y) { _x = x; _y = y; }

	const std::string& GetName() const { return _name; }

private:
    int _x, _y;
    std::string _name = "Unamed Unit";
};