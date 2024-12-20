/*********************************************************************
Matt Marchant 2016 - 2023
http://trederia.blogspot.com

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

#include <TiledForge/Map.hpp>
#include <TiledForge/ObjectGroup.hpp>
#include <TiledForge/LayerGroup.hpp>
#include <TiledForge/TileLayer.hpp>

#include <iostream>
#include <EASTL/array.h>
#include <EASTL/string.h>

void* operator new[](size_t size, const char* name, int flags, unsigned debugFlags, const char* file, int line)
{
    (void)name; (void)flags; (void)debugFlags; (void)file; (void)line;
    return new uint8_t[size];
}

void* operator new[](size_t size, size_t alignment, size_t offset, const char* name, int flags, unsigned debugFlags, const char* file, int line)
{
    (void)alignment; (void)offset; (void)name; (void)flags; (void)debugFlags; (void)file; (void)line;
    return new uint8_t[size];
}

namespace EA
{
    namespace StdC
    {
        EASTL_EASTDC_API int Vsnprintf(char* EA_RESTRICT buffer, size_t size, const char* EA_RESTRICT format, va_list args)
        {
            return vsnprintf(buffer, size, format, args);
        }
    }
}

namespace std
{
    inline std::ostream& operator<<(std::ostream& os, const eastl::string& str)
    {
        os << str.c_str();
        return os;
    }

    inline std::istream& operator>>(std::istream& is, eastl::string& str)
    {
        std::string temp;
        is >> temp;  
        str = eastl::string(temp.c_str());
        return is;
    }
}

namespace
{
    const eastl::array<eastl::string, 4u> LayerStrings =
    {
        eastl::string("Tile"),
        eastl::string("Object"),
        eastl::string("Image"),
        eastl::string("Group"),
    };
}

int main()
{
    TiledForge::Map map;

    if (map.load("../../maps/platform.tmx"))
    {
        std::cout << "Loaded Map version: " << map.getVersion().upper << ", " << map.getVersion().lower << std::endl;
        if (map.isInfinite())
        {
            std::cout << "Map is infinite.\n";
        }
        else
        {
            std::cout << "Map Dimensions: " << map.getBounds() << std::endl;
        }

        const auto& mapProperties = map.getProperties();
        std::cout << "Map class: " << map.getClass() << std::endl;

        std::cout << "Map tileset has " << map.getTilesets().size() << " tilesets" << std::endl;
        for (const auto& tileset : map.getTilesets()) 
        {
            std::cout << "Tileset: " << tileset.getName() << std::endl;
            std::cout << "Tileset class: " << tileset.getClass() << std::endl;
        }

        std::cout << "Map has " << mapProperties.size() << " properties" << std::endl;
        for (const auto& prop : mapProperties)
        {
            std::cout << "Found property: " << prop.getName() << std::endl;
            std::cout << "Type: " << int(prop.getType()) << std::endl;
        }

        std::cout << std::endl;

        const auto& layers = map.getLayers();
        std::cout << "Map has " << layers.size() << " layers" <<  std::endl;
        for (const auto& layer : layers)
        {
            std::cout << "Found Layer: " << layer->getName() << std::endl;
            std::cout << "Layer Type: " << LayerStrings[static_cast<std::int32_t>(layer->getType())] << std::endl;
            std::cout << "Layer Dimensions: " << layer->getSize() << std::endl;
            std::cout << "Layer Tint: " << layer->getTintColour() << std::endl;

            if (layer->getType() == TiledForge::Layer::Type::Group)
            {
                std::cout << "Checking sublayers" << std::endl;
                const auto& sublayers = layer->getLayerAs<TiledForge::LayerGroup>().getLayers();
                std::cout << "LayerGroup has " << sublayers.size() << " layers" << std::endl;
                for (const auto& sublayer : sublayers)
                {
                    std::cout << "Found Layer: " << sublayer->getName() << std::endl;
                    std::cout << "Sub-layer Type: " << LayerStrings[static_cast<std::int32_t>(sublayer->getType())] << std::endl;
                    std::cout << "Sub-layer Class: " << sublayer->getClass() << std::endl;
                    std::cout << "Sub-layer Dimensions: " << sublayer->getSize() << std::endl;
                    std::cout << "Sub-layer Tint: " << sublayer->getTintColour() << std::endl;

                    if (sublayer->getType() == TiledForge::Layer::Type::Object)
                    {
                        std::cout << sublayer->getName() << " has " << sublayer->getLayerAs<TiledForge::ObjectGroup>().getObjects().size() << " objects" << std::endl;
                    }
                    else if (sublayer->getType() == TiledForge::Layer::Type::Tile)
                    {
                        std::cout << sublayer->getName() << " has " << sublayer->getLayerAs<TiledForge::TileLayer>().getTiles().size() << " tiles" << std::endl;
                    }
                }
            }

            if(layer->getType() == TiledForge::Layer::Type::Object)
            {
                const auto& objects = layer->getLayerAs<TiledForge::ObjectGroup>().getObjects();
                std::cout << "Found " << objects.size() << " objects in layer" << std::endl;
                for(const auto& object : objects)
                {
                    std::cout << "Object " << object.getUID() << ", " << object.getName() <<  std::endl;
                    const auto& properties = object.getProperties();
                    std::cout << "Object has " << properties.size() << " properties" << std::endl;
                    for(const auto& prop : properties)
                    {
                        std::cout << "Found property: " << prop.getName() << std::endl;
                        std::cout << "Type: " << int(prop.getType()) << std::endl;
                    }

                    if (!object.getTilesetName().empty())
                    {
                        std::cout << "Object uses template tile set " << object.getTilesetName() << "\n";
                    }
                }
            }

            if (layer->getType() == TiledForge::Layer::Type::Tile)
            {
                const auto& tiles = layer->getLayerAs<TiledForge::TileLayer>().getTiles();
                if (tiles.empty())
                {
                    const auto& chunks = layer->getLayerAs<TiledForge::TileLayer>().getChunks();
                    if (chunks.empty())
                    {
                        std::cout << "Layer has missing tile data\n";
                    }
                    else
                    {
                        std::cout << "Layer has " << chunks.size() << " tile chunks.\n";
                    }
                }
                else
                {
                    std::cout << "Layer has " << tiles.size() << " tiles.\n";
                }
            }

            const auto& properties = layer->getProperties();
            std::cout << properties.size() << " Layer Properties:" << std::endl;
            for (const auto& prop : properties)
            {
                std::cout << "Found property: " << prop.getName() << std::endl;
                std::cout << "Type: " << int(prop.getType()) << std::endl;
            }
        }
    }
    else
    {
        std::cout << "Failed loading map" << std::endl;
    }

    std::cout << std::endl << "Press return to quit..." <<std::endl;
    std::cin.get();

    return 0;
}
