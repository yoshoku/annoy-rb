/**
 * Annoy.rb is a Ruby binding for the Annoy (Approximate Nearest Neighbors Oh Yeah).
 *
 * Copyright (c) 2020-2025 Atsushi Tatsuma
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANNOYEXT_HPP
#define ANNOYEXT_HPP 1

#include <typeinfo>

#include <ruby.h>

#include <annoylib.h>
#include <kissrandom.h>

using namespace Annoy;

#ifdef ANNOYLIB_MULTITHREADED_BUILD
typedef AnnoyIndexMultiThreadedBuildPolicy AnnoyIndexThreadedBuildPolicy;
#else
typedef AnnoyIndexSingleThreadedBuildPolicy AnnoyIndexThreadedBuildPolicy;
#endif

// clang-format off
template<typename El> using AnnoyIndexAngular = AnnoyIndex<int32_t, El, Angular, Kiss64Random, AnnoyIndexThreadedBuildPolicy>;
template<typename El> using AnnoyIndexDotProduct = AnnoyIndex<int32_t, El, DotProduct, Kiss64Random, AnnoyIndexThreadedBuildPolicy>;
template<typename El> using AnnoyIndexHamming = AnnoyIndex<int32_t, El, Hamming, Kiss64Random, AnnoyIndexThreadedBuildPolicy>;
template<typename El> using AnnoyIndexEuclidean = AnnoyIndex<int32_t, El, Euclidean, Kiss64Random, AnnoyIndexThreadedBuildPolicy>;
template<typename El> using AnnoyIndexManhattan = AnnoyIndex<int32_t, El, Manhattan, Kiss64Random, AnnoyIndexThreadedBuildPolicy>;
// clang-format on

template <class Idx, typename El, class Impl>
class RbAnnoyIndex {
public:
  // static const rb_data_type_t annoy_index_type;

  static VALUE annoy_index_alloc(VALUE self) {
    Idx* ptr = (Idx*)ruby_xmalloc(sizeof(Idx));
    new (ptr) Idx();
    return TypedData_Wrap_Struct(self, &Impl::annoy_index_type, ptr);
  };

  static void annoy_index_free(void* ptr) {
    ((Idx*)ptr)->~AnnoyIndex();
    ruby_xfree(ptr);
  };

  static size_t annoy_index_size(const void* ptr) {
    return sizeof(*((Idx*)ptr));
  };

  static Idx* get_annoy_index(VALUE self) {
    Idx* ptr;
    TypedData_Get_Struct(self, Idx, &Impl::annoy_index_type, ptr);
    return ptr;
  };

  static VALUE define_class(VALUE rb_mAnnoy, const char* class_name) {
    VALUE rb_cAnnoyIndex = rb_define_class_under(rb_mAnnoy, class_name, rb_cObject);
    rb_define_alloc_func(rb_cAnnoyIndex, annoy_index_alloc);
    rb_define_method(rb_cAnnoyIndex, "initialize", RUBY_METHOD_FUNC(_annoy_index_init), 1);
    rb_define_method(rb_cAnnoyIndex, "add_item", RUBY_METHOD_FUNC(_annoy_index_add_item), 2);
    rb_define_method(rb_cAnnoyIndex, "build", RUBY_METHOD_FUNC(_annoy_index_build), 2);
    rb_define_method(rb_cAnnoyIndex, "save", RUBY_METHOD_FUNC(_annoy_index_save), 2);
    rb_define_method(rb_cAnnoyIndex, "load", RUBY_METHOD_FUNC(_annoy_index_load), 2);
    rb_define_method(rb_cAnnoyIndex, "unload", RUBY_METHOD_FUNC(_annoy_index_unload), 0);
    rb_define_method(rb_cAnnoyIndex, "get_nns_by_item", RUBY_METHOD_FUNC(_annoy_index_get_nns_by_item), 4);
    rb_define_method(rb_cAnnoyIndex, "get_nns_by_vector", RUBY_METHOD_FUNC(_annoy_index_get_nns_by_vector), 4);
    rb_define_method(rb_cAnnoyIndex, "get_item", RUBY_METHOD_FUNC(_annoy_index_get_item), 1);
    rb_define_method(rb_cAnnoyIndex, "get_distance", RUBY_METHOD_FUNC(_annoy_index_get_distance), 2);
    rb_define_method(rb_cAnnoyIndex, "get_n_items", RUBY_METHOD_FUNC(_annoy_index_get_n_items), 0);
    rb_define_method(rb_cAnnoyIndex, "get_n_trees", RUBY_METHOD_FUNC(_annoy_index_get_n_trees), 0);
    rb_define_method(rb_cAnnoyIndex, "on_disk_build", RUBY_METHOD_FUNC(_annoy_index_on_disk_build), 1);
    rb_define_method(rb_cAnnoyIndex, "set_seed", RUBY_METHOD_FUNC(_annoy_index_set_seed), 1);
    rb_define_method(rb_cAnnoyIndex, "verbose", RUBY_METHOD_FUNC(_annoy_index_verbose), 1);
    rb_define_method(rb_cAnnoyIndex, "get_f", RUBY_METHOD_FUNC(_annoy_index_get_f), 0);
    return rb_cAnnoyIndex;
  };

private:
  static VALUE _annoy_index_init(VALUE self, VALUE _n_dims) {
    const int n_dims = NUM2INT(_n_dims);
    Idx* ptr = get_annoy_index(self);
    new (ptr) Idx(n_dims);
    return Qnil;
  };

  static VALUE _annoy_index_add_item(VALUE self, VALUE _idx, VALUE arr) {
    const int32_t idx = (int32_t)NUM2INT(_idx);
    const int n_dims = get_annoy_index(self)->get_f();

    if (!RB_TYPE_P(arr, T_ARRAY)) {
      rb_raise(rb_eArgError, "Expect item vector to be Array.");
      return Qfalse;
    }

    if (n_dims != RARRAY_LEN(arr)) {
      rb_raise(rb_eArgError, "Array size does not match to index dimensionality.");
      return Qfalse;
    }

    El* vec = (El*)ruby_xmalloc(n_dims * sizeof(El));
    for (int i = 0; i < n_dims; i++) {
      vec[i] = (El)(typeid(El) == typeid(uint64_t) ? NUM2UINT(rb_ary_entry(arr, i)) : NUM2DBL(rb_ary_entry(arr, i)));
    }

    char* error;
    if (!get_annoy_index(self)->add_item(idx, vec, &error)) {
      VALUE error_str = rb_str_new_cstr(error);
      free(error);
      ruby_xfree(vec);
      rb_raise(rb_eRuntimeError, "%s", StringValuePtr(error_str));
      return Qfalse;
    }

    ruby_xfree(vec);
    return Qtrue;
  };

  static VALUE _annoy_index_build(VALUE self, VALUE _n_trees, VALUE _n_jobs) {
    const int n_trees = NUM2INT(_n_trees);
    const int n_jobs = NUM2INT(_n_jobs);
    char* error;
    if (!get_annoy_index(self)->build(n_trees, n_jobs, &error)) {
      VALUE error_str = rb_str_new_cstr(error);
      free(error);
      rb_raise(rb_eRuntimeError, "%s", StringValuePtr(error_str));
      return Qfalse;
    }
    return Qtrue;
  };

  static VALUE _annoy_index_save(VALUE self, VALUE _filename, VALUE _prefault) {
    const char* filename = StringValuePtr(_filename);
    const bool prefault = _prefault == Qtrue ? true : false;
    char* error;
    if (!get_annoy_index(self)->save(filename, prefault, &error)) {
      VALUE error_str = rb_str_new_cstr(error);
      free(error);
      rb_raise(rb_eRuntimeError, "%s", StringValuePtr(error_str));
      return Qfalse;
    }
    RB_GC_GUARD(_filename);
    return Qtrue;
  };

  static VALUE _annoy_index_load(VALUE self, VALUE _filename, VALUE _prefault) {
    const char* filename = StringValuePtr(_filename);
    const bool prefault = _prefault == Qtrue ? true : false;
    char* error;
    if (!get_annoy_index(self)->load(filename, prefault, &error)) {
      VALUE error_str = rb_str_new_cstr(error);
      free(error);
      rb_raise(rb_eRuntimeError, "%s", StringValuePtr(error_str));
      return Qfalse;
    }
    RB_GC_GUARD(_filename);
    return Qtrue;
  };

  static VALUE _annoy_index_unload(VALUE self) {
    get_annoy_index(self)->unload();
    return Qnil;
  };

  static VALUE _annoy_index_get_nns_by_item(VALUE self, VALUE _idx, VALUE _n_neighbors, VALUE _search_k,
                                            VALUE _include_distances) {
    const int32_t idx = (int32_t)NUM2INT(_idx);
    const int n_neighbors = NUM2INT(_n_neighbors);
    const int search_k = NUM2INT(_search_k);
    const bool include_distances = _include_distances == Qtrue ? true : false;
    std::vector<int32_t> neighbors;
    std::vector<El> distances;

    get_annoy_index(self)->get_nns_by_item(idx, n_neighbors, search_k, &neighbors, include_distances ? &distances : NULL);

    const int sz_neighbors = neighbors.size();
    VALUE neighbors_arr = rb_ary_new2(sz_neighbors);

    for (int i = 0; i < sz_neighbors; i++) {
      rb_ary_store(neighbors_arr, i, INT2NUM((int)(neighbors[i])));
    }

    if (include_distances) {
      const int sz_distances = distances.size();
      VALUE distances_arr = rb_ary_new2(sz_distances);
      for (int i = 0; i < sz_distances; i++) {
        rb_ary_store(distances_arr, i, typeid(El) == typeid(uint64_t) ? UINT2NUM(distances[i]) : DBL2NUM(distances[i]));
      }
      VALUE res = rb_ary_new2(2);
      rb_ary_store(res, 0, neighbors_arr);
      rb_ary_store(res, 1, distances_arr);
      return res;
    }

    return neighbors_arr;
  };

  static VALUE _annoy_index_get_nns_by_vector(VALUE self, VALUE _vec, VALUE _n_neighbors, VALUE _search_k,
                                              VALUE _include_distances) {
    const int n_dims = get_annoy_index(self)->get_f();

    if (!RB_TYPE_P(_vec, T_ARRAY)) {
      rb_raise(rb_eArgError, "Expect item vector to be Array.");
      return Qfalse;
    }

    if (n_dims != RARRAY_LEN(_vec)) {
      rb_raise(rb_eArgError, "Array size does not match to index dimensionality.");
      return Qfalse;
    }

    El* vec = (El*)ruby_xmalloc(n_dims * sizeof(El));
    for (int i = 0; i < n_dims; i++) {
      vec[i] = (El)(typeid(El) == typeid(uint64_t) ? NUM2UINT(rb_ary_entry(_vec, i)) : NUM2DBL(rb_ary_entry(_vec, i)));
    }

    const int n_neighbors = NUM2INT(_n_neighbors);
    const int search_k = NUM2INT(_search_k);
    const bool include_distances = _include_distances == Qtrue ? true : false;
    std::vector<int32_t> neighbors;
    std::vector<El> distances;

    get_annoy_index(self)->get_nns_by_vector(vec, n_neighbors, search_k, &neighbors, include_distances ? &distances : NULL);

    ruby_xfree(vec);

    const int sz_neighbors = neighbors.size();
    VALUE neighbors_arr = rb_ary_new2(sz_neighbors);

    for (int i = 0; i < sz_neighbors; i++) {
      rb_ary_store(neighbors_arr, i, INT2NUM((int)(neighbors[i])));
    }

    if (include_distances) {
      const int sz_distances = distances.size();
      VALUE distances_arr = rb_ary_new2(sz_distances);
      for (int i = 0; i < sz_distances; i++) {
        rb_ary_store(distances_arr, i, typeid(El) == typeid(uint64_t) ? UINT2NUM(distances[i]) : DBL2NUM(distances[i]));
      }
      VALUE res = rb_ary_new2(2);
      rb_ary_store(res, 0, neighbors_arr);
      rb_ary_store(res, 1, distances_arr);
      return res;
    }

    return neighbors_arr;
  };

  static VALUE _annoy_index_get_item(VALUE self, VALUE _idx) {
    const int32_t idx = (int32_t)NUM2INT(_idx);
    const int n_dims = get_annoy_index(self)->get_f();
    El* vec = (El*)ruby_xmalloc(n_dims * sizeof(El));
    VALUE arr = rb_ary_new2(n_dims);

    get_annoy_index(self)->get_item(idx, vec);

    for (int i = 0; i < n_dims; i++) {
      rb_ary_store(arr, i, typeid(El) == typeid(uint64_t) ? UINT2NUM(vec[i]) : DBL2NUM(vec[i]));
    }

    ruby_xfree(vec);
    return arr;
  };

  static VALUE _annoy_index_get_distance(VALUE self, VALUE _i, VALUE _j) {
    const int32_t i = (int32_t)NUM2INT(_i);
    const int32_t j = (int32_t)NUM2INT(_j);
    const El dist = get_annoy_index(self)->get_distance(i, j);
    return typeid(El) == typeid(uint64_t) ? UINT2NUM(dist) : DBL2NUM(dist);
  };

  static VALUE _annoy_index_get_n_items(VALUE self) {
    const int32_t n_items = get_annoy_index(self)->get_n_items();
    return INT2NUM(n_items);
  };

  static VALUE _annoy_index_get_n_trees(VALUE self) {
    const int32_t n_trees = get_annoy_index(self)->get_n_trees();
    return INT2NUM(n_trees);
  };

  static VALUE _annoy_index_on_disk_build(VALUE self, VALUE _filename) {
    const char* filename = StringValuePtr(_filename);
    char* error;
    if (!get_annoy_index(self)->on_disk_build(filename, &error)) {
      VALUE error_str = rb_str_new_cstr(error);
      free(error);
      rb_raise(rb_eRuntimeError, "%s", StringValuePtr(error_str));
      return Qfalse;
    }
    RB_GC_GUARD(_filename);
    return Qtrue;
  };

  static VALUE _annoy_index_set_seed(VALUE self, VALUE _seed) {
    const int seed = NUM2INT(_seed);
    get_annoy_index(self)->set_seed(seed);
    return Qnil;
  };

  static VALUE _annoy_index_verbose(VALUE self, VALUE _flag) {
    const bool flag = _flag == Qtrue ? true : false;
    get_annoy_index(self)->verbose(flag);
    return Qnil;
  };

  static VALUE _annoy_index_get_f(VALUE self) {
    const int32_t f = get_annoy_index(self)->get_f();
    return INT2NUM(f);
  };
};

class RbAnnoyIndexAngular : public RbAnnoyIndex<AnnoyIndexAngular<double>, double, RbAnnoyIndexAngular> {
public:
  static const rb_data_type_t annoy_index_type;
};

const rb_data_type_t RbAnnoyIndexAngular::annoy_index_type = {
  "RbAnnoyIndexAngular", { NULL, RbAnnoyIndexAngular::annoy_index_free, RbAnnoyIndexAngular::annoy_index_size }, NULL, NULL, 0
};

class RbAnnoyIndexDotProduct : public RbAnnoyIndex<AnnoyIndexDotProduct<double>, double, RbAnnoyIndexDotProduct> {
public:
  static const rb_data_type_t annoy_index_type;
};

const rb_data_type_t RbAnnoyIndexDotProduct::annoy_index_type = {
  "RbAnnoyIndexDotProduct", { NULL, RbAnnoyIndexDotProduct::annoy_index_free, RbAnnoyIndexDotProduct::annoy_index_size }, NULL, NULL, 0
};

class RbAnnoyIndexHamming : public RbAnnoyIndex<AnnoyIndexHamming<uint64_t>, uint64_t, RbAnnoyIndexHamming> {
public:
  static const rb_data_type_t annoy_index_type;
};

const rb_data_type_t RbAnnoyIndexHamming::annoy_index_type = {
  "RbAnnoyIndexHamming", { NULL, RbAnnoyIndexHamming::annoy_index_free, RbAnnoyIndexHamming::annoy_index_size }, NULL, NULL, 0
};

class RbAnnoyIndexEuclidean : public RbAnnoyIndex<AnnoyIndexEuclidean<double>, double, RbAnnoyIndexEuclidean> {
public:
  static const rb_data_type_t annoy_index_type;
};

const rb_data_type_t RbAnnoyIndexEuclidean::annoy_index_type = {
  "RbAnnoyIndexEuclidean", { NULL, RbAnnoyIndexEuclidean::annoy_index_free, RbAnnoyIndexEuclidean::annoy_index_size }, NULL, NULL, 0
};

class RbAnnoyIndexManhattan : public RbAnnoyIndex<AnnoyIndexManhattan<double>, double, RbAnnoyIndexManhattan> {
public:
  static const rb_data_type_t annoy_index_type;
};

const rb_data_type_t RbAnnoyIndexManhattan::annoy_index_type = {
  "RbAnnoyIndexManhattan", { NULL, RbAnnoyIndexManhattan::annoy_index_free, RbAnnoyIndexManhattan::annoy_index_size }, NULL, NULL, 0
};

class RbAnnoyIndexAngularFloat32 : public RbAnnoyIndex<AnnoyIndexAngular<float>, float, RbAnnoyIndexAngularFloat32> {
public:
  static const rb_data_type_t annoy_index_type;
};

const rb_data_type_t RbAnnoyIndexAngularFloat32::annoy_index_type = {
  "RbAnnoyIndexAngularFloat32", { NULL, RbAnnoyIndexAngularFloat32::annoy_index_free, RbAnnoyIndexAngularFloat32::annoy_index_size }, NULL, NULL, 0
};

class RbAnnoyIndexDotProductFloat32 : public RbAnnoyIndex<AnnoyIndexDotProduct<float>, float, RbAnnoyIndexDotProductFloat32> {
public:
  static const rb_data_type_t annoy_index_type;
};

const rb_data_type_t RbAnnoyIndexDotProductFloat32::annoy_index_type = {
  "RbAnnoyIndexDotProductFloat32", { NULL, RbAnnoyIndexDotProductFloat32::annoy_index_free, RbAnnoyIndexDotProductFloat32::annoy_index_size }, NULL, NULL, 0
};

class RbAnnoyIndexEuclideanFloat32 : public RbAnnoyIndex<AnnoyIndexEuclidean<float>, float, RbAnnoyIndexEuclideanFloat32> {
public:
  static const rb_data_type_t annoy_index_type;
};

const rb_data_type_t RbAnnoyIndexEuclideanFloat32::annoy_index_type = {
  "RbAnnoyIndexEuclideanFloat32", { NULL, RbAnnoyIndexEuclideanFloat32::annoy_index_free, RbAnnoyIndexEuclideanFloat32::annoy_index_size }, NULL, NULL, 0
};

class RbAnnoyIndexManhattanFloat32 : public RbAnnoyIndex<AnnoyIndexManhattan<float>, float, RbAnnoyIndexManhattanFloat32> {
public:
  static const rb_data_type_t annoy_index_type;
};

const rb_data_type_t RbAnnoyIndexManhattanFloat32::annoy_index_type = {
  "RbAnnoyIndexManhattanFloat32", { NULL, RbAnnoyIndexManhattanFloat32::annoy_index_free, RbAnnoyIndexManhattanFloat32::annoy_index_size }, NULL, NULL, 0
};

#endif /* ANNOYEXT_HPP */
