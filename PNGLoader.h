#ifndef PNGLOADER_H_
#define PNGLOADER_H_

#include <string>

class PNGLoader
{
private:
    std::string m_strFilename;

    int m_iWidth;
    int m_iHeight;
public:
    PNGLoader(const char *file=0);
    PNGLoader(std::string file);
    virtual ~PNGLoader();

    virtual void load(const char *file);
};

#endif /* PNGLOADER_H_ */
