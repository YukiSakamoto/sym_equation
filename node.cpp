#include <algorithm>
#include <string>
#include <exception>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <typeinfo>
#include <iostream>
#include <sstream>

#include "node.hpp"

//============================================================
//  Factory Functions
//============================================================
boost::shared_ptr<S_Node> S_Number(double n)
{
    boost::shared_ptr<S_Node> retval(new S_Node_Number(n));
    return retval;
}

boost::shared_ptr<S_Node> S_Variable(std::string name)
{
    boost::shared_ptr<S_Node> retval(new S_Node_Variable(name));
    return retval;
}

boost::shared_ptr<S_Node> S_Sin(boost::shared_ptr<S_Node> var)
{
    boost::shared_ptr<S_Node> retval(new S_Node_Sin(var));
    return retval;
}

boost::shared_ptr<S_Node> S_Cos(boost::shared_ptr<S_Node> var)
{
    boost::shared_ptr<S_Node> retval(new S_Node_Cos(var));
    return retval;
}

boost::shared_ptr<S_Node> S_Plus(boost::shared_ptr<S_Node> lhs, boost::shared_ptr<S_Node> rhs)
{
    boost::shared_ptr<S_Node> retval(new S_Node_Plus(lhs, rhs));
    return retval;
}

boost::shared_ptr<S_Node> S_Multiply(boost::shared_ptr<S_Node> lhs, boost::shared_ptr<S_Node> rhs)
{
    boost::shared_ptr<S_Node> retval(new S_Node_Multiply(lhs, rhs));
    return retval;
}



//============================================================
//  Reduction Functions
//============================================================

boost::shared_ptr<S_Node> 
S_Node_Plus::Reduction(void)
{
    boost::shared_ptr<S_Node> reduct_lhs = this->lhs_->Reduction();
    boost::shared_ptr<S_Node> reduct_rhs = this->rhs_->Reduction();

    // Left Operand
    if (reduct_lhs->Is_Unary() ) {
        boost::shared_ptr<S_Node_Number> reduct_num(
                boost::dynamic_pointer_cast<S_Node_Number>(reduct_lhs));
        if (reduct_num && reduct_num->N_ == 0) {
            return reduct_rhs;
        }
    }
    // Right Operand
    if (reduct_rhs->Is_Unary()) {
        boost::shared_ptr<S_Node_Number> reduct_num(
                boost::dynamic_pointer_cast<S_Node_Number>(reduct_rhs));
        if (reduct_num && reduct_num->N_ == 0) {
            return reduct_lhs;
        }
    }
    return S_Plus(reduct_lhs, reduct_rhs);
}

boost::shared_ptr<S_Node> 
S_Node_Multiply::Reduction(void)
{
    boost::shared_ptr<S_Node> reduct_lhs = this->lhs_->Reduction();
    boost::shared_ptr<S_Node> reduct_rhs = this->rhs_->Reduction();

    // Left Operand
    if (reduct_lhs->Is_Unary() ) {
        boost::shared_ptr<S_Node_Number> reduct_num(
                boost::dynamic_pointer_cast<S_Node_Number>(reduct_lhs));
        if (reduct_num && reduct_num->N_ == 1) {
            return reduct_rhs;
        }
        if (reduct_num && reduct_num->N_ == 0) {
            return S_Number(0);
        }
    }
    // Right Operand
    if (reduct_rhs->Is_Unary()) {
        boost::shared_ptr<S_Node_Number> reduct_num(
                boost::dynamic_pointer_cast<S_Node_Number>(reduct_rhs));
        if (reduct_num && reduct_num->N_ == 1) {
            return reduct_lhs;
        }
        if (reduct_num && reduct_num->N_ == 0) {
            return S_Number(0);
        }
    }
    return S_Multiply(reduct_lhs, reduct_rhs);
}

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

//============================================================
//  Functions
//============================================================
boost::shared_ptr<S_Node> Differentiate(boost::shared_ptr<S_Node> expr, std::string var)
{   return expr->Differentiate(var)->Reduction();   }

double Eval(boost::shared_ptr<S_Node> expr, assignment_map_type const &table)
{   return expr->Eval(table);   }

