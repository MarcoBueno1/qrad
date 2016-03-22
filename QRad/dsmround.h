#ifndef DSM_ROUND_H
#define DSM_ROUND_H

class dsmRound
{
public:
    dsmRound();
    static double PowerRound(double Value);
    static double PowerRound2(double Value);
    static double PowerRoundAmount(double Value);
    static bool IsEqual(double Value1, double Value2);
    static double PowerRoundAmountTruncate(double Value);
};


#endif

