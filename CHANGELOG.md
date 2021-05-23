## 0.3.0
- Add type declaration file: sig/annoy.rbs
- Fix get_distance method to return integer typed value on hamming metric index.
- Rename native extension files.

## 0.2.3
- Add GC guard to index saving and loading methods.

## 0.2.2
- Replace Data_ functions to TypedData_ functions.

## 0.2.1
- Fix to free char array of error message before calling rb_raise.
- Fix to use array allocated with ruby_xmalloc instead of vector class in C++.

## 0.2.0
- Update bundled Annoy version to 1.17.0.
- Support multithreaded index building.

## 0.1.0
- First release.
