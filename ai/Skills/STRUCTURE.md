# 技能目录结构

ai/Skills/
├── Basic/                    # 基础 Skills (L1)
│   ├── Game/
│   │   ├── SKILL.md          # 技能描述
│   │   ├── Resources/
│   │   │   ├── API.md        # API 文档
│   │   │   ├── EXAMPLE.md    # 示例代码
│   │   │   └── FORMS.md     # 输入输出格式
│   │   └── Scripts/          # 脚本目录
│   ├── Scene/                  # 场景系统
│   │   ├── SKILL.md
│   │   ├── Resources/
│   │   │   ├── API.md
│   │   │   ├── EXAMPLE.md
│   │   │   └── FORMS.md
│   │   └── Scripts/
│   ├── Component/             # 组件系统
│   │   ├── SKILL.md
│   │   ├── Resources/
│   │   │   ├── API.md
│   │   │   ├── EXAMPLE.md
│   │   │   └── FORMS.md
│   │   └── Scripts/
│   ├── Boo/             # 命名空间
│   │   ├── SKILL.md
│   │   ├── Resources/
│   │   │   ├── API.md
│   │   │   ├── EXAMPLE.md
│   │   │   └── FORMS.md
│   │   └── Scripts/
│   ├── Input/                # 新增
│   │   ├── SKILL.md
│   │   ├── Resources/
│   │   │   ├── API.md
│   │   │   ├── EXAMPLE.md
│   │   │   └── FORMS.md
│   │   └── Scripts/
│   ├── Event/                # 新增
│   │   ├── SKILL.md
│   │   ├── Resources/
│   │   │   ├── API.md
│   │   │   ├── EXAMPLE.md
│   │   │   └── FORMS.md
│   │   └── Scripts/
│   └── Assets/               # 新增
│       ├── SKILL.md
│       ├── Resources/
│       │   ├── API.md
│       │   ├── EXAMPLE.md
│       │   └── FORMS.md
│       └── Scripts/
├── Composite/                # 组合 Skills (L2)
│   ├── CreateInteractiveButton/
│   │   ├── SKILL.md
│   │   ├── Resources/
│   │   │   ├── API.md
│   │   │   ├── EXAMPLE.md
│   │   │   └── FORMS.md
│   │   ├── Scripts/
│   │   ├── .temp/             # 临时文件目录
│   │   └── config.json       # JSON 配置文件
│   ├── SetupGameScene/
│   │   ├── SKILL.md
│   │   ├── Resources/
│   │   │   ├── API.md
│   │   │   ├── EXAMPLE.md
│   │   │   └── FORMS.md
│   │   ├── Scripts/
│   │   ├── .temp/
│   │   └── config.json
│   ├── CreateMovingSprite/
│   │   ├── SKILL.md
│   │   ├── Resources/
│   │   │   ├── API.md
│   │   │   ├── EXAMPLE.md
│   │   │   └── FORMS.md
│   │   ├── Scripts/
│   │   ├── .temp/
│   │   └── config.json
│   └── CreateCollectible/
│       ├── SKILL.md
│       ├── Resources/
│       │   ├── API.md
│       │   ├── EXAMPLE.md
│       │   └── FORMS.md
│       ├── Scripts/
│       ├── .temp/
│       └── config.json
└── Template/                 # 游戏模板 Skills (L3)
    ├── CreateFlappyBird/
    │   ├── SKILL.md
    │   ├── Resources/
    │   │   ├── API.md
    │   │   ├── EXAMPLE.md
    │   │   └── FORMS.md
    │   ├── Scripts/
    │   ├── .temp/
    │   └── config.json
    ├── CreatePlatformer/
    │   ├── SKILL.md
    │   ├── Resources/
    │   │   ├── API.md
    │   │   ├── EXAMPLE.md
    │   │   └── FORMS.md
    │   ├── Scripts/
    │   ├── .temp/
    │   └── config.json
    └── CreateRunner/
        ├── SKILL.md
        ├── Resources/
        │   ├── API.md
        │   ├── EXAMPLE.md
        │   └── FORMS.md
        ├── Scripts/
        ├── .temp/
        └── config.json
# 文件结构说明 
## 基础 Skills (L1)
- SKILL.md ：技能的基本信息、功能描述、使用方法
- Resources/API.md ：API 文档，包含所有可用方法和参数
- Resources/EXAMPLE.md ：代码示例，展示如何使用该技能
- Resources/FORMS.md ：输入输出格式说明
- Scripts/ ：脚本目录，存放技能相关的脚本文件
## 组合 Skills (L2)
- SKILL.md ：技能的基本信息、功能描述、使用方法
- Resources/API.md ：API 文档，包含所有可用方法和参数
- Resources/EXAMPLE.md ：代码示例，展示如何使用该技能
- Resources/FORMS.md ：输入输出格式说明
- Scripts/ ：脚本目录，存放技能相关的脚本文件
- .temp/ ：临时文件目录，存放技能运行过程中生成的临时文件
- config.json ：JSON 配置文件，定义技能的执行步骤和逻辑 
## 游戏模板 Skills (L3)
- SKILL.md ：技能的基本信息、功能描述、使用方法
- Resources/API.md ：API 文档，包含所有可用方法和参数
- Resources/EXAMPLE.md ：代码示例，展示如何使用该技能
- Resources/FORMS.md ：输入输出格式说明
- Scripts/ ：脚本目录，存放技能相关的脚本文件
- .temp/ ：临时文件目录，存放技能运行过程中生成的临时文件
- config.json ：JSON 配置文件，定义游戏模板的生成逻辑

# 关于 FORMS.md 文件
FORMS.md 文件的作用是定义技能的 输入输出格式 ，具体包括：

- 输入参数的类型、名称、描述和必填项
- 输出结果的结构和字段说明
- 错误码和错误信息定义
- 示例输入输出
这样 AI 就能准确理解如何调用这个技能，以及如何处理技能的返回结果。

# 总结
1. 对于基础 Skills ：如果引擎 API 已经足够清晰，AI 可以直接调用，不需要额外的 implementation
2. 对于组合 Skills ：主要通过 JSON 配置文件定义执行逻辑，不需要 C++ 实现
3. 对于游戏模板 Skills ：同样通过 JSON 配置文件定义生成逻辑