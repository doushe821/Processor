//---------------------------------------------------------------------
//! Compares 2 double numbers with set accuracy.
//!
//! @param [in] a First number.
//! @param [in] b Second number.
//! @param [in] acc Minimal possible difference between numbers.
//!
//! @note Returns 0 if numbers are equal, 1 if a > b and -1 if a < b.
//---------------------------------------------------------------------

#include <math.h>
#include "doublecmp.h"

int doublecmp(double a, double b, double acc)
{
    double delta = a - b;
    if (fabs(delta) <= acc)
        return 0;
    else
        return (delta>0)-(delta<0);
}
