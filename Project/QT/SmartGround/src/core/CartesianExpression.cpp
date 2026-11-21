#include "CartesianExpression.h"
#include <QRegularExpression>
#include <QStringList>
#include <cmath>

namespace {

struct Term {
    double coefficient = 0.0;
    int power = 0;
};

struct Lexer {
    QStringList tokens;
    int index = 0;
    bool ok = true;
    QString error;

    QString peek() const {
        if (index < 0 || index >= tokens.size()) return QString();
        return tokens.at(index);
    }

    QString take() {
        if (index >= tokens.size()) {
            ok = false;
            if (error.isEmpty()) error = "表达式不完整";
            return QString();
        }
        return tokens.at(index++);
    }

    bool accept(const QString& token) {
        if (peek() != token) return false;
        ++index;
        return true;
    }
};

QStringList tokenize(const QString& source, bool& ok, QString& error) {
    QStringList tokens;
    int i = 0;
    const int n = source.size();
    ok = true;
    error.clear();

    while (i < n) {
        const QChar ch = source.at(i);
        if (ch.isSpace()) {
            ++i;
            continue;
        }
        if (ch == '+' || ch == '-' || ch == '*' || ch == '(' || ch == ')' || ch == '^') {
            tokens << QString(ch);
            ++i;
            continue;
        }
        if (ch.isDigit() || ch == '.') {
            QString number;
            bool seenDot = false;
            while (i < n && (source.at(i).isDigit() || (source.at(i) == '.' && !seenDot))) {
                if (source.at(i) == '.') seenDot = true;
                number.append(source.at(i));
                ++i;
            }
            if (number.isEmpty() || number == ".") {
                ok = false;
                error = "数字格式不正确";
                return QStringList();
            }
            tokens << number;
            continue;
        }
        if (ch == QLatin1Char('x') || ch == QLatin1Char('X')) {
            tokens << QStringLiteral("x");
            ++i;
            continue;
        }
        ok = false;
        error = QString("无法识别的字符「%1」").arg(ch);
        return QStringList();
    }

    if (tokens.isEmpty()) {
        ok = false;
        error = "表达式为空";
    }
    return tokens;
}

bool parseExpression(Lexer& lexer, std::vector<Term>& out, QString& error);

bool parsePrimary(Lexer& lexer, std::vector<Term>& out, QString& error) {
    if (lexer.accept("(")) {
        if (!parseExpression(lexer, out, error)) return false;
        if (!lexer.accept(")")) {
            error = "缺少右括号";
            return false;
        }
        return true;
    }

    QString token = lexer.take();
    if (!lexer.ok) {
        error = lexer.error.isEmpty() ? QString("表达式不完整") : lexer.error;
        return false;
    }

    bool isNumber = false;
    const double value = token.toDouble(&isNumber);
    if (isNumber) {
        Term term;
        term.coefficient = value;
        term.power = 0;
        out.push_back(term);
        return true;
    }
    if (token == QLatin1String("x")) {
        Term term;
        term.coefficient = 1.0;
        term.power = 1;
        out.push_back(term);
        return true;
    }
    error = QString("无法识别的记号「%1」").arg(token);
    return false;
}

bool parseUnary(Lexer& lexer, std::vector<Term>& out, QString& error) {
    if (lexer.accept("-")) {
        std::vector<Term> inner;
        if (!parseUnary(lexer, inner, error)) return false;
        for (Term& term : inner) term.coefficient = -term.coefficient;
        for (const Term& term : inner) out.push_back(term);
        return true;
    }
    if (lexer.accept("+")) {
        return parseUnary(lexer, out, error);
    }
    return parsePrimary(lexer, out, error);
}

bool parsePower(Lexer& lexer, std::vector<Term>& out, QString& error) {
    std::vector<Term> base;
    if (!parseUnary(lexer, base, error)) return false;
    if (!lexer.accept("^")) {
        for (const Term& term : base) out.push_back(term);
        return true;
    }
    std::vector<Term> exponent;
    if (!parseUnary(lexer, exponent, error)) return false;
    int power = 0;
    for (const Term& term : exponent) power += static_cast<int>(std::lround(term.coefficient));
    if (power < 0 || power > 3) {
        error = "目前只支持一次、二次、三次函数";
        return false;
    }
    for (const Term& term : base) {
        Term scaled = term;
        scaled.power = term.power * power;
        scaled.coefficient = term.coefficient;
        out.push_back(scaled);
    }
    return true;
}

bool multiplyTerms(const std::vector<Term>& left, const std::vector<Term>& right,
                   std::vector<Term>& out, QString& error) {
    for (const Term& a : left) {
        for (const Term& b : right) {
            Term term;
            term.coefficient = a.coefficient * b.coefficient;
            term.power = a.power + b.power;
            if (term.power > 3) {
                error = "目前只支持一次、二次、三次函数";
                return false;
            }
            out.push_back(term);
        }
    }
    return true;
}

bool startsFactor(const QString& token) {
    if (token.isEmpty()) return false;
    if (token == QLatin1String("x")) return true;
    if (token == QLatin1String("(")) return true;
    bool isNumber = false;
    token.toDouble(&isNumber);
    return isNumber;
}

bool parseProduct(Lexer& lexer, std::vector<Term>& out, QString& error) {
    std::vector<Term> left;
    if (!parsePower(lexer, left, error)) return false;
    while (true) {
        if (lexer.accept("*")) {
            std::vector<Term> right;
            if (!parsePower(lexer, right, error)) return false;
            std::vector<Term> product;
            if (!multiplyTerms(left, right, product, error)) return false;
            left = product;
            continue;
        }
        if (startsFactor(lexer.peek())) {
            std::vector<Term> right;
            if (!parsePower(lexer, right, error)) return false;
            std::vector<Term> product;
            if (!multiplyTerms(left, right, product, error)) return false;
            left = product;
            continue;
        }
        break;
    }
    for (const Term& term : left) out.push_back(term);
    return true;
}

bool parseExpression(Lexer& lexer, std::vector<Term>& out, QString& error) {
    std::vector<Term> first;
    if (!parseProduct(lexer, first, error)) return false;
    for (const Term& term : first) out.push_back(term);

    while (lexer.peek() == QLatin1String("+") || lexer.peek() == QLatin1String("-")) {
        const QString op = lexer.take();
        std::vector<Term> right;
        if (!parseProduct(lexer, right, error)) return false;
        const double sign = (op == QLatin1String("-")) ? -1.0 : 1.0;
        for (Term& term : right) term.coefficient *= sign;
        for (const Term& term : right) out.push_back(term);
    }
    return true;
}

}  // namespace

CartesianExpression::Result CartesianExpression::parse(const QString& expression) {
    Result result;
    const QString source = expression.trimmed();
    if (source.isEmpty()) {
        result.error = "表达式为空";
        return result;
    }

    bool ok = false;
    QString error;
    const QStringList tokens = tokenize(source, ok, error);
    if (!ok) {
        result.error = error;
        return result;
    }

    Lexer lexer;
    lexer.tokens = tokens;
    lexer.index = 0;

    std::vector<Term> terms;
    if (!parseExpression(lexer, terms, error)) {
        result.error = error.isEmpty() ? QString("表达式格式不正确") : error;
        return result;
    }
    if (lexer.index < lexer.tokens.size()) {
        result.error = QString("表达式结尾有多余内容「%1」").arg(lexer.tokens.at(lexer.index));
        return result;
    }

    for (const Term& term : terms) {
        switch (term.power) {
        case 0: result.c0 += term.coefficient; break;
        case 1: result.c1 += term.coefficient; break;
        case 2: result.c2 += term.coefficient; break;
        case 3: result.c3 += term.coefficient; break;
        default:
            result.error = "目前只支持一次、二次、三次函数";
            return result;
        }
    }

    result.ok = true;
    return result;
}

double CartesianExpression::evaluate(const Result& result, double x) {
    return result.c3 * x * x * x + result.c2 * x * x + result.c1 * x + result.c0;
}

QString CartesianExpression::formatNumber(double value, int decimals) {
    if (std::abs(value) < 1e-12) value = 0.0;
    QString text = QString::number(value, 'f', decimals);
    if (text.contains('.')) {
        while (text.endsWith('0')) text.chop(1);
        if (text.endsWith('.')) text.chop(1);
    }
    if (text == "-0") text = "0";
    return text;
}

QString CartesianExpression::describe(const Result& result) {
    if (!result.ok) return QString();

    struct Piece {
        QString body;
        double coefficient;
        int power;
    };
    QVector<Piece> pieces;

    auto append = [&](double coefficient, int power, const QString& body) {
        if (std::abs(coefficient) < 1e-12) return;
        Piece piece;
        piece.body = body;
        piece.coefficient = coefficient;
        piece.power = power;
        pieces.push_back(piece);
    };

    append(result.c3, 3, QString("%1x^3").arg(formatNumber(std::abs(result.c3))));
    append(result.c2, 2, QString("%1x^2").arg(formatNumber(std::abs(result.c2))));
    append(result.c1, 1, QString("%1x").arg(formatNumber(std::abs(result.c1))));
    append(result.c0, 0, formatNumber(std::abs(result.c0)));

    if (pieces.isEmpty()) return QString("y = 0");

    QString body;
    for (int i = 0; i < pieces.size(); ++i) {
        const Piece& piece = pieces.at(i);
        const bool negative = piece.coefficient < 0.0;
        const bool needSign = i > 0;
        if (needSign) {
            body += negative ? QString(" - ") : QString(" + ");
        } else if (negative) {
            body += QString("-");
        }
        body += piece.body;
    }
    return QString("y = %1").arg(body);
}

QString CartesianExpression::describeSegment(double x1, double y1, double x2, double y2) {
    const double dx = x2 - x1;
    const double dy = y2 - y1;
    if (std::abs(dx) < 1e-9) {
        return QString("x = %1").arg(formatNumber(x1));
    }
    const double k = dy / dx;
    const double b = y1 - k * x1;
    if (std::abs(k) < 1e-9) {
        return QString("y = %1").arg(formatNumber(b));
    }
    if (std::abs(k - 1.0) < 1e-9) {
        return QString("y = x %1 %2")
            .arg(b >= 0 ? QString("+") : QString("-"), formatNumber(std::abs(b)));
    }
    if (std::abs(k + 1.0) < 1e-9) {
        return QString("y = -x %1 %2")
            .arg(b >= 0 ? QString("+") : QString("-"), formatNumber(std::abs(b)));
    }
    return QString("y = %1x %2 %3")
        .arg(formatNumber(k), b >= 0 ? QString("+") : QString("-"), formatNumber(std::abs(b)));
}
