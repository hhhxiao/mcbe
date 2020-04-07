#include <iostream>
#include <random>
/**
 * copyright @hhhxiao
 * 
 */

int64_t calSeed(int64_t worldSeed, int64_t chunkY, int64_t chunkX, int val) {
    return worldSeed + val - 245998635L * chunkX - 1724254968L * chunkY;
}

struct FeatureConfig {
    int c1;
    int c2;
    int c3;
};

/*BE中的结构生成参数，和java版的很相似*/
const FeatureConfig RANDOM_SCATTERED = {32, 24, 14357617}; //女巫房子，丛林，沙漠神殿
const FeatureConfig BURIED_TREASURE = {4, 2, 16842397}; //海底宝藏
const FeatureConfig PILLAGER_OUTPOST = {80, 56, 165745296}; //哨塔
const FeatureConfig VILLAGE = {27, 17, 10387312}; //村庄
const FeatureConfig WOODLAND_MANSION = {80, 60, 10387319}; //林地府邸
const FeatureConfig END_CITY = {20, 9, 10387313}; //末地城市
const FeatureConfig OCEAN_MONUMENT = {32, 27, 10387313}; //海底神殿

/*
*能给出正坐标内的可能点位(因为没进行群戏判断)
*游戏内部的地形生成可能还要距离限制
 开头两行是源码用来修正负数坐标的，但是不管用
*/


bool baseFinder(const FeatureConfig &feature, int worldSeed, int chunkPosX, int chunkPosY) {

    if (chunkPosX < 0)chunkPosX -= (feature.c1 - 1);
    if (chunkPosY < 0)chunkPosY -= (feature.c1 - 1);
    auto seed = calSeed(worldSeed, chunkPosX / feature.c1, chunkPosY / feature.c1, feature.c3);
    std::mt19937 mt(seed);
    auto mod = feature.c2;
    auto r1 = mt() % mod;
    auto r2 = mt() % mod;
    auto r3 = mt() % mod;
    auto r4 = mt() % mod;
    auto Xoff = chunkPosX % feature.c1;
    auto Yoff = chunkPosY % feature.c1;
    return (r1 + r2) / 2 == Xoff && (r3 + r4) / 2 == Yoff;
}


/*地狱堡垒 几乎100%准确 */
bool netherFinder(int worldSeed, int x, int y) {
    int v9 = x / 16;
    int v8 = y / 16;
    int seed = worldSeed ^(16 * v8) ^v9;
    std::mt19937 mt(seed);
    int a = mt();
    if (mt() % 3) {
        return false;
    } else {
        int newX = (int) (mt() % 8) + 16 * v9 + 4;
        int newY = (int) (mt() % 8) + 16 * v8 + 4;
        return newX == x && newY == y;
    }
}


/*废弃矿洞 */
bool Mineshaft(int worldSeed, int chunkX, int chunkY) {
    std::mt19937 mt1(worldSeed);
    int v11 = mt1();
    int v10 = mt1();
    int newSeed = worldSeed ^(v10 * chunkY) ^(v11 * chunkX);
    std::mt19937 mt2(newSeed);
    mt2.discard(2);
    int c = mt2();
    auto v16 = abs(chunkX);
    auto v15 = abs(chunkY);
    return c % 80 < std::max(v16, v15);
}

/** 
 * 遗迹有独特的生成算法，未列出
 */
int main() {
    return 0;
}