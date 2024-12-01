# Example

## Based on

- VSCode + platformIO
- ESP32 [CP2102]
- 0.96' OLED I2C 128x64 with BLUE/YELLOW Double Color AT [TX2/RX2]
- Simplest button AT [GPIO 18/19/27]

## structure

在你提供的代码中，整体设计上主要涉及到几个方面的配合和结构安排，尤其是状态机的管理、UI更新和输入控制。以下是一些主要的设计配合：

### 1. **状态机管理（State Machine）**

   你的 `loop()` 函数基于当前状态 (`currentState`) 来决定接下来的行为。每个状态负责不同的UI更新和交互逻辑：

- `IDLE` 状态：等待用户的输入，根据不同的按键（如 `KEY_ENTER`, `KEY_CYCLE`, `KEY_BACK`）进入其他状态。
- `FORWARD` 状态：处理前进的动作，可以通过按键进行循环切换。
- `BACKWARD` 和 `BACKWARD_SELECTED` 状态：处理后退的动作，包含了界面元素的更新和条件判断。
- `MODULE` 状态：根据不同的条件，选择模块，并进行相关的UI渲染和切换。

### 2. **UI绘制与动态更新**

   你使用了 `display.clearDisplay()` 和 `display.display()` 来管理屏幕的刷新。具体的UI元素（例如模块、按钮、框架）是通过不同的 `draw` 函数来绘制的。每次状态变化时，`draw()` 函数会被调用来重绘UI界面。不同的模块（前进/后退模块）也有各自的绘制逻辑，如：

- `drawSeleModule()` 和 `drawUnseleModule()` 用于绘制已选择和未选择的模块。
- `drawForwardModules()` 和 `drawBackwardModules()` 分别用于绘制前进模块和后退模块。

### 3. **输入处理**

   按键的输入控制通过 `digitalRead()` 检测。你在 `loop()` 中针对每个按键状态进行判断，决定进入哪个状态，并相应地更新UI：

- 按下 `KEY_ENTER` 时，如果在 `IDLE` 状态，会根据 `forwardPointer` 的值进入不同的状态。
- 按下 `KEY_CYCLE` 会触发前进操作（`FORWARD` 状态）。
- 按下 `KEY_BACK` 会触发后退操作（`BACKWARD` 状态）。

### 4. **动画和UI过渡**

   你的代码中有处理模块之间动画过渡的逻辑，尤其是在前进和后退时。通过调整 `IconTrans.x` 和 `IconTrans.y` 来平移模块，创建平滑的视觉效果。同时通过计算 `curStep` 和 `totalStep` 来控制模块的动画进度。例如，`drawForwardModules()` 中通过 `curStep` 来逐步渲染模块。

### 5. **模块交互**

   你定义了多个模块，每个模块有不同的交互效果。当用户选择一个模块时，会调用 `modules[forwardPointer]()` 来执行模块相关的回调。这意味着每个模块的具体行为可以通过回调函数来定制，进而通过 `currentState` 控制它们的显示与交互。

### 6. **回调机制**

   在 `Menu::loop()` 中，当某些状态变更时（比如进入 `MODULE` 状态），会调用 `modules[forwardPointer]()`，这是一个回调机制，表示当前所选模块的行为。模块的定义与行为是分开的，模块的选择与展示通过UI管理，而模块本身的逻辑则通过回调来实现。这使得你的UI具备了灵活的扩展性，每个模块都可以独立定义其回调逻辑。

### 配合建议

- **事件和状态同步**：确保在 `loop()` 中对每个状态的处理逻辑足够简洁，并避免状态间的冲突。特别是对于按钮按下和UI更新，要合理安排延时和更新顺序，避免出现视觉上的跳跃或延迟。
- **模块抽象**：尽可能将UI元素和模块的逻辑分离，确保UI的展示和模块的行为能独立开发、独立修改。
- **动画管理**：对于动画的流畅性，考虑使用时间差（如 `millis()`）来控制动画的进度，而不仅仅依赖于 `delay()`，这样可以保证UI的流畅更新。

整体上，你的设计非常接近一个高效的嵌入式UI，状态机和回调函数的配合设计使得界面更新和交互行为得以清晰、灵活地管理。
