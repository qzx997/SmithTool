# SmithTool - 交互式史密斯圆图工具

## 1. 项目概述

SmithTool 是一个独立的交互式史密斯圆图（Smith Chart）设计工具，用于射频（RF）阻抗匹配和电路元件值计算。该工具将作为独立模块开发，完成后可集成到 DesignCircuitAgent 或其他 EDA 软件中。

### 1.1 核心功能

- **交互式Smith圆图显示**：支持阻抗(Z)圆图和导纳(Y)圆图
- **鼠标点击计算**：点击圆图任意位置，自动计算对应的 R、L、C 元件值
- **阻抗匹配设计**：支持 L-section、π-network、T-network 等匹配拓扑
- **轨迹绘制**：显示添加串/并联元件后阻抗变化轨迹
- **S参数导入**：支持 Touchstone (.s1p, .s2p) 文件导入和显示

### 1.2 目标用户

- RF 电路设计工程师
- 学习射频工程的学生
- 需要进行阻抗匹配设计的开发者

---

## 2. 技术架构

### 2.1 技术栈

| 组件 | 技术选择 | 说明 |
|------|----------|------|
| GUI 框架 | Qt 6 (Widgets) | 跨平台，与现有项目一致 |
| 编程语言 | C++ 17 | 高性能，与现有项目一致 |
| 构建系统 | CMake | 跨平台构建 |
| 数学库 | C++ STL <complex> | 复数运算支持 |

### 2.2 模块划分

```
SmithTool/
├── src/
│   ├── core/                    # 核心计算模块
│   │   ├── smithmath.h/cpp      # Smith圆图数学计算
│   │   ├── impedance.h/cpp      # 阻抗/导纳转换
│   │   ├── component.h/cpp      # 元件值计算 (R/L/C)
│   │   └── matching.h/cpp       # 匹配网络设计算法
│   │
│   ├── data/                    # 数据处理模块
│   │   ├── sparamdata.h/cpp     # S参数数据结构
│   │   ├── touchstone.h/cpp     # Touchstone文件解析
│   │   └── project.h/cpp        # 项目文件管理
│   │
│   ├── ui/                      # 用户界面模块
│   │   ├── smithchartwidget.h/cpp    # Smith圆图绑件
│   │   ├── componentpanel.h/cpp      # 元件值显示面板
│   │   ├── matchingwizard.h/cpp      # 匹配设计向导
│   │   ├── settingsdialog.h/cpp      # 设置对话框
│   │   └── mainwindow.h/cpp          # 主窗口
│   │
│   └── main.cpp                 # 程序入口
│
├── resources/                   # 资源文件
│   ├── icons/                   # 图标
│   ├── styles/                  # 样式表
│   └── resources.qrc            # Qt资源文件
│
├── tests/                       # 单元测试
│   ├── test_smithmath.cpp
│   ├── test_component.cpp
│   └── test_matching.cpp
│
├── examples/                    # 示例文件
│   ├── sample.s1p
│   └── sample.s2p
│
├── CMakeLists.txt              # 构建配置
└── PLAN.md                     # 本规划文档
```

---

## 3. 核心算法

### 3.1 坐标变换

#### 反射系数 ↔ 阻抗

```
Γ = (Z - Z₀) / (Z + Z₀)
Z = Z₀ × (1 + Γ) / (1 - Γ)
```

#### 反射系数 ↔ 导纳

```
Γ = (Y₀ - Y) / (Y₀ + Y)
Y = Y₀ × (1 - Γ) / (1 + Γ)
```

#### 屏幕坐标 ↔ Γ 平面

```
screen_x = center_x + Γ_real × radius
screen_y = center_y - Γ_imag × radius  // Y轴翻转
```

### 3.2 元件值计算

给定阻抗 Z = R + jX 和工作频率 f：

| 元件类型 | 条件 | 计算公式 |
|----------|------|----------|
| 电阻 R | 实部 | R = Re(Z) |
| 电感 L | X > 0 (感性) | L = X / (2πf) |
| 电容 C | X < 0 (容性) | C = -1 / (2πfX) |

### 3.3 等阻抗圆/等电抗弧

**等电阻圆 (r = const)**：
- 圆心：(r/(r+1), 0)
- 半径：1/(r+1)

**等电抗弧 (x = const)**：
- 圆心：(1, 1/x)
- 半径：1/|x|

### 3.4 匹配网络算法

#### L-Section 匹配

情况1：Z₀ > R_L（源阻抗大于负载电阻）
```
X = ±√(R_L(Z₀ - R_L)) - X_L
B = ±√((Z₀ - R_L)/R_L) / Z₀
```

情况2：Z₀ < R_L（源阻抗小于负载电阻）
```
B = (X_L ± √(R_L/Z₀)×√(R_L² + X_L² - Z₀×R_L)) / (R_L² + X_L²)
X = 1/B + X_L×Z₀/R_L - Z₀/(B×R_L)
```

元件值：
- 若 X > 0：串联电感 L = X/(2πf)
- 若 X < 0：串联电容 C = -1/(2πfX)
- 若 B > 0：并联电容 C = B/(2πf)
- 若 B < 0：并联电感 L = -1/(2πfB)

---

## 4. 功能详细设计

### 4.1 Smith圆图显示 (Phase 1)

**功能需求**：
- [x] 绘制单位圆（|Γ| = 1）
- [x] 绘制等电阻圆（r = 0, 0.2, 0.5, 1, 2, 5）
- [x] 绘制等电抗弧（x = ±0.2, ±0.5, ±1, ±2, ±5）
- [x] 支持导纳圆图叠加显示
- [x] VSWR 圆显示
- [x] 网格标签显示
- [x] 缩放和平移功能

**交互需求**：
- [x] 鼠标悬停显示阻抗/导纳值
- [x] 鼠标点击选择位置
- [x] 右键菜单操作

### 4.2 元件值计算 (Phase 2)

**功能需求**：
- [x] 点击圆图位置，计算等效 R/L/C 值
- [x] 支持串联/并联元件模式切换
- [x] 显示归一化和非归一化阻抗
- [ ] 支持多频率点计算

**显示面板**：
```
┌─────────────────────────────────┐
│ 频率: 1.000 GHz                 │
├─────────────────────────────────┤
│ 反射系数 Γ:                     │
│   |Γ| = 0.333  ∠45.0°          │
│   Γ = 0.236 + j0.236           │
├─────────────────────────────────┤
│ 阻抗 Z (归一化):                │
│   z = 1.5 + j1.0               │
│ 阻抗 Z (实际):                  │
│   Z = 75.0 + j50.0 Ω           │
├─────────────────────────────────┤
│ 等效元件值:                     │
│   R = 75.0 Ω                   │
│   L = 7.96 nH (串联)           │
│ 或 C = 3.18 pF (并联)          │
├─────────────────────────────────┤
│ VSWR = 2.0                     │
│ Return Loss = -9.54 dB         │
└─────────────────────────────────┘
```

### 4.3 轨迹绘制 (Phase 3)

**功能需求**：
- [x] 添加串联电阻：沿等电抗弧移动
- [x] 添加串联电抗：沿等电阻圆移动
- [x] 添加并联电导：沿等电纳弧移动（导纳圆图）
- [x] 添加并联电纳：沿等电导圆移动（导纳圆图）
- [x] 显示元件添加后的阻抗变化轨迹
- [x] **每级电路轨迹显示**：显示每个元件添加后的阻抗轨迹曲线（如沿等电阻圆移动的弧线）

**轨迹类型**：

| 元件 | 连接方式 | 轨迹说明 |
|------|----------|----------|
| R | 串联 | 沿等电抗弧向右移动 |
| L | 串联 | 沿等电阻圆顺时针移动（上半圆） |
| C | 串联 | 沿等电阻圆逆时针移动（下半圆） |
| R | 并联 | 沿等电纳弧移动（Y圆图） |
| L | 并联 | 沿等电导圆逆时针移动（Y圆图下半） |
| C | 并联 | 沿等电导圆顺时针移动（Y圆图上半） |

### 4.3.1 电路图绘制功能 (Phase 3 新增)

**功能需求**：
- [x] **元件添加按钮**：提供添加串联/并联 R、L、C 元件的按钮
  - 串联电阻、串联电感、串联电容
  - 并联电阻、并联电感、并联电容
- [x] **实时电路图显示**：添加元件后实时显示对应的电路原理图
- [x] **级联电路视图**：显示从源到负载的完整匹配网络拓扑
- [x] **元件参数编辑**：双击电路图中的元件可编辑其值
- [x] **轨迹颜色区分**：每级电路使用不同颜色显示其阻抗轨迹

### 4.3.4 交互式元件编辑 (Phase 7 新增)

**功能需求**：
- [x] **元件值修改**：支持修改每个已添加元件的值
  - 双击元件打开编辑对话框
  - 支持直接输入数值
  - 实时预览修改后的轨迹变化
- [x] **圆图选点添加**：添加元件时可在史密斯圆图上点击选择目标点
  - 点击工具栏元件按钮后进入选点模式
  - 在圆图上点击目标位置，自动计算所需元件值
  - 显示实时轨迹预览
- [x] **轨迹拖动修改**：通过拖动轨迹端点修改元件值
  - 鼠标悬停在轨迹端点时显示拖拽手柄
  - 拖动时实时更新元件值和轨迹
  - 约束轨迹移动在有效范围内（沿等电阻圆/等电抗弧）

**交互模式**：
```
模式1: 默认模式
  - 点击圆图显示阻抗信息
  - 双击元件编辑值

模式2: 选点添加模式
  - 点击元件按钮后激活
  - 圆图上点击选择目标点
  - 自动计算并添加元件

模式3: 拖动编辑模式
  - 选中轨迹端点后激活
  - 拖动修改元件值
  - 轨迹实时更新
```

**电路图显示面板**：
```
┌────────────────────────────────────────┐
│  源端        匹配网络           负载端  │
│  Z_S ─┬─[L1]─┬─[C2]─┬─ Z_L             │
│       │      │      │                  │
│      [C1]   [L2]   [R1]                │
│       │      │      │                  │
│      GND    GND    GND                 │
└────────────────────────────────────────┘
```

### 4.3.2 等Q值圆显示 (Phase 3 新增)

**功能需求**：
- [x] **等Q值圆绘制**：在Smith圆图上绘制等品质因数(Q)圆
- [x] **Q值选择**：支持自定义Q值（如 Q = 0.5, 1, 2, 5）
- [x] **Q值标签**：在圆上标注对应的Q值
- [x] **显示开关**：可选择是否显示等Q值圆

**等Q值圆算法**：
- Q = |X|/R = |Im(Z)|/Re(Z)
- 等Q圆圆心：(1/(1+Q²), 0)
- 等Q圆半径：Q/(1+Q²)

### 4.3.3 源端/负载阻抗设置 (Phase 3 新增)

**功能需求**：
- [x] **源端阻抗输入**：支持复数形式输入 Z_S = R_S + jX_S
- [x] **负载阻抗输入**：支持复数形式输入 Z_L = R_L + jX_L
- [x] **阻抗格式**：支持直角坐标和极坐标两种输入格式
  - 直角坐标：50 + j25 Ω
  - 极坐标：55.9 ∠26.6° Ω
- [x] **归一化参考**：源端阻抗实部可作为归一化参考 Z₀
- [x] **标记显示**：在圆图上用特殊标记显示源端和负载阻抗位置

**阻抗输入面板**：
```
┌─────────────────────────────────────┐
│ 源端阻抗 Z_S:                        │
│   [50.0] + j[0.0] Ω    ○直角 ●极坐标│
├─────────────────────────────────────┤
│ 负载阻抗 Z_L:                        │
│   [75.0] + j[-25.0] Ω  ●直角 ○极坐标│
├─────────────────────────────────────┤
│ 归一化阻抗 Z₀: [50.0] Ω             │
└─────────────────────────────────────┘
```

### 4.4 匹配网络设计 (Phase 4)

**支持的匹配拓扑**：
- [x] L-Section（两种解）
- [x] Pi-Network
- [x] T-Network
- [x] Single Stub（开路/短路）
- [ ] Double Stub
- [x] Quarter-Wave Transformer

**设计向导流程**：
1. 输入源阻抗 Z_S（默认 50Ω）
2. 输入/选择负载阻抗 Z_L
3. 输入工作频率
4. 选择匹配拓扑
5. 计算并显示匹配网络
6. 在圆图上显示匹配轨迹
7. 导出元件值/网表
8. **导出SPICE网表**

### 4.5 S参数导入 (Phase 5)

**支持格式**：
- [x] Touchstone .s1p（单端口）
- [x] Touchstone .s2p（双端口）
- [ ] CSV 格式

**显示功能**：
- [x] 在圆图上绘制 S11/S22 轨迹
- [x] 频率标记点
- [ ] 频率范围选择
- [ ] 轨迹颜色/样式设置

---

## 5. 开发计划

### Phase 1: 基础框架 (Week 1-2)

| 任务 | 预计时间 | 优先级 |
|------|----------|--------|
| 项目结构搭建 | 2h | P0 |
| CMake 构建配置 | 2h | P0 |
| 基础 SmithChartWidget | 8h | P0 |
| 等电阻圆/等电抗弧绘制 | 4h | P0 |
| 坐标变换函数 | 4h | P0 |
| 基础鼠标交互 | 4h | P1 |

### Phase 2: 元件计算 (Week 3)

| 任务 | 预计时间 | 优先级 |
|------|----------|--------|
| 阻抗计算模块 | 4h | P0 |
| R/L/C 值计算 | 4h | P0 |
| 元件显示面板 UI | 6h | P0 |
| 频率输入控件 | 2h | P1 |

### Phase 3: 轨迹功能 (Week 4)

| 任务 | 预计时间 | 优先级 |
|------|----------|--------|
| 轨迹数据结构 | 2h | P0 |
| 串联元件轨迹算法 | 4h | P0 |
| 并联元件轨迹算法 | 4h | P0 |
| 轨迹绘制渲染 | 4h | P0 |
| 轨迹动画效果 | 4h | P2 |

### Phase 4: 匹配设计 (Week 5-6)

| 任务 | 预计时间 | 优先级 |
|------|----------|--------|
| L-Section 算法 | 4h | P0 |
| 匹配向导 UI | 8h | P0 |
| Pi/T-Network 算法 | 4h | P1 |
| Stub 匹配算法 | 6h | P1 |
| 网表导出功能 | 4h | P1 |

### Phase 5: 数据导入 (Week 7)

| 任务 | 预计时间 | 优先级 |
|------|----------|--------|
| Touchstone 解析器 | 4h | P0 |
| S参数轨迹显示 | 4h | P0 |
| 频率标记功能 | 4h | P1 |
| 文件拖放支持 | 2h | P2 |

### Phase 6: 集成准备 (Week 8)

| 任务 | 预计时间 | 优先级 |
|------|----------|--------|
| API 接口设计 | 4h | P0 |
| 单元测试完善 | 8h | P0 |
| 文档编写 | 4h | P1 |
| 集成测试 | 4h | P0 |

---

## 6. 接口设计

### 6.1 对外接口（用于集成）

```cpp
namespace SmithTool {

// 主要组件类
class SmithChartWidget : public QWidget {
public:
    // 数据输入
    void setSParamData(const SParamData& data);
    void setFrequency(double freq_hz);
    void setZ0(double z0);
    
    // 匹配设计
    void setSourceImpedance(std::complex<double> zs);
    void setLoadImpedance(std::complex<double> zl);
    MatchingResult calculateMatching(MatchingTopology topology);
    
    // 轨迹操作
    void addSeriesElement(ComponentType type, double value);
    void addShuntElement(ComponentType type, double value);
    void clearTraces();
    
    // 显示控制
    void setShowAdmittance(bool show);
    void setShowVSWRCircles(bool show);
    
signals:
    void pointClicked(std::complex<double> gamma, std::complex<double> z);
    void componentValuesChanged(double r, double l, double c);
    void matchingCompleted(const MatchingResult& result);
};

// 匹配结果结构
struct MatchingResult {
    bool success;
    MatchingTopology topology;
    std::vector<Component> components;
    QString netlist;
};

// 元件类型
enum class ComponentType { Resistor, Inductor, Capacitor };

// 匹配拓扑
enum class MatchingTopology { 
    LSection, PiNetwork, TNetwork, 
    SingleStub, DoubleStub, QuarterWave 
};

} // namespace SmithTool
```

### 6.2 信号槽接口

```cpp
// 可连接的信号
signals:
    void pointHovered(double freq, std::complex<double> gamma, std::complex<double> z);
    void pointClicked(std::complex<double> gamma, std::complex<double> z);
    void componentValuesCalculated(double freq, double r, double l, double c);
    void matchingCompleted(const MatchingResult& result);
    void traceAdded(int traceId);
```

---

## 7. 测试计划

### 7.1 单元测试

| 测试模块 | 测试内容 |
|----------|----------|
| smithmath | Γ↔Z 转换精度、等圆计算 |
| component | R/L/C 值计算正确性 |
| matching | L-section 两种解验证 |
| touchstone | 文件解析、格式兼容性 |

### 7.2 集成测试

- [ ] 完整匹配流程测试
- [ ] S参数文件导入→显示→标记
- [ ] 多频率点计算一致性
- [ ] UI 响应性能测试

### 7.3 验收标准

- 阻抗计算误差 < 0.1%
- 元件值计算与 ADS/AWR 结果对比误差 < 1%
- 60fps 流畅绑件刷新
- 内存占用 < 100MB

---

## 8. 风险与对策

| 风险 | 可能性 | 影响 | 对策 |
|------|--------|------|------|
| Qt版本兼容问题 | 中 | 中 | 使用 Qt 6.2+ LTS 版本 |
| 复数运算精度 | 低 | 高 | 使用 double 精度，边界检查 |
| 大数据量性能 | 中 | 中 | 数据点采样，异步加载 |
| 匹配算法边界情况 | 中 | 中 | 完善边界条件检查和错误提示 |

---

## 9. 参考资料

1. Philip H. Smith, "Electronic Applications of the Smith Chart", 2nd Edition
2. David M. Pozar, "Microwave Engineering", 4th Edition
3. Peter Vizmuller, "RF Design Guide: Systems, Circuits, and Equations"
4. Qucs-S 项目源码: https://github.com/ra3xdh/qucs_s
5. Qt 6 文档: https://doc.qt.io/qt-6/

---

## 10. 版本历史

| 版本 | 日期 | 说明 |
|------|------|------|
| 0.1 | 2024-12-21 | 初始规划文档 |
| 0.5 | 2024-12-21 | 完成 Phase 1-3: 基础框架、元件计算、轨迹绘制 |
| 0.8 | 2024-12-21 | 完成 Phase 4-5: 匹配网络设计、S参数导入 |
| 0.9 | 2024-12-21 | 完成 Single Stub/Quarter-Wave匹配、源/负载标记显示 |
| 1.0 | 2024-12-21 | 完成 Phase 7: 交互式元件编辑、圆图选点添加功能 |
| 1.1 | 2024-12-21 | 完成轨迹拖动修改功能，实现双向交互编辑 |
| 1.2 | 2024-12-21 | 修复电阻轨迹算法，添加双击电路图元件编辑功能 |
| 1.3 | 2024-12-21 | 添加缩放平移、右键菜单、元件删除功能 |
| 1.4 | 2024-12-21 | 添加SPICE网表导出功能，支持LTspice/Ngspice格式 |
| 1.5 | 2024-12-21 | 添加现代化深色/浅色主题，创建Windows打包配置和安装程序 |

