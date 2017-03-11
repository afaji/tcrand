#include <tcframe/spec.hpp>
#include <tcrand/tree.hpp>
#include <bits/stdc++.h>

using namespace tcframe;
using namespace tcrand;
using namespace std;

class ProblemSpec : public BaseProblemSpec {
protected:
    int N;
    vector<int> U, V;

    int res;

    void InputFormat() {
        LINE(N);
        LINES(U, V) % SIZE(N - 1);
    }

    void OutputFormat() {
        LINE(res);
    }

    void GradingConfig() {
        TimeLimit(1);
        MemoryLimit(64);
    }

    void StyleConfig() {
        NoOutput();
    }

    void Constraints() {
        CONS(1 <= N && N <= 1000);
    }
};

class TestSpec : public BaseTestSpec<ProblemSpec> {
private:
    TreeRandomizer treeRand;
    bool manual;

protected:
    void BeforeTestCase() {
        treeRand = TreeRandomizer();
        manual = false;
    }

    void AfterTestCase() {
        if (manual) {
            return;
        }
        Tree tree = treeRand.next();

        N = tree.node();
        U = tree.pathFrom();
        V = tree.pathTo();
    }

    void TestCases() {
        CASE(manual = true, N = 2, U = {0}, V = {1});
        CASE(treeRand
            .node(5)
            .binary()
        );
        CASE(treeRand
            .node(6)
            .caterpillar()
        );
    }
};
