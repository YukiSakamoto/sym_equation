#ifndef INCLUDE_GUARD_NODE_HPP
#define INCLUDE_GUARD_NODE_HPP


#include <map>
#include <boost/enable_shared_from_this.hpp>
#include <cmath>
#include <boost/type_traits.hpp>

typedef std::map<std::string, double> assignment_map_type;

//============================================================
//  Abstract Node Type
//============================================================
struct S_Node : public boost::enable_shared_from_this<S_Node>
{
    virtual double Eval(assignment_map_type const& values) { return 0.0; }
    virtual boost::shared_ptr<S_Node> Differentiate(std::string name){  throw;  }
    virtual size_t Num_Operand(void) {    throw;  }
    virtual boost::shared_ptr<S_Node> Reduction(){throw;}
    virtual bool Is_Unary(void) {   return this->Num_Operand() == 1;    }
    virtual std::string to_str(void) const
    { return std::string(); }
};

struct S_Node_Binary : public S_Node
{
    S_Node_Binary(
            boost::shared_ptr<S_Node> const &lhs, 
            boost::shared_ptr<S_Node> const &rhs)
        : lhs_(lhs), rhs_(rhs){;}

    boost::shared_ptr<S_Node> lhs_;
    boost::shared_ptr<S_Node> rhs_;
    virtual size_t Num_Operand(void) {    return 2;   }
};

struct S_Node_Unary : public S_Node
{
    virtual size_t Num_Operand(void) {    return 1;   }
    virtual boost::shared_ptr<S_Node> Reduction()
    {
        return shared_from_this();
    }
};

//============================================================
//  Binary Nodes
//============================================================
struct S_Node_Plus : public S_Node_Binary
{
    S_Node_Plus(
            boost::shared_ptr<S_Node> const &lhs, 
            boost::shared_ptr<S_Node> const &rhs)
        : S_Node_Binary(lhs, rhs) {}
    virtual double Eval(assignment_map_type const &values)
    {   return lhs_->Eval(values) + rhs_->Eval(values); }
    virtual boost::shared_ptr<S_Node> Differentiate(std::string name);
    //virtual void Reduction(void);
    virtual boost::shared_ptr<S_Node> Reduction();
    virtual std::string to_str(void) const;
};

struct S_Node_Multiply : public S_Node_Binary
{
    S_Node_Multiply(
            boost::shared_ptr<S_Node> const &lhs, 
            boost::shared_ptr<S_Node> const &rhs)
        : S_Node_Binary(lhs, rhs) {}
    virtual double Eval(assignment_map_type const &values)
    {   return lhs_->Eval(values) * rhs_->Eval(values); }
    virtual boost::shared_ptr<S_Node> Differentiate(std::string name);
    //virtual void Reduction(void);
    virtual boost::shared_ptr<S_Node> Reduction();
    virtual std::string to_str(void) const;
};

//============================================================
//  Unary Nodes (Terminal Nodes)
//============================================================
struct S_Node_Number : public S_Node_Unary
{
    S_Node_Number(double N) : N_(N){;}
    virtual double Eval(assignment_map_type const &values)
    {   return N_;  }
    virtual boost::shared_ptr<S_Node> Differentiate(std::string name);
    virtual std::string to_str(void) const;
    double N_;
};

struct S_Node_Variable : public S_Node_Unary
{
    S_Node_Variable(std::string variable_name) : Name_(variable_name) {;}
    virtual double Eval(assignment_map_type const &values) 
    {
        assignment_map_type::const_iterator it(values.find(this->Name_));
        if(it == values.end()) {
            throw;
        }
        return it->second;
    }
    virtual boost::shared_ptr<S_Node> Differentiate(std::string name);
    virtual std::string to_str(void) const;
    std::string Name_;
};

//============================================================
//  Unary Nodes (Function Nodes)
//============================================================
struct S_Node_Sin : public S_Node_Unary
{
    S_Node_Sin(boost::shared_ptr<S_Node> theta) : theta_(theta){;}
    virtual double Eval(assignment_map_type const &values)
    {
        double phase( this->theta_->Eval(values) );
        return std::sin(phase);
    }
    virtual boost::shared_ptr<S_Node> Differentiate(std::string name);
    virtual std::string to_str(void) const;
    boost::shared_ptr<S_Node> theta_;
};

struct S_Node_Cos : public S_Node_Unary
{
    S_Node_Cos(boost::shared_ptr<S_Node> theta) : theta_(theta){;}
    virtual double Eval(assignment_map_type const &values)
    {
        double phase( this->theta_->Eval(values) );
        return std::cos(phase);
    }
    virtual
    boost::shared_ptr<S_Node> Differentiate(std::string name);
    virtual std::string to_str(void) const;
    boost::shared_ptr<S_Node> theta_;
};

//============================================================
//  Factory Functions
//============================================================
boost::shared_ptr<S_Node> S_Number(double n);
boost::shared_ptr<S_Node> S_Variable(std::string name);
boost::shared_ptr<S_Node> S_Sin(boost::shared_ptr<S_Node> var);
boost::shared_ptr<S_Node> S_Cos(boost::shared_ptr<S_Node> var);
boost::shared_ptr<S_Node> S_Plus(boost::shared_ptr<S_Node> lhs, boost::shared_ptr<S_Node> rhs);
boost::shared_ptr<S_Node> S_Multiply(boost::shared_ptr<S_Node> lhs, boost::shared_ptr<S_Node> rhs);

//============================================================
//  Operators
//============================================================
inline
boost::shared_ptr<S_Node> operator*(boost::shared_ptr<S_Node> const lhs, boost::shared_ptr<S_Node> const rhs)
{
    boost::shared_ptr<S_Node> p(S_Multiply(lhs, rhs));
    return p;
}

template <typename T> 
boost::shared_ptr<S_Node> operator*(boost::shared_ptr<S_Node> const lhs, T const rhs) 
{
    boost::shared_ptr<S_Node> p(S_Multiply(lhs, S_Number(rhs)) );
    return p;
}

template <typename T>
boost::shared_ptr<S_Node> operator*(T const lhs, boost::shared_ptr<S_Node> const rhs)
{
    boost::shared_ptr<S_Node> p(S_Multiply(S_Number(lhs), rhs));
    return p;
}

inline
boost::shared_ptr<S_Node> operator+(boost::shared_ptr<S_Node> const lhs, boost::shared_ptr<S_Node> const rhs)
{
    boost::shared_ptr<S_Node> p(S_Plus(lhs, rhs));
    return p;
}

template <typename T>
boost::shared_ptr<S_Node> operator+(boost::shared_ptr<S_Node> const lhs, T const rhs)
{
    boost::shared_ptr<S_Node> p(S_Plus(lhs, S_Number(rhs) ));
    return p;
}

template <typename T>
boost::shared_ptr<S_Node> operator+(T const lhs, boost::shared_ptr<S_Node> const rhs)
{
    boost::shared_ptr<S_Node> p(S_Plus(S_Number(lhs), rhs));
    return p;
}

//============================================================
//  Functions
//============================================================
boost::shared_ptr<S_Node> Differentiate(boost::shared_ptr<S_Node> expr, std::string var);
double Eval(boost::shared_ptr<S_Node> expr, assignment_map_type const &table);

std::ostream& operator<<(std::ostream& os, boost::shared_ptr<S_Node> const &node);

#endif
