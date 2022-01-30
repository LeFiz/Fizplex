//
//  simplex_on_integers.hpp
//  core
//
//  Created by Andrey on 30.01.2022.
//

#ifndef simplex_on_integers_hpp
#define simplex_on_integers_hpp

#include "simplex.h"

namespace fizplex
{

class SimplexOnIntegers
{
public:
    
    explicit SimplexOnIntegers(LP &lp);
    
    void solve();

    const DVector &get_x() const;
    const double &get_z() const;
    const Simplex::Result &get_result() const;
    
private:
    
    Simplex _smx;
    
};

}

#endif /* simplex_on_integers_hpp */
