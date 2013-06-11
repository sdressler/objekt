#ifndef MICRO_GRAPH_H
#define MICRO_GRAPH_H

#include <map>
#include <string>
#include <vector>
#include <fstream>

template <class tVL, class tVP, class tEP>
class MicroGraph {

public:
    typedef tVL tVertex;
    typedef tVP tVertexProperty;
    typedef tEP tEdgeProperty;

    typedef typename std::map<tVertex, tVertexProperty> tVertices;
    typedef typename tVertices::iterator tVertexIterator;
    typedef typename tVertices::const_iterator tVertexConstIterator;

    //typedef typename std::multimap<tVertex, tVertex> tEdges;
    typedef typename std::pair<tVertex, tVertex> tEdge;
    typedef typename std::multimap<tEdge, tEdgeProperty> tEdges;
    typedef typename tEdges::iterator tEdgeIterator;
    typedef typename tEdges::const_iterator tEdgeConstIterator;

    //typedef typename std::map<tEdge, tEdgeProperty> tEdgeProperties;

private:
    tVertices vertices;
    tEdges edges;

    //tEdgeProperties edgeProperties;

public:
    tVertexIterator vertex_begin() { return vertices.begin(); }
    tVertexIterator vertex_end()   { return vertices.end();   }

    tVertexConstIterator vertex_cbegin() const { return vertices.cbegin(); }
    tVertexConstIterator vertex_cend()   const { return vertices.cend();   }

    tEdgeIterator   edge_begin()   { return edges.begin();    }
    tEdgeIterator   edge_end()     { return edges.end();      }

    tEdgeConstIterator edge_cbegin() const { return edges.cbegin(); }
    tEdgeConstIterator edge_cend()   const { return edges.cend();   }

    std::pair<tVertexIterator, bool> addVertex(tVertex v) {
        // Add vertex
        return vertices.insert(std::make_pair(v, tVertexProperty()));
    }

    // Add a copy of a vertex
    std::pair<tVertexIterator, bool> addVertex(tVertexIterator it) {
        return vertices.insert(std::make_pair(it->first, it->second));
    }

    void removeVertex(tVertexIterator it) {

        // Remove all edges
        for (auto e = edges.begin(); e != edges.end(); ++e) {
            //if (e.first->first == it->first || e.first->second == it->first) {
            if (e->first.first == it->first || e->first.second == it->first) {
                auto edge = std::make_pair(e->first, e->second);
                //edgeProperties.erase(edge);
                edges.erase(e);
            }
        }
        // Remove the vertex
        vertices.erase(it);
    }

    void removeEdge(tEdgeIterator it) { edges.erase(it); }

    tEdgeIterator addEdge(tVertexIterator v1, tVertexIterator v2) {
    	return edges.insert(
    		std::make_pair(
    			std::make_pair(v1->first, v2->first),
    			tEdgeProperty()
    		)
    	);
    }

    //std::pair<tEdgeIterator, tEdgeIterator> outEdges(tVertex v) {
    std::vector<tEdgeIterator> outEdges(tVertex v) {

    	std::vector<tEdgeIterator> cand;

    	// Find all matching edges
    	for (auto it = edges.begin(); it != edges.end(); ++it) {
    		tVertex vx = (it->first).first;

    		if (vx == v) {
    			cand.push_back(it);
    		}

    	}

    	return cand;
    }

    //std::pair<tEdgeIterator, tEdgeIterator> outEdges(tVertexIterator it) {
    std::vector<tEdgeIterator> outEdges(tVertexIterator it) {
    	return outEdges(it->first);
    }

    std::vector<tEdgeConstIterator> outEdges(tVertexConstIterator it) const {

        std::vector<tEdgeConstIterator> cand;

        for (auto it = edges.cbegin(); it != edges.cend(); ++it) {
            tVertex vx = (it->first).first;

            if (vx == it->first.first) {
                cand.push_back(it);
            }
        }

        return cand;
    }

    unsigned int outDegree(tVertex v) {
        auto out_edges = outEdges(v);
        //return std::distance(out_edges.first, out_edges.second);
        return out_edges.size();
    }

    unsigned int outDegree(tVertexIterator it) { return outDegree(it->first); }

    std::vector<tEdgeIterator> inEdges(tVertex v) {

        std::vector<tEdgeIterator> in_edges;

        for (auto e = edges.begin(); e != edges.end(); ++e) {

            tVertex vx = (e->first).second;

            if (vx == v) { in_edges.push_back(e); }

        }

        return in_edges;
    }

    std::vector<tEdgeIterator> inEdges(tVertexIterator it) {
        return inEdges(it->first);
    }

    std::vector<tEdgeConstIterator> inEdges(tVertex v) const {

        std::vector<tEdgeConstIterator> in_edges;

        for (auto e = edges.begin(); e != edges.end(); ++e) {

            tVertex vx = (e->first).second;

            if (vx == v) { in_edges.push_back(e); }

        }

        return in_edges;
    }

    std::vector<tEdgeConstIterator> inEdges(tVertexConstIterator it) const {
        return inEdges(it->first);
    }

    unsigned int inDegree(tVertex v) { return inEdges(v).size(); }
    unsigned int inDegree(tVertexIterator it) { return inDegree(it->first); }

    unsigned int inDegree(tVertex v) const { return inEdges(v).size(); }

    unsigned int inDegree(tVertexConstIterator it) const {
        return inDegree(it->first);
    }

    // Get the target vertex of an edge
    tVertexIterator      target(tEdgeIterator it) {
                             return vertices.find((it->first).second);
                         }

    tVertexConstIterator target(tEdgeConstIterator it) {
                             return vertices.find((it->first).second);
                         }

    tVertexIterator      source(tEdgeIterator it) {
                             return vertices.find((it->first).first);
                         }

    tVertexConstIterator source(tEdgeConstIterator it) {
                             return vertices.find((it->first).first);
                         }

    tVertexIterator operator[](tVertex v) {
        return vertices.find(v);
    }

    tVertexConstIterator operator[](tVertex v) const {
        return vertices.find(v);
    }

    int vertexDegree() const { return vertices.size(); }

};

template <class G>
class MicroGraphWriter {

private:
    G g;
    std::string file;

public:
    MicroGraphWriter(std::string _f, G &_g) {
        g = _g;
        file = _f;
    }

    void writeGraphToFile() {

        std::ofstream f(file);

        // Generate local vertex IDs
        std::map<typename G::tVertex, unsigned long> vertexIDs;
        unsigned long ID = 0;
        for (typename G::tVertexIterator it = g.vertex_begin(); it != g.vertex_end(); ++it) {
            vertexIDs.insert(std::make_pair(it->first, ID));
            ID++;
        }

        f << "digraph {\n";

        // Write graph header
        f << "    rankdir=LR;\n"
          //<< "    splines=ortho;\n"
          //<< "    compound=false;\n"
          //<< "    ranksep=equally;\n"
          << "    node [shape=rectangle style=\"filled\" color=\"gray80\" fillcolor=\"gray80\"];\n\n";

        // Write all vertices
        for (typename G::tVertexIterator it = g.vertex_begin(); it != g.vertex_end(); ++it) {
            auto vertex_id = vertexIDs.find(it->first)->second;
            auto vertex_properties = it->second;

            f << "    " << vertex_id << " [label=\"" << vertex_properties.name;
          
            if (vertex_properties.block != std::string("")) {
                f << "[" << vertex_properties.block << "]";
            }

            f << "\"]\n";
        }

        f << "\n";

        // Write all edges
        for (typename G::tEdgeIterator it = g.edge_begin(); it != g.edge_end(); ++it) {

        	auto vertex_id1 = vertexIDs.find((it->first).first)->second;
        	auto vertex_id2 = vertexIDs.find((it->first).second)->second;

        	auto edge_properties = it->second;

            f << "    " << vertex_id1 << " -> " << vertex_id2
                        //<< " [label=\"" << edge_properties.name << "\"";
                        << " [label=\"" << edge_properties.name << "\""; // ", " << edge_properties.index << "\"";
/*
            if (edge_properties.isPointer) {
            	f << " style=dashed";
            }
*/

            f << "]\n";

        }

        f << "}\n";

        f.close();

    }
};

#endif /* MICRO_GRAPH_H */
