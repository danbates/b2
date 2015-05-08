//
//  UnaryOperator.h
//  b2Test
//
//  Created by Collins, James B. on 5/1/15.
//  Copyright (c) 2015 West Texas A&M University. All rights reserved.
//

#ifndef b2Test_UnaryOperator_h
#define b2Test_UnaryOperator_h


#include <vector>

#include "Operator.h"


class UnaryOperator : public Operator
{
protected:
    std::shared_ptr<Node> children;
    
public:
    virtual void add_Child(std::shared_ptr<Node> child) override
    {
        children = std::move(child);
    }
    

    //Return the only child for the unary operator
    std::shared_ptr<Node> get_first_child()
    {
        return children;
    }

    
    ////////////// TESTING /////////////////
    virtual void printTree()
    {
        for(int ii = 0; ii < tabcount; ++ii)
        {
            std::cout << "\t";
        }
        std::cout << boost::typeindex::type_id_runtime(*this).pretty_name() << " = " << this->eval<dbl>()<< std::endl;
        tabcount++;
        children->printTree();
        tabcount--;
    }
    ////////////// TESTING /////////////////
    

    

};



#endif
