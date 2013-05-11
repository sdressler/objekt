#include <LLVMGraph/KDV/ArgParser.h>

#include <llvm/Support/raw_ostream.h>

#include <llvm/TypeFinder.h>
#include <llvm/Module.h>

void ArgParser::analyzeContainerTemplateType_Map(const StructType *s, TG_vertexIterator root) {

    errs() << "map analysis\n";

    StructType *NodeType = findMapTreeNode(cast<StructType>(s->getElementType(0)));
    StructType *PairType = cast<StructType>(NodeType->getElementType(1));

    root->second.MapNodeType = NodeType;

    // Run argument analysis with types in map
    preAnalyzeArgument(NULL, PairType->getContainedType(0), root, 0);
    preAnalyzeArgument(NULL, PairType->getContainedType(1), root, 1);

}

StructType* ArgParser::findMapTreeNode(StructType *TreeType) {

    StructType *NodeType = NULL;

    for (auto it = M->begin(); it != M->end(); ++it) {

        // Find the function that is responsible for destruction the map
        // First step: find all _M_erase functions
        if ((it->getName().str()).find("_M_erase") != std::string::npos) {
        
            // Second step: parse args to find the matching "TreeType"
            for (auto arg = it->arg_begin(); arg != it->arg_end(); ++arg) {

                if (arg->getType()->getContainedType(0) == TreeType) {

                    // TreeType is found, we can assign the PairType
                    ++arg;

                    NodeType =
                        cast<StructType>(
                            arg->getType()->getContainedType(0)
                        );
                    break;

                }

            }

        }

        if (NodeType != NULL) { break; }

    }

    if (NodeType == NULL) { assert(0); }
    return NodeType;

}
