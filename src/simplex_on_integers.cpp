//
//  simplex_on_integers.cpp
//  core
//
//  Created by Andrey on 30.01.2022.
//

#include "simplex_on_integers.h"


using namespace fizplex;

SimplexOnIntegers::SimplexOnIntegers(LP &a_lp)
: _lp_original(a_lp)
, _smx(a_lp)
{
}

void SimplexOnIntegers::solve()
{
    _smx.solve();
    
    _head_result = _smx.get_result();
    _head_x = _smx.get_x();
    _head_z = - _smx.get_z();
    
    const auto size = _lp_original.column_count();
    printf("original lp size : %lu\n", size);
    const DVector x = get_x();
    for (size_t i=0; i<size; ++i) {
        auto v = floor(x[i] + 0.5);
        
        if (abs(v - x[i]) < 1e-3) {
            ; // integer, pass
        }
        else {
            assert(!_integer_sln_found);
            
            printf("LEFT : \n");
            auto subLP = SubIntegerLP(&_lp_original, i, x[i], true);
            auto n_left = Node(subLP, nullptr, size);
            if (n_left.get_obj_value() > 0) // FIXME: need proper way to know the we have a sln there
            {
                printf("left int-solution found, level : 1\n");
                _head_z = n_left.get_obj_value();
                _head_x = n_left.get_x();
                _integer_sln_found = true;
            }

            printf("RIGHT : \n");
            auto subLP_right = SubIntegerLP(&_lp_original, i, x[i], false);
            auto n_right = Node(subLP_right, nullptr, size);
            if ((_integer_sln_found && n_right.get_obj_value() > _head_z) ||
                (!_integer_sln_found && n_right.get_obj_value() > 0)) {
                printf("right int-solution found, level : 1\n");
                _head_z = n_right.get_obj_value();
                _head_x = n_right.get_x();
                _integer_sln_found = true;
            }
            
            break;
        }
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


//=================================================================================================
SubIntegerLP::SubIntegerLP(const LP* parent_lp,
                           size_t edge_idx,
                           double edge_value,
                           bool branchLess
                           )
: LP(*parent_lp)
, _parent_lp(parent_lp)
{
    DVector coefs;
    for (size_t i=0; i<column_count(); ++i) {
        coefs.append(i == edge_idx ? 1.0 : 0.0);
    }
    
    if (branchLess) {
        double rounded = floor(edge_value);
        add_row(RowType::LE, -inf, rounded, coefs);        // x0 <= floor(x)
    }
    else {
        double rounded = ceil(edge_value);
        add_row(RowType::GE, rounded, inf, coefs);        // x0 >= ceil(x)
    }
    
//    _local_orig_lp = (LP)*this;
}
SubIntegerLP::SubIntegerLP(const SubIntegerLP& other)
: LP(other)
, _parent_lp(other._parent_lp)
{
    
}

//=================================================================================================
Node::Node(SubIntegerLP sub_lp, Node* parent, size_t orig_size)
: _sub_lp(sub_lp)
, _parent(parent)
{
    auto subSmx = Simplex(_sub_lp);
    
    subSmx.solve();
    
    auto res_left = subSmx.get_result();
    auto subx = subSmx.get_x();
    printf("result %i\n", res_left);
    printf("sub obj: %f\n", -subSmx.get_z());
    for (size_t j=0; j<orig_size; j++) {
        printf("%f\n", subx[j]);
    }
    
    if (res_left != Simplex::Result::OptimalSolution) {
        return;
    }
    
    bool integer_sol = true;
    const auto& x = subSmx.get_x();
    for (size_t i=0; i<orig_size; ++i) {
        auto v = floor(x[i] + 0.5);
        
        if (abs(v - x[i]) < 1e-3) {
            ; // integer, pass
        }
        else {
            integer_sol = false;
            break;
        }
    }
    
    if (integer_sol) {
        _obj = - subSmx.get_z();
        _x = subSmx.get_x();
    }
}
