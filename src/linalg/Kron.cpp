#include "linalg/linalg.hpp"
#include "utils/utils.hpp"
#include "Device.hpp"
#include <iostream>
namespace cytnx{

    namespace linalg{
        Tensor Kron(const Tensor &Tl, const Tensor &Tr){
            
            //checking:
            cytnx_error_msg(Tl.shape().size()==0,"[ERROR] pass empty tensor in param #1%s","\n");
            cytnx_error_msg(Tr.shape().size()==0,"[ERROR] pass empty tensor in param #2%s","\n");
            cytnx_error_msg(Tl.device()!= Tr.device(),"[ERROR] two tensor cannot on different devices.%s","\n");
            cytnx_error_msg(!Tl.is_contiguous(),"[ERROR] tensor #1 should be contiguous. suggestion: call Tensor.contiguous() or Tensor.contiguous_() first.%s","\n");
            cytnx_error_msg(!Tr.is_contiguous(),"[ERROR] tensor #2 should be contiguous. suggestion: call Tensor.contiguous() or Tensor.contiguous_() first.%s","\n");

            //check the new shape:
           
            std::vector<cytnx_uint64> new_shape;
            std::vector<cytnx_uint64> pad_shape1 = Tl.shape();
            std::vector<cytnx_uint64> pad_shape2 = Tr.shape();
            if(Tl.shape().size() > Tr.shape().size()){
                for(unsigned long long i=Tr.shape().size();i<Tl.shape().size();i++){
                    pad_shape2.push_back(1);
                }

            }else if(Tl.shape().size() < Tr.shape().size()){
                for(unsigned long long i=Tl.shape().size();i<Tr.shape().size();i++){
                    pad_shape1.push_back(1);
                }

            }
            new_shape.resize(pad_shape1.size());

            for(unsigned long long i=0;i<new_shape.size();i++){
                new_shape[i] = pad_shape1[i]*pad_shape2[i];
            }

            Tensor out(new_shape,Tl.dtype() < Tr.dtype()?Tl.dtype():Tr.dtype(),Tl.device());

            if(Tl.device()==Device.cpu){
                cytnx::linalg_internal::lii.Kron_ii[Tl.dtype()][Tr.dtype()](out._impl->storage()._impl, Tl._impl->storage()._impl, Tr._impl->storage()._impl,pad_shape1,pad_shape2);
            }else{
                #ifdef UNI_GPU
                    cytnx_error_msg(true,"[Kron] currently Kron is not support for GPU, pending for fix.%s","\n");
                    checkCudaErrors(cudaSetDevice(Tl.device()));
                    //cytnx::linalg_internal::lii.cuOuter_ii[Tl.dtype()][Tr.dtype()](out._impl->storage()._impl, Tl._impl->storage()._impl, Tr._impl->storage()._impl,i1,j1,i2,j2);
                #else
                    cytnx_error_msg(true,"[Kron] fatal error, the tensor is on GPU without CUDA support.%s","\n"); 
                #endif
            }
            


            return out;

        }

    }//linalg
}