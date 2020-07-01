# serializability-check

Given a graph with no cycles in it, there must be a serial schedule corresponding to it.

The steps of constructing a precedence graph are :-

1. Create a node for every transaction in the schedule.

2. Find the precedence relationships in conflicting operations. Conflicting
   operations are (read-write) or (write-read) or (write–write) on the same data
   item in two different transactions. But how to find them?

  2.1 For a transaction Ti which reads an item A, find a transaction Tj that
      writes A later in the schedule. If such a transaction is found, draw an
      edge from Ti to Tj.
      
  2.2 For a transaction Ti which has written an item A, find a transaction Tj
      later in the schedule that reads A. If such a transaction is found, draw
      an edge from Ti to Tj.
      
  2.3 For a transaction Ti which has written an item A, find a transaction Tj
      that writes A later than Ti. If such a transaction is found, draw an edge
      from Ti to Tj.
      
3. If there is any cycle in the graph, the schedule is not serialisable.
