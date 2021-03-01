#include <iostream>
#include <random>
/**
 * copyright @hhhxiao
 * 
 */
/*
 * A simple mt19937  random number generator
 * only give the fist N numbers
 */
#define INIT(L, R, OFF) L = 0x6c078965 * ((R)^ (R >> 30u)) + (OFF)
template<int N>
struct PartyMT {
    uint32_t head_arr[N + 1]{};
    uint32_t last_arr[N]{};
    uint32_t result[N]{};

    void reset(uint32_t seed) {
        head_arr[0] = seed;
        for (int i = 1; i < N + 1; i++)
            INIT(head_arr[i], head_arr[i - 1], i);
        uint32_t temp = head_arr[N];
        for (int i = N; i < 397; i++)
            INIT(temp, temp, i + 1);
        last_arr[0] = temp;
        for (int i = 1; i < N + 1; ++i)
            INIT(last_arr[i], last_arr[i - 1], i + 397);
        for (int i = 0; i < N; i++) {
            temp = (head_arr[i] & 0x80000000) + (head_arr[i + 1] & 0x7fffffffu);
            head_arr[i] = (temp >> 1u) ^ last_arr[i];
            if (temp % 2 != 0)
                head_arr[i] = head_arr[i] ^ 0x9908b0df;
        }

        for (int i = 0; i < N; ++i) {
            uint32_t y = head_arr[i];
            y = y ^ y >> 11u;
            y = y ^ y << 7u & 2636928640u;
            y = y ^ y << 15u & 4022730752u;
            y = y ^ y >> 18u;
            result[i] = y;
        }
    }

    explicit PartyMT(uint32_t seed) {
        reset(seed);
    }

    PartyMT() = delete;
};

struct StructureConfig {
    int spacing;
    int spawnRange;
    int salt;
};

struct ChunkPos {
    int x;
    int z;
};
enum StructureType {
    PILLAGER = 165745296,
    RANDOM = 14357617,
    OCEAN = 10387313,
    NETHER_FEATURE = 30084232
};

const StructureConfig RANDOM_SCATTERED = {32, 24, RANDOM}; //女巫房子，丛林，沙漠神殿
//const FeatureConfig BURIED_TREASURE = {4, 2, 16842397}; //海底宝藏
const StructureConfig PILLAGER_OUTPOST = {80, 56, PILLAGER}; //哨塔
const StructureConfig VILLAGE = {27, 17, 10387312}; //村庄
//const FeatureConfig WOODLAND_MANSION = {80, 60, 10387319}; //林地府邸
//const FeatureConfig END_CITY = {20, 9, 10387313}; //末地城市
const StructureConfig OCEAN_MONUMENT = {32, 27, OCEAN}; //海底神殿
const StructureConfig NETHER_FEATURE = {30, 26, 30084232};

static uint32_t getSeed(int worldSeed, int salt, int x, int z) {
    return salt + worldSeed - 245998635 * z - 1724254968 * x;
}
//OCEAN_MONUMENT PILLAGER_OUTPOST
int isFeatureChunk(const StructureConfig &cfg, uint32_t worldSeed, ChunkPos &chunkPos) {
    if (chunkPos.x < 0) chunkPos.x -= cfg.spacing - 1;
    if (chunkPos.z < 0)chunkPos.z -= cfg.spacing - 1;
    uint32_t seed = getSeed(worldSeed, cfg.salt, chunkPos.x / cfg.spacing, chunkPos.z / cfg.spacing);
    PartyMT<4> mt(seed);
    uint32_t r1 = mt.result[0] % cfg.spawnRange;
    uint32_t r2 = mt.result[1] % cfg.spawnRange;
    uint32_t r3 = mt.result[2] % cfg.spawnRange;
    uint32_t r4 = mt.result[3] % cfg.spawnRange;
    int x_offset = chunkPos.x % cfg.spacing;
    int z_offset = chunkPos.z % cfg.spacing;
    if (x_offset < 0) x_offset += cfg.spacing - 1;
    if (z_offset < 0) z_offset += cfg.spacing - 1;
    return (r1 + r2) / 2 == x_offset && (r3 + r4) / 2 == z_offset;
}

//RANDOM_SCATTERED NETHER_FEATURE
int isFeatureChunk2(const StructureConfig &cfg, uint32_t worldSeed, ChunkPos &chunkPos) {
    if (chunkPos.x < 0) chunkPos.x -= cfg.spacing - 1;
    if (chunkPos.z < 0)chunkPos.z -= cfg.spacing - 1;
    uint32_t seed = getSeed(worldSeed, cfg.salt, chunkPos.x / cfg.spacing, chunkPos.z / cfg.spacing);
    PartyMT<2> mt(seed);
    uint32_t r1 = mt.result[0] % cfg.spawnRange;
    uint32_t r2 = mt.result[1] % cfg.spawnRange;
    int xOff = chunkPos.x % cfg.spacing;
    int zOff = chunkPos.z % cfg.spacing;
    if (xOff < 0) xOff += cfg.spacing - 1;
    if (zOff < 0) zOff += cfg.spacing - 1;
    return r1 == xOff && r2 == zOff;
}




/*地狱堡垒 几乎100%准确 */
/*1.14才能用*/
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