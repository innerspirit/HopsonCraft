#include "World.h"

#include <iostream>
#include <SFML/Graphics.hpp>

#include "../Renderer/RMaster.h"

#include "../Player/Player.h"
#include "../Camera.h"

World::World(const Camera& camera, int32_t worldSize)
:   m_p_camera      (&camera)
,   m_worldSize     (worldSize)
{
    for (int32_t x = 0 ; x < m_worldSize; x++)
    {
        for (int32_t z = 0; z < m_worldSize; z++)
        {
            m_chunks.addChunk({x, z}, *this);
        }
    }
}

void World::checkPlayerBounds(Player& player)
{
    if (player.position.x < 0.2)
    {
        player.position.x = 0.2;
    }
    if (player.position.z < 0.2)
    {
        player.position.z = 0.2;
    }

    if (player.position.x + 0.2 > m_worldSize * CHUNK_SIZE - 0.2 )
    {
        player.position.x = m_worldSize * CHUNK_SIZE - 0.3;
    }
    if (player.position.z + 0.2 > m_worldSize * CHUNK_SIZE - 0.2 )
    {
        player.position.z = m_worldSize * CHUNK_SIZE - 0.3;
    }
}

void World::drawWorld(Renderer::Master& renderer, const Camera& camera)
{
    bool isMeshMade = false;

    for (int32_t y = 4 ; y >= 0; y--)
    {
        for (int32_t x = 0 ; x < m_worldSize; x++)
        {
            for (int32_t z = 0; z < m_worldSize; z++)
            {
                if (!m_chunks.get({x, y, z})->made)
                {
                    m_chunks.get({x, y, z})->makeMesh();
                    isMeshMade = true;
                    break;
                }
                else
                {
                    renderer.draw(*m_chunks.get({x, y, z}));
                }
            }
            if (isMeshMade)
            {
                break;
            }
        }
    }
}
