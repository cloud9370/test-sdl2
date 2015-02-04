#include "PNGLoader.h"
#include <png.h>

PNGLoader::PNGLoader(const char *file):
    m_strFilename(),
    m_iWidth(0),
    m_iHeight(0)
{
    if(file)
    {
        m_strFilename = file;
        load(file);
    }
}

PNGLoader::PNGLoader(std::string file):
    m_strFilename(file),
    m_iWidth(0),
    m_iHeight(0)
{
    load(m_strFilename.c_str());
}

PNGLoader::~PNGLoader()
{
}

void PNGLoader::load(const char *file)
{
    if(m_strFilename!=file)
    {
        m_strFilename = file;
    }
}
