#include "Vignette.h"

#include "../Display.h"
#include "../Renderer/RMaster.h"
#include "../Resource_Managers/Resource_Holder.h"

Vignette::Vignette()
{
    m_sprite.setSize({Display::get().getSize().x,
                      Display::get().getSize().y});
    m_sprite.setTexture(&getResources().getTexture(Texture_ID::Vignette));
}

void Vignette::draw(Renderer::Master& renderer)
{
    renderer.draw(m_sprite);
}