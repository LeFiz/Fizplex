//
//  node_on_integer.h
//  fizplex
//
//  Created by Andrey on 31.01.2022.
//

#ifndef node_on_integer_h
#define node_on_integer_h

#include "lp.h"

namespace fizplex {


static uint32_t MAX_DEEP = 16;

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
    
    std::pair<double, DVector> get_solution() const;
    double lower_bound() const;
    
    std::unique_ptr<Node>& left_child();
    std::unique_ptr<Node>& right_child();
    
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

}


#endif /* node_on_integer_h */
