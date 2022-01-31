//
//  simplex_on_integers.hpp
//  core
//
//  Created by Andrey on 30.01.2022.
//

#ifndef simplex_on_integers_hpp
#define simplex_on_integers_hpp

#include "simplex.h"
#include "node_on_integer.h"

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
    
    const LP _lp_original;
//    Simplex _smx;
    std::unique_ptr<Node> _head = nullptr;
    
    Simplex::Result _head_result = Simplex::Result::Unsolved;
    DVector _head_x;
    double _head_z = -inf;
    
};

}

#endif /* simplex_on_integers_hpp */
