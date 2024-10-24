# Overview

After learning Java streams, I thought that I want that API in C++ for my projects (I knew about the existence of `ranges`, but I was not confident in using them at that time.)
I hoped that after implementing a basic `stream` API in C++, I get to appreciate/know about implementation and abstraction of `ranges` library in C++.

I learned a lot in this project, and the main problem that I tackled is: **Ownership problem of temporary containers**.

I solved this using this technique (maybe this is not a good technique? IDK): 
- If the input is r-value, store that container in `stream` API, and use first/last pointers to point to this "internal" container.
- If the input is l-value, simply use first/last pointers to point to "non temporary" container.
