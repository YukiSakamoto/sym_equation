#include "node.hpp"

int main(void)
{
    assignment_map_type m;
    m.insert( std::make_pair(std::string("X"), double(5.0)) );

    boost::shared_ptr<S_Node> X( S_Variable("X") ), Y(S_Variable("Y") );
    std::cout << "* Expression1: f(X, Y) = 6 * X * Y" << std::endl;
    boost::shared_ptr<S_Node> expr1( 1.0 * 6.0 * 1.0 * X * Y * 1.0 * 1.0);
    std::cout << expr1 << std::endl;
    std::cout << "**    df/dX " << std::endl;
    std::cout << expr1->Differentiate("X") << std::endl;
    std::cout << "**    Reduction" << std::endl;
    std::cout << expr1->Differentiate("X")->Reduction() << std::endl;
    
    std::cout << "============================================================" << std::endl;;
    std::cout << "* Expression2: [f(X, Y) = sin(X * X + Y) + Y]" << std::endl;
    boost::shared_ptr<S_Node> expr2( S_Sin( X * X + Y ) + Y );
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
