#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
using qhash_result_t = uint;
#else
using qhash_result_t = size_t;
#endif
