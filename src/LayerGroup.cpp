/*********************************************************************
Grant Gangi 2019
Matt Marchant 2023

tmxlite - Zlib license.

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.
*********************************************************************/

#include <pugixml.hpp>

#include <TiledForge/LayerGroup.hpp>
#include <TiledForge/FreeFuncs.hpp>
#include <TiledForge/ObjectGroup.hpp>
#include <TiledForge/ImageLayer.hpp>
#include <TiledForge/TileLayer.hpp>
#include <TiledForge/detail/Log.hpp>

TiledForge::LayerGroup::LayerGroup(const eastl::string& workingDir, const Vector2u& tileCount)
    : m_workingDir(workingDir),
    m_tileCount(tileCount)
{
}

//public
void TiledForge::LayerGroup::parse(const pugi::xml_node& node, Map* map)
{
    EASTL_ASSERT(map);
    eastl::string attribString = node.name();
    if (attribString != "group")
    {
        Logger::log("Node was not a group layer, node will be skipped.", Logger::Type::Error);
        return;
    }

    setName(node.attribute("name").as_string());
    setClass(node.attribute("class").as_string());
    setOpacity(node.attribute("opacity").as_float(1.f));
    setVisible(node.attribute("visible").as_bool(true));
    setOffset(node.attribute("offsetx").as_int(0), node.attribute("offsety").as_int(0));
    setSize(node.attribute("width").as_uint(0), node.attribute("height").as_uint(0));
    setParallaxFactor(node.attribute("parallaxx").as_float(1.f), node.attribute("parallaxy").as_float(1.f));

    eastl::string tintColour = node.attribute("tintcolor").as_string();
    if (!tintColour.empty())
    {
        setTintColour(colourFromString(tintColour));
    }

    // parse children
    for (const auto& child : node.children())
    {
        attribString = child.name();
        if (attribString == "properties")
        {
            for (const auto& p : child.children())
            {
                addProperty(p);
            }
        }
        else if (attribString == "layer")
        {
            m_layers.emplace_back(eastl::make_unique<TileLayer>(m_tileCount.x * m_tileCount.y));
            m_layers.back()->parse(child, map);
        }
        else if (attribString == "objectgroup")
        {
            m_layers.emplace_back(eastl::make_unique<ObjectGroup>());
            m_layers.back()->parse(child, map);
        }
        else if (attribString == "imagelayer")
        {
            m_layers.emplace_back(eastl::make_unique<ImageLayer>(m_workingDir));
            m_layers.back()->parse(child, map);
        }
        else if (attribString == "group")
        {
            m_layers.emplace_back(eastl::make_unique<LayerGroup>(m_workingDir, m_tileCount));
            m_layers.back()->parse(child, map);
        }
        else
        {
            LOG("Unidentified name " + attribString + ": node skipped", Logger::Type::Warning);
        }
    }
}
