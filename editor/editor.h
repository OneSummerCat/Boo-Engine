#pragma once
#include <iostream>
#include <string>
#include <filesystem>

#include "ipc/editor-ipc.h"

class Engine;
class Scene;
class Node;
class Sprite;
class EditorLayout;
// class EditorHierarchy;
// class EditorAssets;
// class EditorProperty;

class Editor
{
private:
    Editor();
    ~Editor();
    Editor(const Editor &) = delete;            // 禁用拷贝构造
    Editor &operator=(const Editor &) = delete; // 禁用赋值操作符

    EditorLayout *_editorLayout;

    bool _alphaAnimOK = false;
    bool _loadComplete = false;
    void _initEditorRes();
    void _initEditorLayout();
    void _onAlphaAnimOK();
    void _onLoadCallBack(const int complete, const int all, const float progress);
    void _launchEditor();

    /*EditorHierarchy *_hierarchy;
   EditorAssets *_assets;
   EditorProperty *_property;

   void _initHierarchy();
   void _initAssets();
   void _initProperty();

   void _onHierarchyRootUpdate(const EventValue& value);*/
public:
    static Editor *getInstance();
    /**
     * @brief 初始化
     */
    void init();
    /**
     * @brief 更新
     */
    void update(float dt);

    void destroy();

    /* void setHierarchyRoot(Scene *scene);
     void setHierarchyNode(Node *node);
     void setAssetsRoot(std::string root);*/
};
