#include "TileFactory.hpp"
#include "Tile.hpp"

//#include "GrassTile.cpp"
//#include "PathTile.cpp"
//
//#include "PathTurnTileD.cpp"
//#include "PathTurnTileC.cpp"
//#include "PathTurnTileB.cpp"
//#include "PathTurnTileA.cpp"

class TileCreator : public TileFactory {
public:
    TileCreator() = default;

    Tile* createTower(int id) override {
        switch (id) {
            case(0): return new GrassTile;
            case(1): return new PathTile;
            case(2): return new PathTurnTileA;
            case(3): return new PathTurnTileB;
            case(4): return new PathTurnTileC;
            case(5): return new PathTurnTileD;
            case(6): return new RockTile;

            case(7): return new WaterTileA;
            case(8): return new WaterTileB;
            case(9): return new WaterTileC;
            case(10): return new WaterCornerTileA;
            case(11): return new WaterCornerTileB;
            case(12): return new WaterCornerTileC;
            case(13): return new WaterCornerTileD;
            case(14): return new WaterSideTileA;
            case(15): return new WaterSideTileB;
            case(16): return new WaterSideTileC;
            case(17): return new WaterSideTileD;
            case(18): return new WaterTurnTileA;
            case(19): return new WaterTurnTileB;
            case(20): return new WaterTurnTileC;
            case(21): return new WaterTurnTileD;
            default: return new GrassTile();
        }
    }

};
