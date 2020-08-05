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
