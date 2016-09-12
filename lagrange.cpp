#include "lagrange.h"

#include <QDebug>

double lagrange(QVector<double>* x, QVector<double>* y, double value)
{
    double basePolynome, result = 0;


    for (int i = 0; i < x->size(); ++i)
    {
        basePolynome = 1;

//        qDebug() << "i =" << i << "Result:" << result << "Value: " <<value;
//        qDebug() << "l[j] = ( X - x[j] ) / ( x[i] - x[j])";
//        qDebug() << "x[i]=" << x->at(i);

        for (int j = 0; j < x->size(); ++j)
        {
            if (j != i)
            {
                basePolynome *= (value - x->at(j)) / (x->at(i) - x->at(j));
//                qDebug() << "l[" << j << "]\t( X - x[" << j << "] ) / ( x["
//                         << i << "] - x[" << j << "]) = "
//                         << (value - x->at(j)) / (x->at(i) - x->at(j));

//                if (j == x->size() - 1)
//                {
//                    qDebug() << "\nl[" << i << "] = " << basePolynome
//                             << "\t* y[" << i << "] = " << y->at(i)
//                             << " = " << basePolynome * y->at(i);
//                }
            }
        }
        result += basePolynome * y->at(i);
//        qDebug() << "Result[" << i << "] = " << result << "\n\n";
    }
    return result;
}
