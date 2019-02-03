
#include "timeinterval.h"



#include "Logging.h"


TimeInterval::~TimeInterval() {
#ifdef GCC
        gettimeofday(&end, NULL);
//        std::cout << detail
//            << 1000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000
//            << " ms" << endl;
        LOG_INFO << detail << " cost "
                 << 1000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000 << " ms";

#else
        end = clock();
        std::cout << detail
            << (double)(end - start) << " ms" << std::endl;
#endif // GCC

}
