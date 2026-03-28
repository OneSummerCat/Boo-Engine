#include "freetype-mgr.h"
#include "../../platforms/platform.h"
#include "../gfx/gfx-mgr.h"
#include "../../log.h"
#include "../util/util-api.h"
#include "../assets/assets-manager.h"

namespace Boo
{

    FreetypeMgr::FreetypeMgr() : _fontPath(""),
                                 _fontSize(0),
                                 _fontBaseLine(0),
                                 _fontLineHeight(0),
                                 _face(nullptr),
                                 _ft(nullptr)
    {
#if defined(BOO_PLATFORM_WINDOWS)
        this->_fontPath = "C:\\Windows\\Fonts\\simhei.ttf";
#elif defined(BOO_PLATFORM_MACOS)
        this->_fontPath = "/System/Library/Fonts/STHeiti Light.ttc";
#else
        LOGI("[FreetypeMgr]: 其他系统");
        return;
#endif
    }
    FreetypeMgr *FreetypeMgr::getInstance()
    {
        static FreetypeMgr _instance;
        return &_instance;
    }
    void FreetypeMgr::init()
    {
        this->_fontSize = 50;
        this->_fontBaseLine = this->_fontSize * 0.9;
        this->_fontLineHeight = this->_fontSize * 1.1;
        std::cout << "fontSize: " << this->_fontSize << " fontBaseLine: " << this->_fontBaseLine << " fontLineHeight: " << this->_fontLineHeight << std::endl;
        // 初始化 FreeType
        if (FT_Init_FreeType(&this->_ft))
        {
            throw std::runtime_error("could not init FreeType Library");
        }
        // 加载字体

        if (FT_New_Face(this->_ft, this->_fontPath.c_str(), 0, &this->_face))
        {
            throw std::runtime_error("failed to load font");
        }
        // TextTexture textTexture{};
        // this->crateFont(textTexture, "你好,Hello Worldy", 20, 20);
        // this->_parseText("你q");
        // this->create("你奥好MJKHaddfayyiq");
    }
    void FreetypeMgr::create(const std::string &text,FontTexture &fontTexture)
    {
        // 解析文本,进行存储
        this->_parseText(text);
        std::string gfxUuid = "font-";
        std::vector<wchar_t> chars;
        std::wstring wstr = FmtUtil::utf8_to_wstring(text);
        // 遍历所有文字,确定贴图的大小
        uint32_t width = 0;
        uint32_t height = 0;
        for (wchar_t c : wstr)
        {
            if (this->_fontDataHash.find(c) == this->_fontDataHash.end())
            {
                continue;
            }
            gfxUuid += std::to_string(c);
            width += this->_fontDataHash[c].width;
            height = std::max(height, this->_fontDataHash[c].height);
        }
        std::vector<uint8_t> datas(width * height, 0);
        // 遍历所有文字,将其绘制到datas中
        // 每个文字开始前x的偏移量
        uint32_t x = 0;
        for (wchar_t c : wstr)
        {
            if (this->_fontDataHash.find(c) == this->_fontDataHash.end())
            {
                continue;
            }
            FontData &fontData = this->_fontDataHash[c];
            // 复制字体数据到datas
            for (uint32_t h = 0; h < fontData.height; h++)
            {
                for (uint32_t w = 0; w < fontData.width; w++)
                {
                    uint32_t glyphIndex = h * fontData.width + w;
                    uint8_t gray = fontData.datas[glyphIndex];
                    datas[h * width + (w + x)] = gray;
                }
            }
            x += fontData.width;
        }
        // FileUtil::saveAtlasAsPGM(text + ".ppm", datas, width, height);
        // GfxMgr::getInstance()->createTexture(gfxUuid, width, height,
        //                                  1, &datas);
        fontTexture.text = text;
        TextureAsset *texture = new TextureAsset(gfxUuid, "", "");
        texture->create(width, height, 1, datas, GfxTextureFormat::R8_UNORM);
        fontTexture.texture = texture;
    }

    void FreetypeMgr::_parseText(const std::string &text)
    {
        FT_Set_Pixel_Sizes(this->_face, 0, this->_fontSize);
        std::vector<wchar_t> chars;
        std::wstring wstr = FmtUtil::utf8_to_wstring(text);
        for (wchar_t c : wstr)
        {
            if (this->_fontDataHash.find(c) != this->_fontDataHash.end())
            {
                continue;
            }
            if (FT_Load_Char(this->_face, c, FT_LOAD_RENDER))
            {
                continue;
            }
            FT_GlyphSlot glyph = this->_face->glyph;
            uint32_t width = glyph->bitmap.width;     // 位图宽度（像素） 宽度是更具高度动态变化的
            uint32_t height = glyph->bitmap.rows;     // 位图高度（像素） 高度大于会在设置的size,但是说是有可能也会大于size,
            uint32_t bearingX = glyph->bitmap_left;   // 水平距离，即位图相对于原点的水平位置（像素）
            uint32_t bearingY = glyph->bitmap_top;    // 垂直距离，即位图相对于原点的垂直位置（像素）
            uint32_t advance = glyph->advance.x >> 6; // 水平预留值，即原点到下一个字形原点的水平距离（单位：1/64像素）

            int offsetY = _fontBaseLine - bearingY;                       // 基准线距离顶部的距离
            std::vector<uint8_t> datas(width * this->_fontLineHeight, 0); // 70 * width的像素数据
            // 复制位图数据到datas
            for (uint32_t y = 0; y < height; y++)
            {
                for (uint32_t x = 0; x < width; x++)
                {
                    uint32_t glyphIndex = y * width + x;
                    uint8_t gray = glyph->bitmap.buffer[glyphIndex];
                    datas[(y + offsetY) * width + x] = gray;
                }
            }
            // 存储字体数据
            FontData fontData{};
            fontData.width = width;
            fontData.height = uint32_t(this->_fontLineHeight);
            fontData.datas = datas;
            
            this->_fontDataHash[c] = fontData;
            // std::cout << "c: " << " width: " << width << " height: " << height << " bearingX: " << bearingX << " bearingY: " << bearingY << std::endl;
            // FileUtil::saveAtlasAsPGM(std::to_string(c) + ".ppm", datas, width, this->_fontLineHeight);
        }
    }

    // void FreetypeMgr::crateFont(TextTexture &textTexture, const std::string &str, uint32_t fontSize, uint32_t lineHeight)
    // {
    //     std::string textureUuid = "font-";
    //     FT_Set_Pixel_Sizes(this->_face, 0, fontSize);
    //     // 开启抗锯齿
    //     std::vector<wchar_t> chars;
    //     std::wstring wstr = FmtUtil::utf8_to_wstring(str);
    //     for (wchar_t c : wstr)
    //     {
    //         // 避免重复添加
    //         chars.push_back(c);
    //     }
    //     uint32_t x = 3;
    //     uint32_t y = 3;
    //     uint32_t atlasWidth = 6;
    //     uint32_t atlasHeight = 6;
    //     uint32_t datumLine = 0;              // 基准线
    //     uint32_t datumLine_below_height = 0; // 基准线下边的高度
    //     uint32_t space = fontSize * 0.1;     // 空格宽度
    //     // 提前与计算宽高
    //     for (wchar_t c : chars)
    //     {
    //         if (FT_Load_Char(this->_face, c, FT_LOAD_RENDER))
    //         {
    //             continue;
    //         }
    //         textureUuid += std::to_string(c);
    //         FT_GlyphSlot glyph = this->_face->glyph;
    //         // 最大宽度
    //         uint32_t width = glyph->bitmap.width;
    //         atlasWidth += width + space; // 加1像素间隔
    //         // 最大高度
    //         uint32_t height = glyph->bitmap.rows; // 文字的高度
    //         uint32_t top = glyph->bitmap_top;     // 文字的基准线距离顶部的距离
    //         datumLine = std::max(datumLine, top);
    //         if (height > top)
    //         {
    //             datumLine_below_height = std::max(datumLine_below_height, height - top);
    //         }
    //     }
    //     atlasHeight += datumLine + datumLine_below_height;
    //     std::vector<uint8_t> atlasData(atlasWidth * atlasHeight * 1, 0);
    //     for (wchar_t c : chars)
    //     {
    //         if (FT_Load_Char(this->_face, c, FT_LOAD_RENDER))

    //         {
    //             continue;
    //         }

    //         FT_GlyphSlot glyph = this->_face->glyph;
    //         uint32_t width = glyph->bitmap.width;
    //         uint32_t height = glyph->bitmap.rows;
    //         int offsetY = (datumLine - glyph->bitmap_top);
    //         // 复制字形数据到图集
    //         for (uint32_t i = 0; i < height; i++)
    //         {
    //             for (uint32_t j = 0; j < width; j++)
    //             {
    //                 // 计算当前文字的x和y坐标的颜色值  0或1
    //                 uint32_t glyphIndex = i * width + j;

    //                 uint8_t gray = glyph->bitmap.buffer[glyphIndex];
    //                 // 填充到atlasData
    //                 uint32_t _x = x + j;
    //                 uint32_t _y = y + i + offsetY; // 高度需要进行偏移
    //                 uint32_t atlasIndex = (_y)*atlasWidth + (_x);
    //                 if (atlasIndex < atlasData.size())
    //                 {
    //                     atlasData[atlasIndex] = gray;
    //                 }
    //             }
    //         }
    //         x += width + space; // 加1像素间隔
    //     }
    //     // FileUtil::saveAtlasAsPGM(textureUuid + ".ppm", atlasData, atlasWidth, atlasHeight);
    //     // 转换为RGBA格式
    //     std::vector<uint8_t> rgbaDatas(atlasWidth * atlasHeight * 4, 0);
    //     for (uint32_t i = 0; i < atlasWidth * atlasHeight; i++)
    //     {
    //         rgbaDatas[i * 4 + 0] = 255;
    //         rgbaDatas[i * 4 + 1] = 255;
    //         rgbaDatas[i * 4 + 2] = 255;
    //         rgbaDatas[i * 4 + 3] = atlasData[i] > 100 ? 255 : 0;
    //     }
    //     textureUuid += ".png";
    //     textTexture.uuid = textureUuid;
    //     textTexture.width = atlasWidth;
    //     textTexture.height = atlasHeight;
    //     textTexture.channels = 4;
    //     textTexture.datas = rgbaDatas;
    // }

    FreetypeMgr::~FreetypeMgr()
    {
        FT_Done_Face(this->_face);
        FT_Done_FreeType(this->_ft);
    }

} // namespace Boo
