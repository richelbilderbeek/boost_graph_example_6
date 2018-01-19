// Adapted from http://stackoverflow.com/questions/18563877/struggling-to-implement-simple-boostgraph-traversal

#include <stdexcept>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/graph/depth_first_search.hpp>

struct Node
{
  Node(const int t = 0, const std::string& name = "")
    : m_t{t}, m_name{name} {}
  int m_t;
  std::string m_name;
};

/// A phylogeny is a graph of Node of different generations
/// connected by ancestry
using phylogeny = boost::adjacency_list<
  boost::vecS,
  boost::vecS,
  boost::directedS,
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
      // must terminate a search by throwing an exception
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
//        b-->c-->d
//        |
//    a-->+
//        |
//        e-->f
//
// ---+---+---+---+---
//    0   1   2   3  t (generation)
phylogeny create_phylogeny()
{
  phylogeny p;
  const auto a = boost::add_vertex(Node(0, "a"), p);
  const auto b = boost::add_vertex(Node(1, "b"), p);
  const auto c = boost::add_vertex(Node(2, "c"), p);
  const auto d = boost::add_vertex(Node(3, "d"), p);
  const auto e = boost::add_vertex(Node(1, "e"), p);
  const auto f = boost::add_vertex(Node(2, "f"), p);
  boost::add_edge(a, b, p);
  boost::add_edge(b, c, p);
  boost::add_edge(c, d, p);
  boost::add_edge(a, e, p);
  boost::add_edge(e, f, p);
  return p;
}

bool has_extant_descendant(
  const phylogeny_vd vd,
  const phylogeny& p,
  const int t_end
)
{
  try
  {
    std::vector<boost::default_color_type> color_map(boost::num_vertices(p));
    my_visitor v(t_end);

    boost::depth_first_visit(
      p,
      vd,
      v,
      //color map
      boost::make_iterator_property_map(
        std::begin(color_map),
        boost::get(boost::vertex_index, p),
        color_map[0]
      )
    );
    return false;
  }
  catch (std::runtime_error&)
  {
    // depth_first_visit is terminated by an exception
    return true;
  }
}

/// Showing the (timepoint of) vertices that are descended
/// from an ancestor vertex, using a depth-first search
int main()
{
  const phylogeny p = create_phylogeny();
  {
    const auto vd_b = boost::vertex(1, p);
    assert(p[vd_b].m_name == "b");
    assert(has_extant_descendant(vd_b, p, 3));
  }
  std::cout << '\n';
  {
    const auto vd_e = boost::vertex(4, p);
    assert(p[vd_e].m_name == "e");
    assert(!has_extant_descendant(vd_e, p, 3));
  }
}
