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
//  Functions
//============================================================
boost::shared_ptr<S_Node> Differentiate(boost::shared_ptr<S_Node> expr, std::string var)
{   return expr->Differentiate(var)->Reduction();   }

double Eval(boost::shared_ptr<S_Node> expr, assignment_map_type const &table)
{   return expr->Eval(table);   }

