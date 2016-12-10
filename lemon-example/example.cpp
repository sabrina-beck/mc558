#include <iostream>
#include <lemon/lp.h>
using namespace lemon;
using namespace std;
int main(int argc, const char *argv[]) {
    Lp lp;

    //Variáveis: xi = total investido no fundo i
    Lp::Col x0 = lp.addCol();
    Lp::Col x1 = lp.addCol();
    Lp::Col x2 = lp.addCol();
    Lp::Col x3 = lp.addCol();
    Lp::Col x4 = lp.addCol();
    Lp::Col x5 = lp.addCol();

    //Função objetivo:
    //max z = .0865x0 +.095x1 +.10x2 +.0875x3 +.0925x4 +.09x5
    lp.max();
    lp.obj(0.0865 * x0 + 0.095 * x1 + 0.1 * x2 + 0.0875
                           * x3 + 0.0925 * x4 + 0.09 * x5);

    //Restrição (i): xi ≤ 187.500,i = 0,..., 5
    lp.addRow(x0 <= 187500);
    lp.addRow(x1 <= 187500);
    lp.addRow(x2 <= 187500);
    lp.addRow(x3 <= 187500);
    lp.addRow(x4 <= 187500);
    lp.addRow(x5 <= 187500);

    //Restrição (ii): x0 + x1 + x3 + x5 ≥ 375.000
    lp.addRow(x0 + x1 + x3 + x5 >= 375000);

    //Restrição (iii): x1 + x2 + x4 ≤ 262.500
    lp.addRow(x1 + x2 + x4 <= 262500);

    //Total investido: x0 + x1 + x2 + x3 + x4 + x5 = 750.000
    lp.addRow(x0 + x1 + x2 + x3 + x4 + x5 == 750000);

    //Não negatividade: x0,x1,x2,x3,x4,x5 ≥ 0
    lp.colLowerBound(x0, 0);
    lp.colLowerBound(x1, 0);
    lp.colLowerBound(x2, 0);
    lp.colLowerBound(x3, 0);
    lp.colLowerBound(x4, 0);
    lp.colLowerBound(x5, 0);

    //Solucionando o LP
    lp.solve();

    //Imprimindo a solução
    if (lp.primalType() == Lp::OPTIMAL) {
        cout << "Valor da funcao objetivo: " << lp.primal() << endl;
        cout << "x0 = " << lp.primal(x0) << endl;
        cout << "x1 = " << lp.primal(x1) << endl;
        cout << "x2 = " << lp.primal(x2) << endl;
        cout << "x3 = " << lp.primal(x3) << endl;
        cout << "x4 = " << lp.primal(x4) << endl;
        cout << "x5 = " << lp.primal(x5) << endl;
    } else {
        cout << "Solucao otima nao foi encontrada." << endl;
    }

    return 0;
}