#ifndef QRad_ROUND_H
#define QRad_ROUND_H

class QRadRound
{
public:
    QRadRound();
    static double PowerRound(double Value);
    static double PowerRound2(double Value);
    static double PowerRoundAmount(double Value);
    static bool IsEqual(double Value1, double Value2);
    static double PowerRoundAmountTruncate(double Value);
};


#endif

