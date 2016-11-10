#include "Chunk.h"

#include "D_Blocks.h"
#include "Random.h"
#include "Hasher.h"
#include "Noise_Generator.h"

namespace
{
    struct Biome
    {
        Biome(Block_t& surface, int depth, int treeChance, std::vector<Block_Location>& treeLocations, bool isHot, bool hasPlants)
        :   surfaceBlock    (&surface)
        ,   depth           (depth)
        ,   treeChance      (treeChance)
        ,   treeLocations   (&treeLocations)
        ,   isHot           (isHot)
        ,   hasPlants       (hasPlants)
        {}

        const Block_t* surfaceBlock;
        const int depth;
        const int treeChance;
        std::vector<Block_Location>* treeLocations;
        const bool isHot;
        const bool hasPlants;
    };

    const Biome& getBiome (int biomeValue, Biome& forest, Biome& fields, Biome& desert, Biome& snow)
    {
        if (biomeValue > 250) return snow;
        else if (biomeValue <= 250 && biomeValue > 165 ) return fields;
        else if (biomeValue <= 165 && biomeValue >= 90) return forest;
        else return desert;
    }
}

void Chunk::generateChunk(int maxHeight, const std::vector<int>& heightMap, const std::vector<int>& biomeMap)
{

    static const auto noiseSeed = Noise_Generator::getSeed();

    Biome forest(Block::grass,  0,  70,     m_blocks.m_treeLocations,    false, true);
    Biome desert(Block::sand,   5,  1000,   m_blocks.m_cactusLocations,  true, false);
    Biome fields(Block::grass,  0,  1000,   m_blocks.m_treeLocations,    false, true);
    Biome snow  (Block::snow,   3,  1000,   m_blocks.m_treeLocations,    false, false);

    for (int y = 0; y < maxHeight + 1 ; y++)
    {
        for (char x = 0 ; x < SIZE ; x++)
        {
            for (char z = 0 ; z < SIZE ; z++)
            {
                Random::setSeed(Hasher::hash(x + noiseSeed,
                                             y + noiseSeed,
                                             z + noiseSeed)); //This for trees, so they gen in the same place


                auto* biome = &getBiome(biomeMap.at(x * SIZE + z), forest, fields, desert, snow);

                int h = heightMap.at (x * SIZE + z);
                if (y > h)
                {
                    y <= WATER_LEVEL ?
                        m_blocks.qSetBlock({x, y, z}, Block::water) :
                        m_blocks.qSetBlock({x, y, z}, Block::air);
                }
                else if (y <= h && y >= h - biome->depth)
                {
                    if (y > BEACH_LEVEL) //Top levels
                    {
                        if ( y <= SNOW_LEVEL || biome->isHot )
                        {
                            m_blocks.qSetBlock({x, y, z}, *biome->surfaceBlock );

                            if ( Random::integer(0, biome->treeChance) == 1  &&
                               (x > 4 && x < SIZE - 4) &&
                               (z > 4 && z < SIZE - 4)
                                && (y <= SNOW_LEVEL - 10 || biome->isHot))
                            {
                                biome->treeLocations->emplace_back(x, y, z);    //Trees
                            }
                            if(biome->hasPlants)
                            {
                                if(Random::integer(1, 80) == 1)
                                    m_blocks.m_floraLocations.insert(std::make_pair(Block_Location{x, y + 1, z}, &Block::rose));
                                if(Random::integer(1, 50) == 1)
                                    m_blocks.m_floraLocations.insert(std::make_pair(Block_Location{x, y + 1, z}, &Block::tallGrass));
                            }

                        }
                        else
                        {
                            Random::integer(y, maxHeight + 10) < y + 5?
                                m_blocks.qSetBlock({x, y, z}, Block::snow) :
                                m_blocks.qSetBlock({x, y, z}, *biome->surfaceBlock );
                        }

                    }
                    else if (y <= BEACH_LEVEL && y >= WATER_LEVEL) //Beach
                    {
                        m_blocks.qSetBlock({x, y, z}, Block::sand);
                    }
                    else
                    {
                        Random::integer(0, 10) < 6 ?
                            m_blocks.qSetBlock({x, y, z}, Block::sand)   :
                            m_blocks.qSetBlock({x, y, z}, Block::dirt);
                    }
                }
                else  if (y < h && y > h - 5)
                {
                    m_blocks.qSetBlock({x, y, z}, Block::dirt);
                }
                else
                {
                    m_blocks.qSetBlock({x, y, z}, Block::stone);
                }
            }
        }
    }
}
