/*
 * GraphProperties.h
 *
 *  Created on: Jan 14, 2013
 *      Author: bzcdress
 */

#ifndef GRAPHPROPERTIES_H_
#define GRAPHPROPERTIES_H_

#include <llvm/Value.h>
#include <llvm/Type.h>
#include <llvm/DerivedTypes.h>

enum VTypes { Undef, Block, Memory, Instruction };

enum Types {
            POD,
            STL_STRING,
            ARRAY,
            STRUCT,
            POINTER,
            FUNCTION,
            STL_CONTAINER_BEGIN,    // Dummy entry to mark begin
            STL_CONTAINER_VECTOR,
            STL_CONTAINER_MAP,
            STL_CONTAINER_END,      // Dummy entry to mark end
        };

enum MemOps { NONE, LOAD, STORE };

struct GraphVertexProperties {

    std::string name;
    std::string block;

    llvm::Value *value = NULL;
    llvm::Type *type = NULL;

    VTypes vtype = Undef;

    unsigned int level;

    bool breadcrumb = false;
    bool isPointer = false;

    unsigned int ptrDepth = 0;

    llvm::StructType *MapNodeType = NULL;

    Types t;

    uint64_t bitwidth = 0;

    unsigned int index;

};

struct GraphEdgeProperties : public GraphVertexProperties {

    MemOps memop = NONE;

};


#endif /* GRAPHPROPERTIES_H_ */
