#include "linalg.hpp"
#include "linalg_internal_interface.hpp"
#include <iostream>

namespace cytnx{
    namespace linalg{
        Tensor Norm(const Tensor &Tl){
            
            cytnx_error_msg(Tl.shape().size() != 1,"[Norm] error, tensor Tl ,Norm can only operate on rank-1 Tensor.%s","\n");
            cytnx_error_msg(!Tl.is_contiguous(), "[Norm] error tensor Tl must be contiguous. Call Contiguous_() or Contiguous() first%s","\n");


            //check type:
            Tensor _tl;
            Tensor out;
            if(Tl.dtype() > 4){
                //do conversion:
                _tl = Tl.astype(Type.Double);
                
            }else{
                _tl = Tl;
            }
            out.Init({1},_tl.dtype(),_tl.device());

            if(Tl.device()==Device.cpu){


                cytnx::linalg_internal::lii.Norm_ii[_tl.dtype()](out._impl->storage()._impl->Mem, 
                                                        _tl._impl->storage()._impl);
 
                return out;

            }else{
                #ifdef UNI_GPU
                    checkCudaErrors(cudaSetDevice(Tl.device()));
                    //cytnx::linalg_internal::lii.cuNorm_ii[_tl.dtype()](out._impl->storage()._impl->Mem,
                    //                                        _tl._impl->storage()._impl,
                    //                                        _tl._impl->storage()._impl);

                    cytnx_error_msg(true,"[Developing][Norm][Gpu]%s","\n");                    
                    return out;
                #else
                    cytnx_error_msg(true,"[Norm] fatal error,%s","try to call the gpu section without CUDA support.\n");
                    return Tensor();                                         
                #endif
            }    

        }

    }
}


