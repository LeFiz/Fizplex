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

static uint32_t MAX_DEEP = 12;

class SubIntegerLP : public LP
{
public:
    
    SubIntegerLP(const LP* parent_lp);
    SubIntegerLP(const LP* parent_lp, size_t edge_idx, double edge_value, bool branchLess);
    SubIntegerLP(const SubIntegerLP&);
    SubIntegerLP(SubIntegerLP&&) = delete;
    SubIntegerLP* operator=(const SubIntegerLP&) = delete;
    SubIntegerLP* operator=(SubIntegerLP&&) = delete;
    
private:
    
//    LP _local_orig_lp;
    const LP *_parent_lp;
};

class Node
{
public:
    Node(SubIntegerLP, Node* parent, size_t orig_size, uint32_t deep_level);
    
    bool build_tree();
    
    std::pair<double, DVector> get_solution() const {
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
    
private:
    
    bool create_children(size_t idx, double value);
    
private:
    
    SubIntegerLP _sub_lp;
    const SubIntegerLP _backup_lp;
    
    size_t _orig_size;
    uint32_t _deep_level;
    
    Node* _parent;
    
    std::unique_ptr<Node> _left = nullptr;
    std::unique_ptr<Node> _right = nullptr;
    
    double _obj = -inf;
    DVector _x;
};

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
    std::unique_ptr<Node> _head;
    
    Simplex::Result _head_result = Simplex::Result::Unsolved;
    DVector _head_x;
    double _head_z = -inf;
    
};

}

#endif /* simplex_on_integers_hpp */
