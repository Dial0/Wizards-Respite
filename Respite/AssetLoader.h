#pragma once

#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include "Model.h"
#include "PixelFont.h"


std::vector<std::string> getNextLineAndSplitIntoTokens(std::istream& str);

struct staticMesh
{
    uint16_t* trilist;
    VertexData* vertexdata;
    bgfx::VertexBufferHandle vbh;
    bgfx::IndexBufferHandle ibh;
};

struct animatedMesh
{
    uint16_t* trilist;
    //animated vertexdata
    //bonesmatrix
    bgfx::VertexBufferHandle vbh;
    bgfx::IndexBufferHandle ibh;
};

struct texture
{
    bgfx::TextureHandle texh;
};

struct StaticProp
{
    std::string textureName;
    texture p_texture;

    std::string meshName;
    staticMesh p_Meshcontainer;
};

int LoadStaticMeshFile(std::string fileName, staticMesh& mesh);

const bgfx::Memory* AreadTexture(const char* filename);

int LoadTexture(std::string fileName, bgfx::TextureHandle& ret_tex);



void LoadUIFiles(std::unordered_map<std::string, texture>& TexturesMap);

void LoadPropsFile(std::string fileName, std::unordered_map<std::string, texture>& TexturesMap, std::unordered_map<std::string, staticMesh>& StaticMeshMap, std::unordered_map<std::string, StaticProp>& StaticPropMap);

void LoadFontFile(std::string fileName, std::unordered_map<std::string, texture>& TexturesMap, pixel_font& fontstruct);