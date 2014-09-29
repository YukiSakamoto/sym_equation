#include <string>
#include <boost/shared_ptr.hpp>

#include "node.hpp"

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
S_Node_Multiply::Differentiate(std::string name)
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
