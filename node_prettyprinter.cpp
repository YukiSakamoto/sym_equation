#include "node.hpp"
#include <sstream>
#include <iostream>
#include <string>
#include <boost/shared_ptr.hpp>

//============================================================
//  Print Functions(Not Instance Functions)
//============================================================
std::string S_Node_Plus::to_str() const
{
    std::stringstream s;
    s << "(" << this->lhs_->to_str() << " + " << this->rhs_->to_str() << ")";
    return s.str();
}

std::string S_Node_Multiply::to_str() const
{
    std::stringstream s;
    s << /*"(" <<*/ this->lhs_->to_str() << " * " << this->rhs_->to_str()/* << ")"*/;
    return s.str();
}

std::string S_Node_Sin::to_str() const
{
    std::stringstream s;
    s << "Sin " << "(" << this->theta_->to_str() << ")";
    return s.str();
}

std::string S_Node_Cos::to_str() const
{
    std::stringstream s;
    s << "Cos " << "(" << this->theta_->to_str() << ")";
    return s.str();
}

std::string S_Node_Number::to_str() const
{
    std::stringstream s;
    s << this->N_;
    return s.str();
}

std::string S_Node_Variable::to_str() const
{
    std::stringstream s;
    s << this->Name_;
    return s.str();
}

std::ostream& operator<<(std::ostream& os, boost::shared_ptr<S_Node> const &node) 
{
        os << node->to_str();
        return os;
}

