#pragma once
#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include <codecvt>
#include <locale>
#include <algorithm>
#include <iostream>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "../math/vec2.h"

namespace Boo{
    class TextureAsset;
}

namespace Boo
{

    // 修改后的字符结构
    struct FontData
    {
        std::string uuid;
        uint32_t width;
        uint32_t height;
        uint32_t channels;
        std::vector<uint8_t> datas;
    };
    struct FontTexture
    {
        std::string text;
        TextureAsset *texture = nullptr;
    };

    class FreetypeMgr
    {
    private:
        FreetypeMgr(const FreetypeMgr &) = delete;            // 禁用拷贝构造
        FreetypeMgr &operator=(const FreetypeMgr &) = delete; // 禁用赋值操作符
        FreetypeMgr();                                         // 构造函数设为私有
        ~FreetypeMgr();                                        // 析构函数设为私有
    private:
        FT_Face _face;
        FT_Library _ft;
    private:
        std::string _fontPath;
        // 字体默认大小
        int _fontSize;
        // 字体默认基准线距离顶部的距离
        int _fontBaseLine;
        // 字体默认行高
        int _fontLineHeight;
        std::map<wchar_t, FontData> _fontDataHash;

    private:    
        /**
         * @brief 解析文本
         * 
         * @param text 
         */
        void _parseText(const std::string &text);
    public:
        static FreetypeMgr *getInstance();
        void init();
        void create(const std::string &text,FontTexture &fontTexture);
        // void crateFont(TextTexture &textTexture, const std::string &str, uint32_t fontSize, uint32_t lineHeight);
        // void clearFont(std::string key);
    };

} // namespace Boo
