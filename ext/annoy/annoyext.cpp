/**
 * Annoy.rb is a Ruby binding for the Annoy (Approximate Nearest Neighbors Oh Yeah).
 *
 * Copyright (c) 2020-2022 Atsushi Tatsuma
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

#include "annoyext.hpp"

extern "C" void Init_annoyext(void) {
  VALUE rb_mAnnoy = rb_define_module("Annoy");
  RbAnnoyIndex<AnnoyIndexAngular<double>, double>::define_class(rb_mAnnoy, "AnnoyIndexAngular");
  RbAnnoyIndex<AnnoyIndexDotProduct<double>, double>::define_class(rb_mAnnoy, "AnnoyIndexDotProduct");
  RbAnnoyIndex<AnnoyIndexHamming<uint64_t>, uint64_t>::define_class(rb_mAnnoy, "AnnoyIndexHamming");
  RbAnnoyIndex<AnnoyIndexEuclidean<double>, double>::define_class(rb_mAnnoy, "AnnoyIndexEuclidean");
  RbAnnoyIndex<AnnoyIndexManhattan<double>, double>::define_class(rb_mAnnoy, "AnnoyIndexManhattan");

  RbAnnoyIndex<AnnoyIndexAngular<float>, float>::define_class(rb_mAnnoy, "AnnoyIndexAngularFloat32");
  RbAnnoyIndex<AnnoyIndexDotProduct<float>, float>::define_class(rb_mAnnoy, "AnnoyIndexDotProductFloat32");
  RbAnnoyIndex<AnnoyIndexEuclidean<float>, float>::define_class(rb_mAnnoy, "AnnoyIndexEuclideanFloat32");
  RbAnnoyIndex<AnnoyIndexManhattan<float>, float>::define_class(rb_mAnnoy, "AnnoyIndexManhattanFloat32");
}
