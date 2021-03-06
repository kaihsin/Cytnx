#include "utils/utils_internal_gpu/cuSetZeros_gpu.hpp"

using namespace std;

namespace cytnx{
    namespace utils_internal{
    #ifdef UNI_GPU
        void cuSetZeros(void * c_ptr, const cytnx_uint64 &bytes){
            checkCudaErrors(cudaMemset(c_ptr,0,bytes));
        }
    #endif
    }//utils_internal
}//cytnx
