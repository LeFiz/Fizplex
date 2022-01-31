//
//  simplex_on_integers.cpp
//  core
//
//  Created by Andrey on 30.01.2022.
//

#include "simplex_on_integers.h"
#include <queue>

using namespace fizplex;

SimplexOnIntegers::SimplexOnIntegers(LP &a_lp)
: _lp_original(a_lp)
{
    SubIntegerLP lp_copy(&_lp_original);
    _head = std::make_unique< Node >(lp_copy, nullptr, _lp_original.column_count(), 0);
}

void SimplexOnIntegers::solve()
{
    std::queue<Node*> tree;
    tree.push(_head.get());
    
    bool sol_found = false;
    double cur_best = -inf;
    DVector solution;
    
    while (!tree.empty()) {
        auto& top_node = tree.front();
        bool built = top_node->build_tree();
        
        if (built)
        {
            sol_found = true;
            auto local_sol = top_node->get_solution();
            if (cur_best < local_sol.first) {
                cur_best = local_sol.first;
                solution = local_sol.second;
            }
        }
        else {
            if (nullptr != top_node->left_child())
                tree.push( top_node->left_child().get() );
            
            if (nullptr != top_node->right_child())
                tree.push( top_node->right_child().get() );
        }
        
        tree.pop();
    }
    
    if (sol_found) {
        _head_result = Simplex::Result::OptimalSolution;
        _head_z = cur_best;
        _head_x = solution;
    }
    else {
        _head_result = Simplex::Result::Infeasible;
    }
}

const DVector &SimplexOnIntegers::get_x() const
{
    return _head_x;
}

const double &SimplexOnIntegers::get_z() const
{
    return _head_z;
}

const Simplex::Result &SimplexOnIntegers::get_result() const
{
    return _head_result;
}
