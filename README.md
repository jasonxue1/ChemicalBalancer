# ChemicalBalancer 
**ChemicalBalancer**  是一个用C++编写的命令行应用程序，用于自动配平化学和离子方程式。该项目不依赖任何第三方库，适合快速构建和使用。
## 功能 
 
- **自动配平** ：快速配平化学和离子方程式。
 
- **内置测试** ：包含测试用例，确保配平功能的正确性。

## 前提条件 
 
- **C++ 编译器** ：支持C++11标准（如 `g++`）。
 
- **CMake** ：用于构建项目。

## 安装 

### 1. 克隆仓库 


```bash
git clone https://github.com/jasonxue1/ChemicalBalancer.git
cd ChemicalBalancer
```

### 2. 构建项目 


```bash
mkdir build
cd build
cmake ..
make
```
构建完成后，生成的可执行文件 `ChemicalBalancer` 位于 `build` 目录下。
## 使用方法 

### 运行应用程序 


```bash
./ChemicalBalancer
```

### 输入方程式 
根据提示输入化学或离子方程式，使用 `+` 分隔反应物和生成物，使用 `->` 表示反应方向。**示例** ：

```css
H2 + O2 -> H2O
```

### 查看配平结果 

程序将输出配平后的方程式。
**输出** ：

```rust
配平后的方程式：
2H2 + O2 -> 2H2O
```

## 运行测试 
在 `build` 目录下运行测试用例：

```bash
./ChemicalBalancer --test
```
**示例输出** ：

```rust
运行测试...
测试通过： H2 + O2 -> H2O -> 2H2 + O2 -> 2H2O
测试通过： CH4 + O2 -> CO2 + H2O -> CH4 + 2O2 -> CO2 + 2H2O
测试通过： Fe + O2 -> Fe2O3 -> 4Fe + 3O2 -> 2Fe2O3
测试通过： HgO -> Hg + O2 -> 2HgO -> 2Hg + O2
测试通过（无法配平）： H2 + O2 -> H2O2
所有测试完成。
```

## 故障排除 
 
- **编译错误** ：
  - 确保使用支持C++11的编译器。

  - 验证是否正确安装并配置了CMake。
 
- **运行时错误** ： 
  - 检查输入格式是否正确，例如是否使用 `+` 和 `->` 分隔反应物和生成物。

  - 确保所有元素符号正确拼写并以大写字母开头。
 
- **配平问题** ：
  - 某些复杂的方程式可能无法配平，确保方程式在化学上是可行的。

## 许可证 
本项目采用MIT许可证。详见 [LICENSE](LICENSE)  文件。

---


## 快速参考 

### 构建项目 


```bash
git clone https://github.com/jasonxue1/ChemicalBalancer.git
cd ChemicalBalancer
mkdir build
cd build
cmake ..
make
```

### 运行程序 


```bash
./ChemicalBalancer
```

### 运行测试 


```bash
./ChemicalBalancer --test
```


---

如果你有任何问题或建议，欢迎在 [GitHub Issues](https://github.com/jasonxue1/ChemicalBalancer/issues)  中提出！