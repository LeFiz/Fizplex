//
//  node_on_integer.cpp
//  core
//
//  Created by Andrey on 31.01.2022.
//

#include "node_on_integer.h"
#include "simplex.h"

using namespace fizplex;


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
std::unique_ptr<Node>& Node::left_child()
{
    return _left;
}
std::unique_ptr<Node>& Node::right_child()
{
    return _right;
}

bool Node::build_tree()
{
    auto subSmx = Simplex(_sub_lp);
    
    subSmx.solve();
    
    auto res_left = subSmx.get_result();
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
    
    create_children(none_integer_idx, x[none_integer_idx]);
    
    return false;
}

bool Node::create_children(size_t idx, double value)
{
    if (_deep_level >= MAX_DEEP) {
        return false;
    }
    
    auto subLP_left = SubIntegerLP(&_backup_lp, idx, value, true);
    _left = std::make_unique< Node >(subLP_left, this, _orig_size, _deep_level + 1);
    
    auto subLP_right = SubIntegerLP(&_backup_lp, idx, value, false);
    _right = std::make_unique< Node >(subLP_right, this, _orig_size, _deep_level + 1);
    
    return true;
}
double Node::lower_bound() const
{
    auto lp0 = LP(_backup_lp);
    
    // invert the objective 
    for (size_t i=0; i<_orig_size; ++i)
    {
        auto o0 = lp0.get_obj_value(i);
        lp0.add_obj_value(i, -o0);
    }
    auto bsplx = Simplex(lp0);
    bsplx.solve();
    
    return bsplx.get_z();
}

std::pair<double, DVector> Node::get_solution() const {
    double z_left = -inf;
    DVector x_left;
    if (_left != nullptr) {
        std::tie(z_left, x_left) = _left->get_solution();
    }
    if (_right != nullptr) {
        auto res_right = _right->get_solution();
        
        if (res_right.first > z_left) {
            return res_right;
        }
        else {
            return std::make_pair(z_left, x_left);
        }
    }
    return std::make_pair(_obj, _x);
}
