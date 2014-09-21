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
S_Node_Sin::Differentiate(std::string name)
{
    boost::shared_ptr<S_Node> diff
        (S_Multiply (
                     S_Cos( this->theta_ ),
                     this->theta_->Differentiate(name)
                    ));
    return diff;
}

boost::shared_ptr<S_Node>
S_Node_Cos::Differentiate(std::string name)
{
    boost::shared_ptr<S_Node> diff
        (S_Multiply (
                     S_Multiply( 
                         S_Number(-1.0),
                         S_Cos(this->theta_) ),
                     this->theta_->Differentiate(name)
                    ));
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

std::string S_Node_Sin::to_str() const
{
    std::stringstream s;
    s << "(Sin " << this->theta_->to_str() << ")";
    return s.str();
}

std::string S_Node_Cos::to_str() const
{
    std::stringstream s;
    s << "(Cos " << this->theta_->to_str() << ")";
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
//  Operators
//============================================================
boost::shared_ptr<S_Node> operator*(boost::shared_ptr<S_Node> const lhs, boost::shared_ptr<S_Node> const rhs)
{
    boost::shared_ptr<S_Node> p(S_Multiply(lhs, rhs));
    return p;
}

boost::shared_ptr<S_Node> operator+(boost::shared_ptr<S_Node> const lhs, boost::shared_ptr<S_Node> const rhs)
{
    boost::shared_ptr<S_Node> p(S_Plus(lhs, rhs));
    return p;
}

#define VAR     S_Variable
#define DOUBLE  S_Number


int main(void)
{
    assignment_map_type m;
    m.insert( std::make_pair(std::string("X"), double(5.0)) );

    std::cout << "* Expression1: f(X, Y) = 6 * X * Y" << std::endl;
    boost::shared_ptr<S_Node> expr1( DOUBLE(6) * VAR("X") * VAR("Y") );
    std::cout << expr1 << std::endl;
    std::cout << "**    df/dX " << std::endl;
    std::cout << expr1->Differentiate("X") << std::endl;
    std::cout << "**    Reduction" << std::endl;
    std::cout << expr1->Differentiate("X")->Reduction() << std::endl;
    
    std::cout << "============================================================" << std::endl;;
    std::cout << "* Expression2: [f(X, Y) = sin(X * X + Y) + Y]" << std::endl;
    boost::shared_ptr<S_Node> expr2( S_Sin( VAR("X") * VAR("X") + VAR("Y") ) + VAR("Y") );
    std::cout << expr2 << std::endl;
    std::cout << "** df/dX (and reduction)" << std::endl;
    std::cout << expr2->Differentiate("X")->Reduction() << std::endl;
    std::cout << "** df/dY (and reduction)" << std::endl;
    std::cout << expr2->Differentiate("Y")->Reduction() << std::endl;
    std::cout << "** df/dY | X = 0, Y = pi/2" << std::endl;
    assignment_map_type m2;
    m2.insert(std::make_pair("X", 0));
    m2.insert(std::make_pair("Y", M_PI / 2) );
    std::cout << expr2->Differentiate("Y")->Eval(m2) << std::endl;
    return 0;
}
