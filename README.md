# boost_graph_example_6: showing the vertices visited by depth-first traversal

Branch|[![Travis CI logo](TravisCI.png)](https://travis-ci.org)
---|---
master|[![Build Status](https://travis-ci.org/richelbilderbeek/boost_graph_example_6.svg?branch=master)](https://travis-ci.org/richelbilderbeek/boost_graph_example_6)

A [Boost.Graph example](https://github.com/richelbilderbeek/boost_graph_examples) 
that searches a phylogeny for individuals with extant descendants.

Phylogeny created:

```
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
```

Timepoints starting from `b` (which should be `b`, `c` and `d`):

```
1
2
3
```

Timepoints starting from `e` (which should be `e` and `f`):

```
1
2
```