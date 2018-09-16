//
//  main.cpp
//  learn functional programming in c++
//
//  see https://zhuanlan.zhihu.com/p/36601068
//

#include <iostream>
#include <vector>
#include <math.h>

int sum(const std::vector<int>& xs);
int product(const std::vector<int>& xs);
std::string concat(const std::vector<std::string>& xs);
double sqrt(double x, double eps);

template <class T>
T reduce(std::function<void(T&, T)> f, T init, const std::vector<T>& xs) {
    T res = init;
    /*
    for (std::vector<int>::const_iterator it = xs.begin(); it != xs.end(); it++) {
        f(res, *it);
    }
    */

    for (T x: xs) {
        f(res, x);
    }

    return res;
}

int64_t int64sum(const std::vector<int>& xs) {
    return reduce<int>(
       [](int& x, int y) {
           x += y;
       },
       0,
       xs
    );
}

struct Tree {
    int payload;
    std::vector<Tree> children;
};

template <class T>
T reduceTree(std::function<void(T&, T)> f, T init, const Tree& t) {
    T res = init;
    f(res, t.payload);

    for (const Tree& s: t.children) {
        f(res, reduceTree(f, init, s));
    }

    return res;
}

auto add = [](int& x, int y) {
    x += y;
};

auto bindsum = std::bind(&reduce<int>, add, 0, std::placeholders::_1);
auto sumTree = std::bind(&reduceTree<int>, add, 0, std::placeholders::_1);

auto iterate = [](std::function<double(double)> f, double init) {
    double an = init;
    
    return [=]() mutable {
        double x = an;
        an = f(an);
        
        return x;
    };
};

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";

    std::cout << sum(std::vector<int>{}) << std::endl;
    std::cout << sum(std::vector<int>{1, 2, 3, 4, 5}) << std::endl;

    std::cout << product(std::vector<int>{}) << std::endl;
    std::cout << product(std::vector<int>{1, 2, 3, 4, 5}) << std::endl;

    std::cout << concat(std::vector<std::string>{}) << std::endl;
    std::cout << concat(std::vector<std::string>{"a", "bc", "def"}) << std::endl;

    std::cout << int64sum(std::vector<int>{}) << std::endl;
    std::cout << int64sum(std::vector<int>{1, 2, 3, 4, 5}) << std::endl;

    auto seq = iterate([](double x) { return x * 2; }, 1);
    std::cout << seq() << std::endl;
    std::cout << seq() << std::endl;
    std::cout << seq() << std::endl;

    Tree tree, child1, child2, child3, child4;
    tree.payload = 1;
    child1.payload = 2;
    child2.payload = 3;
    child3.payload = 4;
    child4.payload = 9;
    tree.children = std::vector<Tree>{child1, child2};
    // sumTree(tree) -> 6, when initialize std::vector, child1 and child2 are called by value
    // so put tree.children after child and child2's children assignment
    child1.children = std::vector<Tree>{child3};
    child2.children = std::vector<Tree>{child4};
    tree.children = std::vector<Tree>{child1, child2};
    std::cout << sumTree(tree) << std::endl;

    std::cout << sqrt(2, 1e-5) << std::endl;

    return 0;
}

int sum(const std::vector<int>& xs) {
    int res = 0;

    for (std::vector<int>::const_iterator it = xs.begin(); it != xs.end(); it++) {
        res += *it;
    }

    return res;
}

int product(const std::vector<int>& xs) {
    int res = 1;

    for (std::vector<int>::const_iterator it = xs.begin(); it != xs.end(); it++) {
        res *= *it;
    }

    return res;
}

std::string concat(const std::vector<std::string>& xs) {
    std::string res = "";

    for (std::vector<std::string>::const_iterator it = xs.begin(); it != xs.end(); it++) {
        res = res.append(*it);
    }

    return res;
}

double sqrt(double x, double eps) {
    double cur = 1.0;

    while (true) {
        double nxt = (cur + x / cur) / 2;

        if (fabs(cur - nxt) < eps) {
            break;
        }

        cur = nxt;
    }

    return cur;
}
