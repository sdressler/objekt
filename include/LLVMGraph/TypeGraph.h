#ifndef TYPE_GRAPH_H
#define TYPE_GRAPH_H

#include <LLVMGraph/GraphProperties.h>
#include <MicroGraph/MicroGraph.h>

typedef MicroGraph<llvm::Type*, GraphVertexProperties, GraphEdgeProperties> TG;

typedef TG::tVertexIterator TG_vertexIterator;
typedef TG::tEdgeIterator TG_edgeIterator;

#endif /* TYPE_GRAPH_H */
