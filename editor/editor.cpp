#include "editor.h"
#include "../engine/engine.h"
#include "../engine/core/game.h"
#include "../engine/core/assets/assets-manager.h"
#include "editor-layout.h"
#include "../engine/core/utils/time-util.h"

// #include "../engine/core/renderer/scene.h"
// #include "../engine/core/renderer/ui/sprite.h"
// #include "../engine/core/global/event.h"
// #include "../engine/core/global/global.h"
// #include "hierarchy/editor-hierarchy.h"
// #include "assets/editor-assets.h"
// #include "property/editor-property.h"

Editor::Editor()
{
}
Editor::~Editor()
{
}
Editor *Editor::getInstance()
{
	static Editor instance;
	return &instance;
}

void Editor::init()
{
	this->_initEditorLayout();
	this->_initEditorRes();
}
void Editor::_initEditorLayout()
{
	this->_editorLayout = new EditorLayout();
	/*this->_initHierarchy();
	this->_initAssets();
	this->_initProperty();*/

	/*  EditorIpc::getInstance()->on(IpcEvent::UPDATE_HIERARCHY_ROOT, &Editor::_onHierarchyRootUpdate, this);
	  EditorIpc::getInstance()->send(IpcEvent::UPDATE_HIERARCHY_ROOT,0);*/
}
void Editor::_initEditorRes()
{
	// 先加载公用resources文件,
	const std::string &root = Game::getInstance()->assetsManager()->root();
	std::filesystem::path fullPath = std::filesystem::path(root) / "resources";
	std::vector<std::string> paths;
	for (const auto &entry : std::filesystem::recursive_directory_iterator(fullPath))
	{
		std::filesystem::path path = std::filesystem::relative(entry.path(), std::filesystem::path(root));
		paths.push_back(path.generic_string());
		std::cout << "add resource " << path.generic_string() << std::endl;
	}
	Game::getInstance()->assetsManager()->loadListAsync(paths, [](const int complete, const int all, const float progress)
														{ std::cout << "load resources " << complete << " / " << all << " progress " << progress << std::endl; });
}

// void Editor::_initHierarchy()
//{
//     Node2D *layout = this->_editorLayout->getHierarchy();
//     this->_hierarchy = new EditorHierarchy(layout);
// }
// void Editor::_initAssets()
//{
//     Node2D *layout = this->_editorLayout->getAssets();
//     this->_assets = new EditorAssets(layout);
// }
// void Editor::_initProperty()
//{
//     Node2D *layout = this->_editorLayout->getProperty();
//     this->_property = new EditorProperty(layout);
// }
// void Editor::_onHierarchyRootUpdate(const EventValue &value)
//{
//     if (this->_hierarchy != nullptr)
//     {
//         std::cout << "EditorHierarchy Update Tree" << std::endl;
//         this->_hierarchy->updateRoot();
//     }
// }
// void Editor::setHierarchyRoot(Scene *scene)
//{
//     if (this->_hierarchy != nullptr)
//     {
//         this->_hierarchy->setScene(scene);
//     }
// }
// void Editor::setHierarchyNode(Node *node)
//{
//     if (this->_hierarchy != nullptr)
//     {
//         this->_hierarchy->setNode(node);
//     }
// }
// void Editor::setAssetsRoot(std::string root)
//{
//     if (this->_assets != nullptr)
//     {
//         this->_assets->setRoot(root);
//     }
// }

void Editor::update(float dt)
{
	if (this->_editorLayout != nullptr)
	{
		this->_editorLayout->update(dt);
	}
	// this->_hierarchy->update(dt);
	// this->_assets->update(dt);
	// this->_property->update(dt);*/
}