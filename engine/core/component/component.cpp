#include "component.h"
#include "../../boo.h"
#include "../../log.h"

namespace Boo
{

    Component::Component(std::string name, Node *node, std::string uuid) : _isAwaked(false),
                                                                           _uuid(uuid),
                                                                           _name(name),
                                                                           _layer(ComponentLayer::Default),
                                                                           _node(node),
                                                                           _isEnabled(true),
                                                                           _isEnabledInHierarchy(false)

    {
        if (this->_uuid.size() <= 0)
        {
            this->_uuid = UuidUtil::generateUUID();
        }
    }
    void Component::setEnabled(bool enabled)
    {
        this->_isEnabled = enabled;
        this->setNodeActiveInHierarchy(this->_node->getActiveInHierarchy());
    }

    void Component::setNodeActiveInHierarchy(bool isActiveInHierarchy)
    {
        // LOGI("Component::setNodeActiveInHierarchy: %s, isActiveInHierarchy: %d", this->_name.c_str(), isActiveInHierarchy);

        bool isEnableInHierarchy = isActiveInHierarchy && this->_isEnabled;
        // LOGI("Component::setNodeActiveInHierarchy: %s, isEnableInHierarchy: %d", this->_name.c_str(), isEnableInHierarchy);
        if (this->_isEnabledInHierarchy == isEnableInHierarchy)
        {
            return; // 状态未改变
        }
        this->_isEnabledInHierarchy = isEnableInHierarchy;
        if (this->_isEnabledInHierarchy)
        {
            if (!this->_isAwaked)
            {
                this->_isAwaked = true;
                this->Awake();
            }
            this->Enable();
        }
        else
        {
            this->Disable();
        }
    }
    /**
     * @brief 组件唤醒函数
     * 组件被添加到节点后,第一次激活会调用Awake函数
     */
    void Component::Awake()
    {
        // std::cout << "Component::Awake" << std::endl;
        // LOGI("Component::Awake: %s", this->_name.c_str());
    }
    /**
     * @brief 组件启用函数
     * 组件被激活后,会调用Enable函数
     */
    void Component::Enable()
    {
        // std::cout << "Component::Enable" << std::endl;
    }

    void Component::Update(float deltaTime)
    {
    }
    void Component::LateUpdate(float deltaTime)
    {
    }
    /**
     * @brief 组件禁用函数
     * 组件被禁用后,会调用Disable函数
     */
    void Component::Disable()
    {
        // std::cout << "Component::Disable" << std::endl;
    }

    void Component::destroy()
    {
        // std::cout << "Component::destroy" << std::endl;
        game->addCompClearCaches(this);
    }

    Component::~Component()
    {
        // std::cout << "Component::~destructor" << std::endl;
    }

} // namespace Boo
