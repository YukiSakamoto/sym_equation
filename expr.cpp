#include <algorithm>
#include <string>
#include <exception>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <typeinfo>
#include <iostream>
#include <sstream>

#include "expr.hpp"

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

boost::shared_ptr<S_Node> S_Plus(boost::shared_ptr<S_Node> lhs, boost::shared_ptr<S_Node> rhs)
{
    boost::shared_ptr<S_Node> retval(new S_Node_Plus(lhs, rhs));
    return retval;
}

boost::shared_ptr<S_Node> S_Multiply(boost::shared_ptr<S_Node> lhs, boost::shared_ptr<S_Node> rhs)
{
    boost::shared_ptr<S_Node> retval(new S_Node_Multily(lhs, rhs));
    return retval;
}

//============================================================
//  Differentiate Functions
//============================================================
boost::shared_ptr<S_Node>
S_Node_Plus::Differentiate(std::string name)
{
    boost::shared_ptr<S_Node> diff(
            S_Plus(
                this->lhs_->Differentiate(name), 
                this->rhs_->Differentiate(name)
                ) );
    return diff;
}

boost::shared_ptr<S_Node>
S_Node_Multily::Differentiate(std::string name)
{
    boost::shared_ptr<S_Node> diff
        (S_Plus(
                S_Multiply(this->lhs_->Differentiate(name), this->rhs_),
                S_Multiply(this->lhs_, this->rhs_->Differentiate(name))
               ) );
    return diff;
}

boost::shared_ptr<S_Node>
S_Node_Number::Differentiate(std::string name)
{
    boost::shared_ptr<S_Node> diff(S_Number(0));
    return diff;
}

boost::shared_ptr<S_Node>
S_Node_Variable::Differentiate(std::string name)
{
    double C;
    if(this->Name_ == name) {
        C = 1;
    } else {
        C = 0;
    }
    boost::shared_ptr<S_Node> diff(S_Number(C));
    return diff;
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
S_Node_Multily::Reduction(void)
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
    s << "(+ " << this->lhs_->to_str() << ", " << this->rhs_->to_str() << ")";
    return s.str();
}

std::string S_Node_Multily::to_str() const
{
    std::stringstream s;
    s << "(* " << this->lhs_->to_str() << ", " << this->rhs_->to_str() << ")";
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


int main(void)
{
    assignment_map_type m;
    m.insert( std::make_pair(std::string("X"), double(5.0)) );
    boost::shared_ptr<S_Node> expr2( S_Multiply(S_Number(6.0), S_Variable("X")) );
    std::cout << expr2 << std::endl;
    std::cout << "==== Eval ===" << std::endl;
    std::cout << expr2->Eval(m) << std::endl;
    std::cout << "==== Differentiate by X ===" << std::endl;
    boost::shared_ptr<S_Node> diff( expr2->Differentiate("X") );
    std::cout << diff << std::endl;
    std::cout << "==== Differentiate & Reduction ===" << std::endl;
    boost::shared_ptr<S_Node> reduct(diff->Reduction());
    std::cout << reduct << std::endl;

    std::cout << "==== Another Expression ===" << std::endl;
    boost::shared_ptr<S_Node> expr3(
            S_Multiply(
                S_Multiply( S_Number(6.0), S_Variable("X") ), 
                S_Variable("Y")
                ));
    assignment_map_type m3;
    m3.insert( std::make_pair(std::string("X"), double(5.0)) );
    std::cout << expr3 << std::endl;
    std::cout << "==== Partial Differentiate by X ===" << std::endl;
    std::cout << expr3->Differentiate("X")->Reduction()  << std::endl;
    std::cout << "==== Partial Differentiate by Y ===" << std::endl;
    std::cout << expr3->Differentiate("Y")->Reduction() << std::endl;
    m3.insert( std::make_pair(std::string("Y"), double(2.0)) );
    std::cout << expr3->Eval(m3) << std::endl;
    return 0;
}
