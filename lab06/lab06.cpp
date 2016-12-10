#include <iostream>
#include <iomanip>
#include <lemon/lp.h>

#define NUMBER_OF_MONTHS 12

using namespace lemon;
using namespace std;

/*
 * Represents the Chell problem
 */
typedef struct {
    double maxMonthProduction[NUMBER_OF_MONTHS];    // maximum amount of liters of beverage that chell can produce no ith month
    double whiskeyDemand[NUMBER_OF_MONTHS];         // demand in liters of whiskey in the i th month
    double vodkaDemand[NUMBER_OF_MONTHS];           // demand in liters of vodka in the i th month
    double whiskeyCost[NUMBER_OF_MONTHS];           // cost of manufacturing whiskey in reais per liter in the i th month
    double vodkaCost[NUMBER_OF_MONTHS];             // cost of manufacturing vodka in reais per liter in the i th month
    double storageCapacity;                         // maximum storage capacity, in liters, of beverage that Chell can make
    double storageCost;                             // cost, in reais, of storage per month per liter
} ChellProblem;

ChellProblem read();
Lp buildLinearProblem(ChellProblem problem);

int main(int argc, const char *argv[]) {
    int t;
    cin >> t;

    for(int index = 0; index < t; index++) {
        ChellProblem problem = read();
        Lp lp = buildLinearProblem(problem);
        lp.solve();
        
        if (lp.primalType() == Lp::OPTIMAL) {
            cout << fixed << setprecision(4) << lp.primal() << endl;
        } else {
            cout << "Escalonamento impossivel" << endl;
        }

    }

    return 0;
}

ChellProblem read() {
    ChellProblem problem;

    cin >> problem.storageCapacity >> problem.storageCost;
    for(int i = 0; i < NUMBER_OF_MONTHS; i++) {
        cin >> problem.whiskeyDemand[i];
    }

    for(int i = 0; i < NUMBER_OF_MONTHS; i++) {
        cin >> problem.vodkaDemand[i];
    }

    for(int i = 0; i < NUMBER_OF_MONTHS; i++) {
        cin >> problem.whiskeyCost[i];
    }

    for(int i = 0; i < NUMBER_OF_MONTHS; i++) {
        cin >> problem.vodkaCost[i];
    }

    for(int i = 0; i < NUMBER_OF_MONTHS; i++) {
        cin >> problem.maxMonthProduction[i];
    }

    return problem;
}

Lp buildLinearProblem(ChellProblem problem) {
    Lp lp;

    std::vector<Lp::Col> producedWhiskey(NUMBER_OF_MONTHS);
    std::vector<Lp::Col> producedVodka(NUMBER_OF_MONTHS);
    std::vector<Lp::Col> whiskeyInStock(NUMBER_OF_MONTHS);
    std::vector<Lp::Col> vodkaInStock(NUMBER_OF_MONTHS);
    Lp::Expr obj;

    // setup
    lp.addColSet(producedWhiskey);
    lp.addColSet(producedVodka);
    lp.addColSet(whiskeyInStock);
    lp.addColSet(vodkaInStock);

    // non negativity restriction
    lp.colLowerBound(producedWhiskey, 0);
    lp.colLowerBound(producedVodka, 0);
    lp.colLowerBound(whiskeyInStock, 0);
    lp.colLowerBound(vodkaInStock, 0);

    for(int i = 0; i < NUMBER_OF_MONTHS; i++) {

        // maximum month production restriction
        lp.addRow(producedWhiskey[i] + producedVodka[i] <= problem.maxMonthProduction[i]);
        // storage capacity restriction
        lp.addRow(whiskeyInStock[i] + vodkaInStock[i] <= problem.storageCapacity);

        if(i == 0) {
            // first month stock amount restriction
            lp.addRow(whiskeyInStock[i] == producedWhiskey[i] - problem.whiskeyDemand[i]);
            lp.addRow(vodkaInStock[i] == producedVodka[i] - problem.vodkaDemand[i]);
        } else {
            // stock amount restriction
            lp.addRow(whiskeyInStock[i] == (producedWhiskey[i] + whiskeyInStock[i-1]) - problem.whiskeyDemand[i]);
            lp.addRow(vodkaInStock[i] == (producedVodka[i] + vodkaInStock[i-1]) - problem.vodkaDemand[i]);
        }

        // total cost for objective function
        obj += problem.whiskeyCost[i] * producedWhiskey[i] + problem.vodkaCost[i] * producedVodka[i]
                                        + whiskeyInStock[i] * problem.storageCost + vodkaInStock[i] * problem.storageCost;
    }

    lp.min();
    lp.obj(obj);

    return lp;
}
