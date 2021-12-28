#pragma once

#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include "Model.h"
#include "AssetLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::vector<std::string> getNextLineAndSplitIntoTokens(std::istream& str)
{
    std::vector<std::string>   result;
    std::string                line;
    std::getline(str, line);

    std::stringstream          lineStream(line);
    std::string                cell;

    while (std::getline(lineStream, cell, ','))
    {
        result.push_back(cell);
    }
    // This checks for a trailing comma with no data after it.
    if (!lineStream && cell.empty())
    {
        // If there was a trailing comma then add an empty element.
        result.push_back("");
    }
    return result;
}



int LoadStaticMeshFile(std::string fileName, staticMesh& mesh)
{
    std::ifstream fs;
    fs.open(fileName, std::ios::in | std::ios::binary);
    fs.seekg(0, std::ios::end);
    const size_t LEN = fs.tellg();
    fs.seekg(0, std::ios::beg);

    //------------------
    //load tri indicies
    //------------------

    uint16_t* trilist = NULL;
    uint16_t trilist_count;

    uint8_t header = 0;
    fs.read((char*)&header, 1);

    if (header == uint8_t(0xAA))
    {
        uint32_t count = 0;
        fs.read((char*)&count, 4);

        trilist_count = count;
        trilist = new uint16_t[count];

        uint32_t readsize = trilist_count * sizeof(uint16_t);

        fs.read((char*)trilist, readsize);

        uint8_t end = 0;
        fs.read((char*)&end, 1);
        if (end != 0x00)
        {
            return 1;
        }
    }
    else
    {
        return 1;
    }

    mesh.trilist = trilist;

    //----------------
    //Load Vertex Data
    //----------------

    VertexData* vertexdata = NULL;
    uint32_t vertexdata_count;

    fs.read((char*)&header, 1);
    if (header == uint8_t(0xBB))
    {
        uint32_t count = 0;
        fs.read((char*)&count, 4);

        uint32_t vertexData_size = sizeof(VertexData);

        uint32_t ReadSize = count * vertexData_size;

        vertexdata_count = count;
        vertexdata = new VertexData[count];

        fs.read((char*)vertexdata, ReadSize);

        uint8_t end = 0;
        fs.read((char*)&end, 1);
        if (end != 0x00)
        {
            return 1;
        }
    }
    else
    {
        return 1;
    }

    for (size_t i = 0; i < vertexdata_count; i++)
    {
        vertexdata[i].m_normal_x = vertexdata[i].m_normal_x;
        vertexdata[i].m_normal_y = vertexdata[i].m_normal_y;
        vertexdata[i].m_normal_z = vertexdata[i].m_normal_z;

        //vertexdata[i].m_x = bx::round(vertexdata[i].m_x);
        //vertexdata[i].m_y = bx::round(vertexdata[i].m_y);
        //vertexdata[i].m_z = bx::round(vertexdata[i].m_z);
    }


    mesh.vertexdata = vertexdata;

    mesh.vbh = bgfx::createVertexBuffer(
        // Static data can be passed with bgfx::makeRef
        bgfx::makeRef(vertexdata, sizeof(VertexData) * vertexdata_count),
        VertexData::ms_decl
    );


    mesh.ibh = bgfx::createIndexBuffer(
        // Static data can be passed with bgfx::makeRef
        bgfx::makeRef(trilist, sizeof(uint16_t) * trilist_count)
    );

    size_t readpos = fs.tellg();

    fs.read((char*)&header, 1);
    if (header == uint8_t(0xDD))
    {
        assert("mesh load failed - fileformat");//error
    }



    fs.close();
}

const bgfx::Memory* AreadTexture(const char* filename) {
    /// Opens a file and returns a bgfx::Memory of the raw data. The lifetime of the data is controlled by bgfx
    std::ifstream fs(filename, std::ios::in | std::ios::binary);
    if (!fs.is_open()) {
        return NULL;
    }
    fs.seekg(0, std::ios::end);
    const size_t LEN = fs.tellg();
    fs.seekg(0, std::ios::beg);

    const bgfx::Memory* mem = bgfx::alloc(LEN);
    fs.read((char*)mem->data, LEN);
    fs.close();
    return mem;
}

int LoadTexture(std::string fileName, bgfx::TextureHandle& ret_tex)
{
    if (fileName.substr(fileName.find_last_of(".") + 1) == "png") // load png image
    {
        int x, y, n;
        unsigned char* image = stbi_load(fileName.c_str(), &x, &y, &n, 0);
        const bgfx::Memory* mem_image = bgfx::makeRef(image, x * y * n);
        //const bgfx::Memory* mem_image = bgfx::copy(image, x * y * n);
        //stbi_image_free(image);
        ret_tex = bgfx::createTexture2D(x, y, false, 1, bgfx::TextureFormat::RGBA8, 0 | BGFX_TEXTURE_RT
            | BGFX_SAMPLER_MIN_ANISOTROPIC
            | BGFX_SAMPLER_MAG_ANISOTROPIC
            | BGFX_SAMPLER_MIP_POINT
            | BGFX_SAMPLER_U_CLAMP
            | BGFX_SAMPLER_V_CLAMP, mem_image);

        return 1;
    }
    else if (fileName.substr(fileName.find_last_of(".") + 1) == "dds")    //if DDS
    {
        ret_tex = bgfx::createTexture(AreadTexture(fileName.c_str()));
        return 1;
    }
    else
    {
        return 0;
    }


}



void LoadUIFiles(std::unordered_map<std::string, texture>& TexturesMap)
{

    static const std::vector<std::string> ui_textures = { "ui_box2.png","cursor.png" };

    for (size_t i = 0; i < ui_textures.size(); i++)
    {
        if (TexturesMap.find(ui_textures[i]) == TexturesMap.end())
        {
            bgfx::TextureHandle texhandle;
            if (LoadTexture("Assets\\UI\\" + ui_textures[i], texhandle))
            {
                texture newtexture;
                newtexture.texh = texhandle;
                std::pair<std::string, texture> addtexture(ui_textures[i], newtexture);
                TexturesMap.insert(addtexture);
            }
            else
            {
                //texture cant be loaded
                //replace with debug texture?
            }
        }
    }

}

void LoadPropsFile(std::string fileName, std::unordered_map<std::string, texture>& TexturesMap, std::unordered_map<std::string, staticMesh>& StaticMeshMap, std::unordered_map<std::string, StaticProp>& StaticPropMap)
{
    std::filebuf fb;
    if (fb.open(fileName, std::ios::in))
    {
        std::istream is(&fb);

        if (is)
        {
            std::vector<std::string> line = getNextLineAndSplitIntoTokens(is); //read first header line
        }

        while (is)
        {
            std::vector<std::string> line = getNextLineAndSplitIntoTokens(is);
            if (line.size() == 3)
            {
                std::string name = line[0];
                std::string meshfile = line[1];
                std::string texturefile = line[2];

                //-----------------------------
                //load texture in map
                //if texture not already loaded
                //-----------------------------

                if (TexturesMap.find(texturefile) == TexturesMap.end())
                {
                    bgfx::TextureHandle texhandle;
                    if (LoadTexture("Assets\\Texture\\" + texturefile, texhandle))
                    {
                        texture newtexture;
                        newtexture.texh = texhandle;
                        std::pair<std::string, texture> addtexture(texturefile, newtexture);
                        TexturesMap.insert(addtexture);
                    }
                    else
                    {
                        //texture cant be loaded
                        //replace with debug texture?
                    }
                }

                //---------------------------
                //load meshdata if not in map
                //---------------------------

                if (StaticMeshMap.find(meshfile) == StaticMeshMap.end())
                {
                    staticMesh Meshcontainer;
                    if (LoadStaticMeshFile("Assets\\Mesh\\" + meshfile, Meshcontainer))
                    {
                        std::pair<std::string, staticMesh> addmesh(meshfile, Meshcontainer);
                        StaticMeshMap.insert(addmesh);
                    }

                }


                //---------------------------
                //Add Prop To List
                //---------------------------
                StaticProp NewProp;
                NewProp.textureName = texturefile;
                NewProp.meshName = meshfile;
                staticMesh f_mesh = StaticMeshMap[NewProp.meshName];
                texture f_tex = TexturesMap[NewProp.textureName];
                NewProp.p_texture = TexturesMap[NewProp.textureName];
                NewProp.p_Meshcontainer = StaticMeshMap[NewProp.meshName];
                std::pair<std::string, StaticProp> addprop(name, NewProp);
                StaticPropMap.insert(addprop);
            }




        }
        fb.close();
    }





    //read props file
    //create prop entity if doesnt already exist?
    //check if mesh already loaded
    //load if not
    //check if texture already loaded
    //load if not
}

void LoadFontFile(std::string fileName, std::unordered_map<std::string, texture>& TexturesMap, pixel_font& fontstruct)
{
    std::filebuf fb;
    if (fb.open(fileName, std::ios::in))
    {
        std::istream is(&fb);

        if (is)
        {
            std::vector<std::string> line = getNextLineAndSplitIntoTokens(is); //read first header line

            std::string texturefile = line[1];

            if (TexturesMap.find(texturefile) == TexturesMap.end())
            {
                bgfx::TextureHandle texhandle;
                if (LoadTexture("Assets\\Font\\" + texturefile, texhandle))
                {
                    texture newtexture;
                    newtexture.texh = texhandle;
                    std::pair<std::string, texture> addtexture(texturefile, newtexture);
                    TexturesMap.insert(addtexture);
                }
                else
                {
                    //texture cant be loaded
                    //replace with debug texture?
                }
            }


            fontstruct.texhandle = TexturesMap[texturefile].texh;
            fontstruct.glyph_height = atoi(line[2].c_str());
            fontstruct.textureWidth = atoi(line[3].c_str());
            fontstruct.textureHeight = atoi(line[4].c_str());

        }

        while (is)
        {
            std::vector<std::string> line = getNextLineAndSplitIntoTokens(is);

            if (line.size() == 4)
            {
                tex_coord read_tex_coord;
                read_tex_coord.x = atoi(line[1].c_str());
                read_tex_coord.y = atoi(line[2].c_str());

                fontstruct.glyph_tex_cord.push_back(read_tex_coord);
                fontstruct.glyph_width.push_back(atoi(line[3].c_str()));
            }
        }
    }
}