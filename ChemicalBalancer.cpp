// ChemicalBalancer.cpp

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <regex>
#include <sstream>
#include <algorithm>
#include <cmath>

using namespace std;

struct Compound {
    map<string, int> elements;
};

bool gaussianElimination(vector<vector<double>>& A, vector<double>& solution) {
    int n = A.size(), m = A[0].size();
    for(auto &row : A) row.push_back(0.0);
    int rank = 0;
    for(int col = 0; col < m; ++col){
        int sel = -1;
        for(int row = rank; row < n; ++row)
            if(abs(A[row][col]) > 1e-8){ sel = row; break;}
        if(sel == -1) continue;
        swap(A[rank], A[sel]);
        double factor = A[rank][col];
        for(auto &val : A[rank]) val /= factor;
        for(int row = 0; row < n; ++row){
            if(row != rank && abs(A[row][col]) > 1e-8){
                double f = A[row][col];
                for(int j = 0; j <= m; ++j) A[row][j] -= f * A[rank][j];
            }
        }
        rank++;
    }
    if(rank == m) return false;
    solution.assign(m, 0.0);
    for(int i = 0; i < rank; ++i){
        int lead = -1;
        for(int j = 0; j < m; ++j){
            if(abs(A[i][j]) > 1e-8){ lead = j; break;}
        }
        if(lead == -1) continue;
        solution[lead] = 1.0;
        for(int j = lead + 1; j < m; ++j)
            solution[lead] -= A[i][j] * solution[j];
    }
    return true;
}

bool parseFormula(const string& formula, Compound& compound){
    regex re("([A-Z][a-z]?)(\\d*)");
    auto it = sregex_iterator(formula.begin(), formula.end(), re);
    auto end = sregex_iterator();
    for(; it != end; ++it){
        smatch match = *it;
        string elem = match[1];
        int count = match[2].matched ? stoi(match[2]) : 1;
        compound.elements[elem] += count;
    }
    string parsed;
    it = sregex_iterator(formula.begin(), formula.end(), re);
    for(; it != end; ++it) parsed += it->str();
    return parsed.length() == formula.length();
}

bool splitEquation(const string& eq, vector<string>& reactants, vector<string>& products){
    size_t pos = eq.find('=');
    if(pos == string::npos) return false;
    auto split = [&](const string& side, vector<string>& compounds){
        stringstream ss(side);
        string comp;
        while(getline(ss, comp, '+')){
            comp.erase(remove_if(comp.begin(), comp.end(), ::isspace), comp.end());
            compounds.push_back(comp);
        }
    };
    split(eq.substr(0, pos), reactants);
    split(eq.substr(pos+1), products);
    return true;
}

bool balanceEquation(const string& eq, string& balanced){
    vector<string> react, prod;
    if(!splitEquation(eq, react, prod)) return false;
    vector<Compound> reactants, products;
    for(auto &r : react){
        Compound c;
        if(!parseFormula(r, c)) return false;
        reactants.push_back(c);
    }
    for(auto &p : prod){
        Compound c;
        if(!parseFormula(p, c)) return false;
        products.push_back(c);
    }
    vector<Compound> all = reactants;
    all.insert(all.end(), products.begin(), products.end());
    vector<string> elements;
    map<string, bool> elemMap;
    for(auto &c : all) for(auto &[e, _] : c.elements) elemMap[e] = true;
    for(auto &[e, _] : elemMap) elements.push_back(e);
    int rows = elements.size(), cols = reactants.size() + products.size();
    vector<vector<double>> A(rows, vector<double>(cols, 0.0));
    for(int i = 0; i < rows; ++i){
        string elem = elements[i];
        for(int j = 0; j < reactants.size(); ++j)
            if(reactants[j].elements.count(elem)) A[i][j] = reactants[j].elements[elem];
        for(int j = 0; j < products.size(); ++j)
            if(products[j].elements.count(elem)) A[i][reactants.size()+j] = -products[j].elements[elem];
    }
    vector<double> sol;
    if(!gaussianElimination(A, sol)) return false;
    double min_ratio = 1e20;
    for(auto val : sol) if(abs(val) > 1e-6) min_ratio = min(min_ratio, abs(val));
    if(min_ratio == 1e20) return false;
    vector<int> coeff(cols, 0);
    for(int i = 0; i < cols; ++i) coeff[i] = round(sol[i]/min_ratio);
    for(auto c : coeff) if(c == 0) return false;
    int gcd_val = abs(coeff[0]);
    for(auto c : coeff) gcd_val = gcd(gcd_val, abs(c));
    if(gcd_val > 1) for(auto &c : coeff) c /= gcd_val;
    stringstream ss;
    for(int i = 0; i < reactants.size(); ++i){
        if(coeff[i] != 1) ss << coeff[i];
        ss << react[i];
        if(i != reactants.size()-1) ss << " + ";
    }
    ss << " = ";
    for(int i = 0; i < products.size(); ++i){
        if(coeff[reactants.size()+i] != 1) ss << coeff[reactants.size()+i];
        ss << prod[i];
        if(i != products.size()-1) ss << " + ";
    }
    balanced = ss.str();
    return true;
}

int main(){
    while(true){
        cout << "请输入化学或离子方程式（或输入 'exit' 退出）：\n> ";
        string eq;
        getline(cin, eq);
        if(eq == "exit" || eq == "quit") break;
        string balanced;
        if(balanceEquation(eq, balanced))
            cout << "配平后的方程式：\n" << balanced << "\n\n";
        else
            cout << "错误：无法解析或配平方程式。请检查输入格式。\n\n";
    }
    return 0;
}
