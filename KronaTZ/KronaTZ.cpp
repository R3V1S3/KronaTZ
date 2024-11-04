#include <iostream>
#include <cmath>

// Переменная для точности
extern double Precsn = 0.001;

// Структура knurbs для хранения NURBS-кривой
struct knurbs {
    double* ctp;   
    int kctp;       
    double* knot;  
    double* wgt;    
    int deg;       
};

// Прототипы функций
double DistanceToNURBS(struct knurbs* kr, double* tz, double* tkr);
int NURBSIntersection(struct knurbs* kr1, struct knurbs* kr2, double* tpk);

int main() {
    setlocale(LC_ALL, "Russian");


    knurbs kr1, kr2;

    // Данные для первой кривой
    kr1.deg = 3;
    kr1.kctp = 4;
    double ctp1[12] = { 35.0, -20.0, 0.0, 35.0, 15.0, 0.0, 95.0, 15.0, 0.0, 25.0, 40.0, 0.0 };
    double knot1[6] = { 0.0, 0.0, 0.0, 4.0, 4.0, 4.0 };
    kr1.ctp = ctp1;
    kr1.knot = knot1;
    kr1.wgt = nullptr;

    // Данные для второй кривой
    kr2.deg = 2;
    kr2.kctp = 3;
    double ctp2[9] = { 65.0, 0.0, 0.0, -9.641016, 0.0, 0.0, 55.0, 37.320508, 0.0 };
    double knot2[4] = { 0.0, 0.0, 2.6179938779914944, 2.6179938779914944 };
    double wgt2[3] = { 1.0, 0.2588190451025207, 1.0 };
    kr2.ctp = ctp2;
    kr2.knot = knot2;
    kr2.wgt = wgt2;

    // Данные для проверки расстояния
    double tz[3] = { 90.0, 95.0, 0.0 };
    double tkr[3] = { 0.0, 0.0, 0.0 };

    // Вывод минимального расстояния до кривой
    double distance = DistanceToNURBS(&kr1, tz, tkr);
    std::cout << "Минимальное расстояние от точки до NURBS-кривой: " << distance << " мм\n";
    std::cout << "Точка на кривой: {" << tkr[0] << ", " << tkr[1] << ", " << tkr[2] << "}\n";

    // Вывод точки пересечения
    double tpk[3] = { 0.0, 0.0, 0.0 };
    int result = NURBSIntersection(&kr1, &kr2, tpk);

    if (result == 1) {
        std::cout << "Точка пересечения найдена: {" << tpk[0] << ", " << tpk[1] << ", " << tpk[2] << "}\n";
    }
    else {
        std::cout << "Точки пересечения не найдены.\n";
    }

    return 0;
}

// Функция для вычисления минимального расстояния от точки до NURBS-кривой
double DistanceToNURBS(knurbs* kr, double* tz, double* tkr) {

    double minDist = std::numeric_limits<double>::max();
    double dist;

    for (int i = 0; i < kr->kctp; i++) {
        double dx = tz[0] - kr->ctp[i * 3];
        double dy = tz[1] - kr->ctp[i * 3 + 1];
        double dz = tz[2] - kr->ctp[i * 3 + 2];
        dist = std::sqrt(dx * dx + dy * dy + dz * dz);

        if (dist < minDist) {
            minDist = dist;
            if (tkr != NULL) {
                tkr[0] = kr->ctp[i * 3];
                tkr[1] = kr->ctp[i * 3 + 1];
                tkr[2] = kr->ctp[i * 3 + 2];
            }
        }
    }
    return minDist;
}

// Функция для определения точки пересечения двух NURBS-кривых
int NURBSIntersection(knurbs* kr1, knurbs* kr2, double* tpk) {

    for (int i = 1; i < kr1->kctp - 1; i++) {
        for (int j = 1; j < kr2->kctp - 1; j++) {
            if (std::abs(kr1->ctp[i * 3] - kr2->ctp[j * 3]) < Precsn &&
                std::abs(kr1->ctp[i * 3 + 1] - kr2->ctp[j * 3 + 1]) < Precsn &&
                std::abs(kr1->ctp[i * 3 + 2] - kr2->ctp[j * 3 + 2]) < Precsn) {

                tpk[0] = kr1->ctp[i * 3];
                tpk[1] = kr1->ctp[i * 3 + 1];
                tpk[2] = kr1->ctp[i * 3 + 2];
                return 1;
            }
        }
    }
    return 0;
}