#include <iostream>
#include <iomanip>
#include <lemon/lp.h>

#define NUMBER_OF_MONTHS 12

using namespace lemon;
using namespace std;

int main(int argc, const char *argv[]) {
    int t;
    cin >> t;

    for(int index = 0; index < t; index++) {
        Lp lp;
        double p[NUMBER_OF_MONTHS], x[NUMBER_OF_MONTHS], y[NUMBER_OF_MONTHS];
        double a[NUMBER_OF_MONTHS], b[NUMBER_OF_MONTHS];
        double A;
        double k;

        cin >> A >> k;
        for(int i = 0; i < NUMBER_OF_MONTHS; i++) {
            cin >> x[i];
        }

        for(int i = 0; i < NUMBER_OF_MONTHS; i++) {
            cin >> y[i];
        }

        for(int i = 0; i < NUMBER_OF_MONTHS; i++) {
            cin >> a[i];
        }

        for(int i = 0; i < NUMBER_OF_MONTHS; i++) {
            cin >> b[i];
        }

        for(int i = 0; i < NUMBER_OF_MONTHS; i++) {
            cin >> p[i];
        }

        std::vector<Lp::Col> u(NUMBER_OF_MONTHS);
        std::vector<Lp::Col> v(NUMBER_OF_MONTHS);
        std::vector<Lp::Col> t(NUMBER_OF_MONTHS);
        std::vector<Lp::Col> w(NUMBER_OF_MONTHS);
        Lp::Expr obj;

        lp.addColSet(u);
        lp.addColSet(v);
        lp.addColSet(t);
        lp.addColSet(w);

        lp.colLowerBound(u, 0);
        lp.colLowerBound(v, 0);
        lp.colLowerBound(t, 0);
        lp.colLowerBound(w, 0);

        for(int i = 0; i < NUMBER_OF_MONTHS; i++) {

            lp.addRow(u[i] + v[i] <= p[i]);
            lp.addRow(t[i] + w[i] <= A);

            if(i == 0) {
                lp.addRow(t[i] == u[i] - x[i]);
                lp.addRow(w[i] == v[i] - y[i]);
            } else {
                lp.addRow(t[i] == (u[i] + t[i-1]) - x[i]);
                lp.addRow(w[i] == (v[i] + w[i-1]) - y[i]);
            }

            obj += a[i] * u[i] + b[i] * v[i] + t[i] * k + w[i] * k;
        }

        lp.min();
        lp.obj(obj);
        lp.solve();
        
        if (lp.primalType() == Lp::OPTIMAL) {
            cout << fixed << setprecision(4) << lp.primal() << endl;
        } else {
            cout << "Escalonamento impossivel" << endl;
        }

    }

    return 0;
}