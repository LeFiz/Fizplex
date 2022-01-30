//
//  simplex_on_integers.cpp
//  core
//
//  Created by Andrey on 30.01.2022.
//

#include "simplex_on_integers.h"


using namespace fizplex;

SimplexOnIntegers::SimplexOnIntegers(LP &lp)
: _smx(lp)
{
    
}

void SimplexOnIntegers::solve()
{
    _smx.solve();
}

const DVector &SimplexOnIntegers::get_x() const
{
    return _smx.get_x();
}

const double &SimplexOnIntegers::get_z() const
{
    return _smx.get_z();
}

const Simplex::Result &SimplexOnIntegers::get_result() const
{
    return _smx.get_result();
}
