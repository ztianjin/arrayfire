/*******************************************************
 * Copyright (c) 2014, ArrayFire
 * All rights reserved.
 *
 * This file is distributed under 3-clause BSD license.
 * The complete license agreement can be obtained at:
 * http://arrayfire.com/licenses/BSD-3-Clause
 ********************************************************/

#pragma once
#include <af/array.h>
#include <cl.hpp>
#include <optypes.hpp>
#include <string>
#include <vector>

namespace opencl
{

namespace JIT
{
    class Node
    {
    protected:
        std::string m_type_str;
        int m_id;
        bool m_set_id;
        bool m_gen_func;
        bool m_gen_param;
        bool m_gen_offset;
        std::vector<Node *> m_parents;

    public:

        Node(const char *type_str)
            : m_type_str(type_str), m_id(-1),
              m_set_id(false),
              m_gen_func(false),
              m_gen_param(false),
              m_gen_offset(false),
              m_parents()
        {}

        virtual void replaceChild(Node *prev, Node *curr) {};
        virtual void genKerName(std::stringstream &kerStream, bool genInputs) {}
        virtual void genParams  (std::stringstream &kerStream) {}
        virtual void genOffsets (std::stringstream &kerStream) {}
        virtual void genFuncs   (std::stringstream &kerStream) { m_gen_func = true;}

        virtual int setArgs (cl::Kernel &ker, int id) { return id; }

        virtual int setId(int id) { m_set_id = true; return id; }

        virtual void resetFlags() {}

        std::string getTypeStr() { return m_type_str; }

        bool isGenFunc() { return m_gen_func; }
        bool isGenParam() { return m_gen_param; }
        bool isGenOffset() { return m_gen_offset; }

        int getId()  { return m_id; }


        void addParent(Node *node)
        {
            m_parents.push_back(node);
        }

        void replace(Node *node)
        {
            for (size_t i = 0; i < m_parents.size(); i++) {
                m_parents[i]->replaceChild(this, node);
            }
        }

        virtual ~Node() {}
    };

}

}
