#pragma once

namespace Boo
{

    class AssetBuiltin
    {
    private:
        void _initDefaultTextures();
        void _initDefaultMaterials();

    public:
        AssetBuiltin();
        void init();
        ~AssetBuiltin();
    };

} // namespace Boo