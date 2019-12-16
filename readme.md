[![Build Status](https://travis-ci.com/hyunsukimsokcho/miniCS.svg?token=T9n2aMC1KMsGvMxHppQX&branch=master)](https://travis-ci.com/hyunsukimsokcho/miniCS)

# miniCS

### The goal of this project is to minimise critical section in concurrent programming by applying heuristics. (e.g. GA) 
It is usually known that there is no gold standard for setting critical sections. (i.e. when to acquire lock, then when to release the lock) 
We may need to tackle some intractability such as combinatorically explosive cases due to nondeterministic thread interleaving.

### Progress
**[31 Oct]** Initial setup for group report release.

**[02 Nov]** Author registration complete.

**[05 Nov]** Started writing *abstract*.

**[28 Nov]** Wrote two easy test cases. Used Clang AST to find references to global variables.

**[29 Nov]** Used Clang AST to find scopes.

**[1 Dec]** Generated all candidates to place a lock.

**[9 Dec]** Implemented threadSanitizer error message parser.

**[11 Dec]** Started writing GA.

**[13 Dec]** Wrote *abstract* and *population generation*.

**[14 Dec]** Added machine instruction counter.

**[15 Dec]** Completed evaluation.
