## 0.5.0
### Breaking change
- Remove `-march=native` and `-DANNOYLIB_MULTITHREADED_BUILD` from CXXFLAGS.
For example, the installation command to reproduce the same build as the previous version is as follows:

```
$ gem install annoy-rb -- --with-cxxflags=-march=native -DANNOYLIB_MULTITHREADED_BUILD
```

```
$ bundle config --local build.annoy-rb "--with-cxxflags=-march=native -DANNOYLIB_MULTITHREADED_BUILD"
$ bundle install
```

## 0.4.0
- Add dummy constructor call at memory allocation of binding class to prevent occuring segment fault on GC when initialize method is failed.

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
