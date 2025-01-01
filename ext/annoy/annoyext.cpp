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

#include "annoyext.hpp"

extern "C" void Init_annoyext(void) {
  VALUE rb_mAnnoy = rb_define_module("Annoy");
  RbAnnoyIndexAngular::define_class(rb_mAnnoy, "AnnoyIndexAngular");
  RbAnnoyIndexDotProduct::define_class(rb_mAnnoy, "AnnoyIndexDotProduct");
  RbAnnoyIndexHamming::define_class(rb_mAnnoy, "AnnoyIndexHamming");
  RbAnnoyIndexEuclidean::define_class(rb_mAnnoy, "AnnoyIndexEuclidean");
  RbAnnoyIndexManhattan::define_class(rb_mAnnoy, "AnnoyIndexManhattan");

  RbAnnoyIndexAngularFloat32::define_class(rb_mAnnoy, "AnnoyIndexAngularFloat32");
  RbAnnoyIndexDotProductFloat32::define_class(rb_mAnnoy, "AnnoyIndexDotProductFloat32");
  RbAnnoyIndexEuclideanFloat32::define_class(rb_mAnnoy, "AnnoyIndexEuclideanFloat32");
  RbAnnoyIndexManhattanFloat32::define_class(rb_mAnnoy, "AnnoyIndexManhattanFloat32");
}
