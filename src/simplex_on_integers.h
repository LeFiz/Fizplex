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

class SubIntegerLP : public LP
{
public:
    
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
    Node(SubIntegerLP, Node* parent, size_t orig_size);
    
    double get_obj_value() const {
        if (_left != nullptr) {
            ;
        }
        if (_right != nullptr) {
            ;
        }
        return _obj;
    }
    DVector get_x() const {
        if (_left != nullptr) {
            ;
        }
        if (_right != nullptr) {
            ;
        }
        return _x;
    }
    
private:
    
    SubIntegerLP _sub_lp;
    
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
    Simplex _smx;
    
    Simplex::Result _head_result = Simplex::Result::Unsolved;
    DVector _head_x;
    double _head_z = -inf;
    bool _integer_sln_found = false;
    
};

}

#endif /* simplex_on_integers_hpp */
