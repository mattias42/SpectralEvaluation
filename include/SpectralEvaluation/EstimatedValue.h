#pragma once

#ifndef ESTIMATEDVALUE_H
#define ESTIMATEDVALUE_H

/** This is a simple representation of a value with an
    estimated uncertainty */
struct EstimatedValue
{
    EstimatedValue()
        : value(0.0), error(0.0)
    {
    }

    EstimatedValue(double v, double e)
        : value(v), error(e)
    {
    }

    double value = 0.0;

    double error = 0.0;

    /** Returns the relative error of this value, i.e. error / value */
    double RelativeError() const;
};

#endif  //ESTIMATEDVALUE_H
