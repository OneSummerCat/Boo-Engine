#pragma once

namespace Boo
{

    class AssetBuiltin
    {
    private:
        void _initDefaultTextureLogo();
        void _initDefaultShaderUI();
        void _initDefaultShaderUnlit();
        void _initDefaultMaterialUI();
        void _initDefaultMaterialUnlit();

    public:
        AssetBuiltin();
        void init();
        ~AssetBuiltin();
    };

} // namespace Boo