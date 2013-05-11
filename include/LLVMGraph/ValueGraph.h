#ifndef VALUE_GRAPH_H
#define VALUE_GRAPH_H

#include <LLVMGraph/GraphProperties.h>
#include <MicroGraph/MicroGraph.h>

typedef MicroGraph<llvm::Value*, GraphVertexProperties, GraphEdgeProperties> VG;

typedef VG::tVertexIterator VG_vertexIterator;
typedef VG::tVertexConstIterator VG_vertexConstIterator;

typedef VG::tEdgeIterator VG_edgeIterator;
typedef VG::tEdgeConstIterator VG_edgeConstIterator;

#endif /* VALUE_GRAPH_H */
