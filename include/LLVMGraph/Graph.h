#ifndef GRAPH_H
#define GRAPH_H

#include <llvm/Value.h>
#include <llvm/Type.h>
#include <llvm/DerivedTypes.h>

#include <LLVMGraph/GraphProperties.h>
#include <MicroGraph/MicroGraph.h>

typedef MicroGraph<llvm::Value*, GraphVertexProperties, GraphEdgeProperties> G;

typedef G::tVertexIterator vertexIterator;
typedef G::tVertexConstIterator vertexConstIterator;
typedef G::tEdgeIterator edgeIterator;
typedef G::tEdgeConstIterator edgeConstIterator;

#endif /* GRAPH_H */
