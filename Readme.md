# SmithTool - 史密斯圆图辅助工具

![License](https://img.shields.io/github/license/qzx997/SmithTool)
![Version](https://img.shields.io/badge/version-1.0.0-blue)
![Status](https://img.shields.io/badge/status-active-success)

**SmithTool** 是一个轻量级、易用的史密斯圆图（Smith Chart）可视化与阻抗匹配计算工具。旨在帮助射频（RF）工程师、学生和电子爱好者快速进行阻抗分析和匹配网络设计。

[English README](./README_EN.md) ---

## 📸 截图预览

![软件截图](docs/screenshot_main.png)
*(请将截图文件放入 docs 文件夹或直接替换此链接)*

## ✨ 主要功能

* **可视化绘图**：支持在史密斯圆图上直观地绘制阻抗点（Impedance）和导纳点（Admittance）。
* **多参数显示**：实时计算并显示反射系数（$\Gamma$）、电压驻波比（VSWR）、回波损耗（Return Loss）等关键参数。
* **阻抗匹配设计**：
    * 支持串联/并联电感、电容的动态添加。
    * 实时显示元件值变化对匹配路径的影响。
* **圆图辅助线**：支持显示等电阻圆、等电抗圆、等 Q 值曲线及 VSWR 圆。
* **[可选功能] 数据导入导出**：支持 Touchstone (.s1p, .s2p) 文件导入。

## 🛠️ 安装与运行

### 环境要求

* 操作系统：Windows / Linux / macOS
* 编程语言：**[例如：Python 3.8+]**
* 依赖库：见 `requirements.txt`

### 快速开始

1.  **克隆仓库**
    ```bash
    git clone [https://github.com/qzx997/SmithTool.git](https://github.com/qzx997/SmithTool.git)
    cd SmithTool
    ```

2.  **安装依赖**
    ```bash
    pip install -r requirements.txt
    ```
    3.  **运行工具**
    ```bash
    python main.py
    ```

## 📖 使用说明

1.  **设置系统阻抗**：默认为 $50\Omega$，可在设置中修改。
2.  **输入起始点**：在左侧面板输入负载阻抗（$Z_L$）的实部和虚部。
3.  **添加匹配元件**：点击工具栏的 L/C 图标添加串联或并联元件，拖动滑块调整数值，观察轨迹变化走向圆心。
4.  **导出结果**：计算完成后，可导出匹配网络的电路拓扑图或元件清单。

## 🤝 贡献指南 (Contributing)

欢迎提交 Issue 反馈 Bug 或提交 Pull Request 改进代码！

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request

## 📄 开源协议

本项目采用 **MIT 协议** 开源，详情请参阅 [LICENSE](LICENSE) 文件。

---

**Author:** [qzx997](https://github.com/qzx997)