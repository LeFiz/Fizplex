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
{
    SubIntegerLP lp_copy(&_lp_original);
    _head = std::make_unique< Node >(lp_copy, nullptr, _lp_original.column_count(), 0);
}

void SimplexOnIntegers::solve()
{
    bool built = _head->build_tree();
    
    if (built) {
        _head_result = Simplex::Result::OptimalSolution;
        std::tie(_head_z, _head_x) = _head->get_solution();
    }
    else {
        _head_result = Simplex::Result::Infeasible;
    }
//    _smx.solve();
//
//    _head_result = _smx.get_result();
//    _head_x = _smx.get_x();
//    _head_z = - _smx.get_z();
//
//    const auto size = _lp_original.column_count();
//    printf("original lp size : %lu\n", size);
//    const DVector x = get_x();
//    for (size_t i=0; i<size; ++i) {
//        auto v = floor(x[i] + 0.5);
//
//        if (abs(v - x[i]) < 1e-3) {
//            ; // integer, pass
//        }
//        else {
//            assert(!_integer_sln_found);
//
            
//        }
//    }
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
SubIntegerLP::SubIntegerLP(const LP* parent_lp)
: LP(*parent_lp)
{
}
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
Node::Node(SubIntegerLP sub_lp, Node* parent, size_t orig_size, uint32_t deep_level)
: _sub_lp(sub_lp)
, _backup_lp(sub_lp)
, _orig_size(orig_size)
, _deep_level(deep_level)
, _parent(parent)
{
    printf("Node level of %u created\n", deep_level);
}

bool Node::build_tree()
{
    auto subSmx = Simplex(_sub_lp);
    
    subSmx.solve();
    
    ////// prints
    auto res_left = subSmx.get_result();
    auto subx = subSmx.get_x();
    printf("result %i\n", res_left);
    printf("sub obj: %f\n", -subSmx.get_z());
    for (size_t j=0; j<_orig_size; j++) {
        printf("%f\n", subx[j]);
    }
    ///////
    
    if (res_left != Simplex::Result::OptimalSolution) {
        return false;
    }
    
    bool integer_sol = true;
    size_t none_integer_idx = 0;
    const auto& x = subSmx.get_x();
    for (size_t i=0; i<_orig_size; ++i) {
        auto v = floor(x[i] + 0.5);
        
        if (abs(v - x[i]) < 1e-3) {
            ; // integer, pass
        }
        else {
            integer_sol = false;
            none_integer_idx = i;
            break;
        }
    }
    
    if (integer_sol) {
        _obj = - subSmx.get_z();
        _x = subSmx.get_x();
        
        return true;
    }
    
    return create_children(none_integer_idx, x[none_integer_idx]);
}

bool Node::create_children(size_t idx, double value)
{
    if (_deep_level >= MAX_DEEP) {
        return false;
    }
    
    printf("LEFT : \n");
    auto subLP_left = SubIntegerLP(&_backup_lp, idx, value, true);
    _left = std::make_unique< Node >(subLP_left, this, _orig_size, _deep_level + 1);
    bool left_built = _left->build_tree();

    printf("RIGHT : \n");
    auto subLP_right = SubIntegerLP(&_backup_lp, idx, value, false);
    _right = std::make_unique< Node >(subLP_right, this, _orig_size, _deep_level + 1);
    bool right_built = _right->build_tree();

    return left_built || right_built;
}
