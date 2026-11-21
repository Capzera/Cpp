#ifndef CARTESIANEXPRESSION_H
#define CARTESIANEXPRESSION_H
#include <QString>

class CartesianExpression {
public:
    struct Result {
        bool ok = false;
        double c3 = 0.0;
        double c2 = 0.0;
        double c1 = 0.0;
        double c0 = 0.0;
        QString error;
    };

    static Result parse(const QString& expression);
    static double evaluate(const Result& result, double x);
    static QString formatNumber(double value, int decimals = 4);
    static QString describe(const Result& result);
    static QString describeSegment(double x1, double y1, double x2, double y2);
};

#endif
