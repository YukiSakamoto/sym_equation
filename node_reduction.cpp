#include "node.hpp"
#include <boost/shared_ptr.hpp>


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
