// Adapted from http://stackoverflow.com/questions/18563877/struggling-to-implement-simple-boostgraph-traversal

#include <stdexcept>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/graph/depth_first_search.hpp>

struct Node
{
  Node(const int t = 0) : m_t{t} {}
  int m_t;
};

/// A phylogeny is a graph of Node of different generations
/// connected by ancestry
using phylogeny = boost::adjacency_list<
  boost::vecS,
  boost::vecS,
  boost::undirectedS,
  Node
>;

using phylogeny_vd = boost::graph_traits<phylogeny>::vertex_descriptor;

class my_visitor : public boost::default_dfs_visitor
{
public:
  my_visitor(const int t_now)
   : m_t_target{t_now} {}
  void discover_vertex(phylogeny_vd vd, const phylogeny& g)
  {
    const auto t_here = g[vd].m_t;
    std::cout << t_here << std::endl;
    if (t_here == m_t_target)
    {
      throw std::runtime_error("found an extant descendant");
    }
  }

private:

  ///The generation at the current time
  const int m_t_target;
};

//Create a phylogeny in which b has an extant relative,
// and e has not
//
//        b---c---d
//        |
//    a---+
//        |
//        e---f
//
// ---+---+---+---+---
//    0   1   2   3  t (generation)
phylogeny create_phylogeny()
{
  phylogeny p;
  const auto a = boost::add_vertex(Node(0), p);
  const auto b = boost::add_vertex(Node(1), p);
  const auto c = boost::add_vertex(Node(2), p);
  const auto d = boost::add_vertex(Node(3), p);
  const auto e = boost::add_vertex(Node(1), p);
  const auto f = boost::add_vertex(Node(2), p);
  boost::add_edge(a, b, p);
  boost::add_edge(b, c, p);
  boost::add_edge(c, d, p);
  boost::add_edge(a, e, p);
  boost::add_edge(e, f, p);
  return p;
}

bool has_extant_descendant(
  const phylogeny& p,
  const int t_end
)
{
  try
  {
    my_visitor v(t_end);
    boost::depth_first_search(
      p,
      boost::visitor(v)
    );
    return false;
  }
  catch (std::runtime_error&)
  {
    return true;
  }
}

int main()
{
  const phylogeny p = create_phylogeny();
  assert(has_extant_descendant(p, 3));
}
