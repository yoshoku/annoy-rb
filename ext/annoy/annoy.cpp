/**
 * Annoy.rb is a Ruby binding for the Annoy (Approximate Nearest Neighbors Oh Yeah).
 *
 * Copyright (c) 2020 Atsushi Tatsuma
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

#include "annoy.hpp"

extern "C"
void Init_annoy(void)
{
  VALUE rb_mAnnoy = rb_define_module("Annoy");
  VALUE rb_cAnnoyIndexAngular = RbAnnoyIndex<AnnoyIndexAngular, double>::define_class(rb_mAnnoy, "AnnoyIndexAngular");
  VALUE rb_cAnnoyIndexDotProduct = RbAnnoyIndex<AnnoyIndexDotProduct, double>::define_class(rb_mAnnoy, "AnnoyIndexDotProduct");
  VALUE rb_cAnnoyIndexHamming = RbAnnoyIndex<AnnoyIndexHamming, uint64_t>::define_class(rb_mAnnoy, "AnnoyIndexHamming");
  VALUE rb_cAnnoyIndexEuclidean = RbAnnoyIndex<AnnoyIndexEuclidean, double>::define_class(rb_mAnnoy, "AnnoyIndexEuclidean");
  VALUE rb_cAnnoyIndexManhattan = RbAnnoyIndex<AnnoyIndexManhattan, double>::define_class(rb_mAnnoy, "AnnoyIndexManhattan");
}
