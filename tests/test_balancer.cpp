// test_balancer.cpp

#include "balancer.h"
#include <iostream>
#include <cassert>

using namespace std;

void runTest(const string& input, const string& expected) {
    Balancer balancer;
    string output;
    bool success = balancer.balanceEquation(input, output);
    if (success) {
        if (output == expected) {
            cout << "测试通过： " << input << " -> " << output << endl;
        } else {
            cout << "测试失败： " << input << endl;
            cout << "预期： " << expected << endl;
            cout << "实际： " << output << endl;
        }
    } else {
        cout << "测试失败（无法配平）： " << input << endl;
    }
}

int main() {
    cout << "运行测试...\n";

    // 测试1
    runTest("H2 + O2 -> H2O", "2H2 + O2 -> 2H2O");

    // 测试2
    runTest("CH4 + O2 -> CO2 + H2O", "CH4 + 2O2 -> CO2 + 2H2O");

    // 测试3
    runTest("Fe + O2 -> Fe2O3", "4Fe + 3O2 -> 2Fe2O3");

    // 测试4
    runTest("HgO -> Hg + O2", "2HgO -> 2Hg + O2");

    // 额外测试：无解
    Balancer balancer;
    string balanced;
    bool success = balancer.balanceEquation("H2 + O2 -> H2O2", balanced);
    if (!success) {
        cout << "测试通过（无法配平）： H2 + O2 -> H2O2" << endl;
    } else {
        cout << "测试失败（应无法配平）： H2 + O2 -> H2O2, 得到：" << balanced << endl;
    }

    cout << "所有测试完成。\n";
    return 0;
}