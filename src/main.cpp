// balancer.cpp

#include "balancer.h"
#include <iostream>
#include <vector>
#include <map>
#include <regex>
#include <sstream>
#include <algorithm>
#include <cmath>

using namespace std;

// 结构体用于保存化合物的元素及其数量
struct Compound {
    map<string, int> elements;
};

// 简单的高斯消元法求解线性方程组 Ax = 0
// 返回解向量，如果有非零解则返回true，否则返回false
bool gaussianElimination(vector<vector<double>>& A, vector<double>& solution) {
    int n = A.size();
    if (n == 0) return false;
    int m = A[0].size();

    // 构建增广矩阵
    for (int i = 0; i < n; ++i) {
        A[i].push_back(0.0); // Ax = 0 的增广部分
    }

    // 高斯消元
    int rank = 0;
    for (int col = 0; col < m; ++col) {
        // 寻找主元
        int sel = -1;
        for (int row = rank; row < n; ++row) {
            if (abs(A[row][col]) > 1e-8) {
                sel = row;
                break;
            }
        }
        if (sel == -1) continue;

        // 交换行
        swap(A[rank], A[sel]);

        // 归一化
        double factor = A[rank][col];
        for (int j = 0; j <= m; ++j) {
            A[rank][j] /= factor;
        }

        // 消元
        for (int row = 0; row < n; ++row) {
            if (row != rank && abs(A[row][col]) > 1e-8) {
                double factor = A[row][col];
                for (int j = 0; j <= m; ++j) {
                    A[row][j] -= factor * A[rank][j];
                }
            }
        }

        rank++;
    }

    // 检查是否存在非零解
    if (rank == m) {
        // 只有零解
        return false;
    }

    // 自由变量取1，求解其他变量
    solution.assign(m, 0.0);
    for (int i = 0; i < rank; ++i) {
        int leading = -1;
        for (int j = 0; j < m; ++j) {
            if (abs(A[i][j]) > 1e-8) {
                leading = j;
                break;
            }
        }
        if (leading == -1) continue;
        solution[leading] = 1.0;
        for (int j = leading + 1; j < m; ++j) {
            solution[leading] -= A[i][j] * solution[j];
        }
    }

    return true;
}

// 解析化学式，将其拆分为元素及其数量
bool parseFormula(const string& formula, Compound& compound) {
    regex elementRegex("([A-Z][a-z]?)(\\d*)");
    auto elementsBegin = sregex_iterator(formula.begin(), formula.end(), elementRegex);
    auto elementsEnd = sregex_iterator();

    for (auto it = elementsBegin; it != elementsEnd; ++it) {
        smatch match = *it;
        string element = match[1];
        string countStr = match[2];
        int count = countStr.empty() ? 1 : stoi(countStr);
        compound.elements[element] += count;
    }

    // 检查是否整个化学式都被解析
    string parsed;
    for (auto it = elementsBegin; it != elementsEnd; ++it) {
        parsed += it->str();
    }
    return parsed.length() == formula.length();
}

// 将方程式分割为反应物和生成物
bool splitEquation(const string& equation, vector<string>& reactants, vector<string>& products) {
    size_t arrowPos = equation.find("->");
    if (arrowPos == string::npos) return false;

    string react = equation.substr(0, arrowPos);
    string prod = equation.substr(arrowPos + 2);

    // 通过 '+' 分割反应物和生成物
    auto splitByPlus = [](const string& side, vector<string>& compounds) {
        stringstream ss(side);
        string compound;
        while (getline(ss, compound, '+')) {
            // 去除任何空白字符
            compound.erase(remove_if(compound.begin(), compound.end(), ::isspace), compound.end());
            compounds.push_back(compound);
        }
    };

    splitByPlus(react, reactants);
    splitByPlus(prod, products);
    return true;
}

// 获取方程式中所有独特的元素
void getAllElements(const vector<Compound>& compounds, vector<string>& elements) {
    map<string, bool> elementMap;
    for (const auto& comp : compounds) {
        for (const auto& elem : comp.elements) {
            elementMap[elem.first] = true;
        }
    }
    for (const auto& elem : elementMap) {
        elements.push_back(elem.first);
    }
}

// 计算两个整数的最大公约数
int gcd_int(int a, int b) {
    if (b == 0) return a;
    return gcd_int(b, a % b);
}

// 配平方程式
bool Balancer::balanceEquation(const string& equation, string& balancedEquation) {
    vector<string> reactantStrs, productStrs;
    if (!splitEquation(equation, reactantStrs, productStrs)) {
        return false;
    }

    // 解析所有化合物
    vector<Compound> reactants, products;
    for (const auto& r : reactantStrs) {
        Compound comp;
        if (!parseFormula(r, comp)) return false;
        reactants.push_back(comp);
    }
    for (const auto& p : productStrs) {
        Compound comp;
        if (!parseFormula(p, comp)) return false;
        products.push_back(comp);
    }

    // 获取所有独特的元素
    vector<Compound> allCompounds = reactants;
    allCompounds.insert(allCompounds.end(), products.begin(), products.end());
    vector<string> elements;
    getAllElements(allCompounds, elements);
    int numElements = elements.size();
    int numCompounds = reactants.size() + products.size();

    // 设置矩阵
    vector<vector<double>> A(numElements, vector<double>(numCompounds, 0.0));

    // 填充矩阵
    for (int i = 0; i < numElements; ++i) {
        string elem = elements[i];
        // 反应物
        for (int j = 0; j < reactants.size(); ++j) {
            if (reactants[j].elements.find(elem) != reactants[j].elements.end()) {
                A[i][j] = reactants[j].elements.at(elem);
            }
        }
        // 生成物
        for (int j = 0; j < products.size(); ++j) {
            if (products[j].elements.find(elem) != products[j].elements.end()) {
                A[i][reactants.size() + j] = -products[j].elements.at(elem);
            }
        }
    }

    // 求解 Ax = 0
    vector<double> solution;
    bool hasSolution = gaussianElimination(A, solution);
    if (!hasSolution) {
        return false;
    }

    // 寻找最小正整数比例
    // 将解中的所有系数除以最小的非零系数，并取最接近的整数
    double min_ratio = 1e20;
    for (double val : solution) {
        if (abs(val) > 1e-6) {
            min_ratio = min(min_ratio, abs(val));
        }
    }
    if (min_ratio == 1e20) return false; // 无非零解

    vector<int> coeffs(numCompounds, 0);
    for (int i = 0; i < numCompounds; ++i) {
        coeffs[i] = round(solution[i] / min_ratio);
    }

    // 检查是否有系数为0
    for (const auto& c : coeffs) {
        if (c == 0) return false;
    }

    // 通过最大公约数简化系数
    int gcd_val = abs(coeffs[0]);
    for (int i = 1; i < coeffs.size(); ++i) {
        gcd_val = gcd(gcd_val, abs(coeffs[i]));
    }
    if (gcd_val > 1) {
        for (int i = 0; i < coeffs.size(); ++i) {
            coeffs[i] /= gcd_val;
        }
    }

    // 构建配平后的方程式
    stringstream ss;
    for (int i = 0; i < reactants.size(); ++i) {
        if (coeffs[i] != 1) ss << coeffs[i];
        ss << reactantStrs[i];
        if (i != reactants.size() - 1) ss << " + ";
    }
    ss << " -> ";
    for (int i = 0; i < products.size(); ++i) {
        if (coeffs[reactants.size() + i] != 1) ss << coeffs[reactants.size() + i];
        ss << productStrs[i];
        if (i != products.size() - 1) ss << " + ";
    }

    balancedEquation = ss.str();
    return true;
}