# HinarUI Refactor Plan

## 1 板级解耦（Boards & PAL）

* [x] 新建 `boards/` 目录
* [x] 将 `resource/asset.h` 中**硬件相关**项迁移到对应 `boards/*.h`（SDA/SCL、KEY_*、I²C 地址等）
* [x] 新建统一入口 `board_profile.h`，通过编译宏 `-D BOARD=ESP32_DEVKIT` 选择
* [ ] `resource/asset.h` 保留**UI 资源声明**与常量（不再混入 GPIO/I²C）
* [ ] 文档：README 增补“如何切换板型”的 3 步
* 验收：切换两种板文件编译通过，示例可运行（不改业务代码）。([GitHub][1])

## 2 UI 内核拆分（Core）

* [ ] 新建 `src/ui_core/renderer.{h,cpp}`（双缓冲、脏矩形、局部刷新）
* [ ] 新建 `src/ui_core/input.{h,cpp}`（按键扫描→事件，含防抖/长按/重复）
* [ ] 新建 `src/ui_core/router.{h,cpp}`（页面栈：`push/pop/replace`）
* [ ] 将 `UI.cpp` 中对应职责函数迁移，`UI.h` 仅暴露高层 API
* 验收：现有示例编译通过、帧率不劣化

## 3 统一事件与状态机

* [ ] 定义 `struct UiEvent { uint8_t type; uint16_t code; int16_t v; };`
* [ ] 事件源：按键、超时、传感器/业务更新 → 同一环形队列
* [ ] 页面生命周期钩子：`enter(ctx) / update(ctx,event) / exit(ctx)`
* [ ] 典型流：告警弹窗、长任务进度、确认对话框走状态机而不是控件内 if-else
* 验收：新增 `example/state_demo`，覆盖“弹窗→确认/取消→回退”

## 4 渲染与性能专项（OLED 128×64）

* [ ] 双缓冲（128×64 × 1bpp ≈ 1 KB/帧）与脏区裁剪
* [ ] 曲线/列表滚动：仅重绘变化行或曲线新像素
* [ ] 数字读数显示滤波：阈值 + 滞回，减少抖动（工业读数友好）
* 验收：提供 `example/perf_meter`，串口打印“重绘像素数/帧时间（ms）”

## 5 资源与主题（Design Tokens）

* [ ] 新建 `themes/tokens.h`：语义色（fg/bg/ok/warn）、间距、圆角、字号尺度
* [ ] `fonts/`：将常用字号切分为**字符子集**（数字/符号为常驻，其余按需）
* [ ] `tools/png2c.py`：把 1bpp PNG 自动打包成 `resource/icon.h`（生成日期+校验）
* 验收：切换 `tokens-compact.h` 与 `tokens-standard.h` 外观稳定

## 6 控件层规范

* [ ] 核心控件统一签名：`measure(layout_ctx) → layout(rect) → draw(renderer)`
* [ ] 完成最小集：`Label / Button / List / Dialog / Progress / Sparkline`
* [ ] 业务控件（如通道卡）仅依赖核心控件接口，不触碰硬件
* 验收：`example/widgets_gallery` 展示并可操作

## 7 输入与可访问性

* [ ] 焦点系统（方向键移动 + 长按/重复速率）
* [ ] 固定三键语义：确认/返回/更多（映射当前 KEY_*）
* [ ] 危险操作“两段式确认 + 超时自动回退”
* 验收：在 gallery 中验证焦点可视化与可回退路径

## 8 可测试性与可观测性

* [ ] `example/replay_demo`：从 `events.log` 回放，导出帧缓冲为 PBM（便于回归）
* [ ] `tools/size_report.py`：构建后输出 `size.json`（固件体积/常驻 RAM 估算）
* [ ] 简易指标：输入到响应延迟、重绘耗时（串口打印）
* 验收：PR CI 附带 size 变化与回放截图工件

## 9 打包与分发

* [ ] 调整为 PlatformIO library 规范：`library.json` + `src/` + `examples/`
* [ ] `examples/` 保留现有示例并新增上述 demo
* [ ] Release note：0.3.x 内部重构项与升级指引
* 验收：示例通过 PIO 一键运行

## 10 文档与示意图

* [ ] README 新增：板型切换、事件/状态机示意、tokens 说明
* [ ] 在 README “Usage” 中保留现有“改 `resource/asset.h`”提示，同时链接到板型方式（两种都可行，推荐新法）([GitHub][1])
* [ ] 增加一张“页面/事件/渲染”关系图（`pic/architecture.svg`）
