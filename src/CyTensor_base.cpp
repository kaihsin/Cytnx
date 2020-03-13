#include "CyTensor.hpp"
#include "Tensor.hpp"
#include "linalg.hpp"
#include <string>

namespace cytnx_extension{
    using namespace cytnx;
    //====================================================    
    std::string CyTensorType_class::getname(const int &ut_type){
        if(ut_type==this->Void){
            return std::string("Void (un-initialize CyTensor");
        }else if(ut_type==this->Dense){
            return std::string("Dense");
        }else if(ut_type==this->Sparse){
            return std::string("Sparse (block-form)");
        }else{
            cytnx_error_msg(true,"%s\n","[ERROR] invalid ut_type");
            return std::string("");
        }
        // extend more in here!!
    }
    CyTensorType_class UTenType;
    //===================================================

        void CyTensor_base::Init(const std::vector<Bond> &bonds, const std::vector<cytnx_int64> &in_labels, const cytnx_int64 &Rowrank,const unsigned int &dtype,const int &device,const bool &is_diag){
            cytnx_error_msg(true,"[ERROR] fatal internal, cannot call on a un-initialize CyTensor_base%s","\n");
        }
        void CyTensor_base::Init_by_Tensor(const Tensor &in_tensor, const cytnx_uint64 &Rowrank){
            cytnx_error_msg(true,"[ERROR] fatal internal, cannot call on a un-initialize CyTensor_base%s","\n");
        }   
        
        std::vector<cytnx_uint64> CyTensor_base::shape() const{
            cytnx_error_msg(true,"[ERROR] fatal internal, cannot call on a un-initialize CyTensor_base%s","\n");
            return std::vector<cytnx_uint64>();
        }
        bool      CyTensor_base::is_blockform() const{
            cytnx_error_msg(true,"[ERROR] fatal internal, cannot call on a un-initialize CyTensor_base%s","\n");
            return false;
        }
        bool     CyTensor_base::is_contiguous() const{
            cytnx_error_msg(true,"[ERROR] fatal internal, cannot call on a un-initialize CyTensor_base%s","\n");
            return false;
        }
        void CyTensor_base::to_(const int &device){
            cytnx_error_msg(true,"[ERROR] fatal internal, cannot call on a un-initialize CyTensor_base%s","\n");
        }
        boost::intrusive_ptr<CyTensor_base> CyTensor_base::to(const int &device){
            cytnx_error_msg(true,"[ERROR] fatal internal, cannot call on a un-initialize CyTensor_base%s","\n");
            return nullptr;
        }
        boost::intrusive_ptr<CyTensor_base> CyTensor_base::clone() const{
            cytnx_error_msg(true,"[ERROR] fatal internal, cannot call on a un-initialize CyTensor_base%s","\n");
            return nullptr;
        }
        unsigned int  CyTensor_base::dtype() const{
            cytnx_error_msg(true,"[ERROR] fatal internal, cannot call on a un-initialize CyTensor_base%s","\n");
            return 0;
        }
       int          CyTensor_base::device() const{
            cytnx_error_msg(true,"[ERROR] fatal internal, cannot call on a un-initialize CyTensor_base%s","\n");
            return -1;
        }
        std::string      CyTensor_base::dtype_str() const{
            cytnx_error_msg(true,"[ERROR] fatal internal, cannot call on a un-initialize CyTensor_base%s","\n");
            return std::string("");
        }
        std::string     CyTensor_base::device_str() const{
            cytnx_error_msg(true,"[ERROR] fatal internal, cannot call on a un-initialize CyTensor_base%s","\n");
            return std::string("");
        }
        boost::intrusive_ptr<CyTensor_base> CyTensor_base::permute(const std::vector<cytnx_int64> &mapper,const cytnx_int64 &Rowrank, const bool &by_label){
            cytnx_error_msg(true,"[ERROR] fatal internal, cannot call on a un-initialize CyTensor_base%s","\n");
            return nullptr;
        }
        void CyTensor_base::permute_(const std::vector<cytnx_int64> &mapper, const cytnx_int64 &Rowrank, const bool &by_label){
            cytnx_error_msg(true,"[ERROR] fatal internal, cannot call on a un-initialize CyTensor_base%s","\n");
        }

        boost::intrusive_ptr<CyTensor_base> CyTensor_base::contiguous_(){
            cytnx_error_msg(true,"[ERROR] fatal internal, cannot call on a un-initialize CyTensor_base%s","\n");
        }
        boost::intrusive_ptr<CyTensor_base> CyTensor_base::contiguous(){
            cytnx_error_msg(true,"[ERROR] fatal internal, cannot call on a un-initialize CyTensor_base%s","\n");
            boost::intrusive_ptr<CyTensor_base> out(new CyTensor_base());
            return out;
        }            
        void CyTensor_base::print_diagram(const bool &bond_info){
            cytnx_error_msg(true,"[ERROR] fatal internal, cannot call on a un-initialize CyTensor_base%s","\n");
        }

        Tensor CyTensor_base::get_block(const cytnx_uint64 &idx) const{
            cytnx_error_msg(true,"[ERROR] fatal internal, cannot call on a un-initialize CyTensor_base%s","\n");
            return Tensor();
        }
        Tensor CyTensor_base::get_block(const std::vector<cytnx_int64> &qnum) const{
            cytnx_error_msg(true,"[ERROR] fatal internal, cannot call on a un-initialize CyTensor_base%s","\n");
            return Tensor();
        }
        // return a share view of block, this only work for non-symm tensor.
        Tensor CyTensor_base::get_block_(const cytnx_uint64 &idx) const{
            cytnx_error_msg(true,"[ERROR] fatal internal, cannot call on a un-initialize CyTensor_base%s","\n");
            return Tensor();
        }
        Tensor CyTensor_base::get_block_(const std::vector<cytnx_int64> &qnum) const{
            cytnx_error_msg(true,"[ERROR] fatal internal, cannot call on a un-initialize CyTensor_base%s","\n");
            return Tensor();
        }

        std::vector<Tensor> CyTensor_base::get_blocks() const{
            cytnx_error_msg(true,"[ERROR] fatal internal, cannot call on a un-initialize CyTensor_base%s","\n");
            return std::vector<Tensor>();
        }
        std::vector<Tensor> CyTensor_base::get_blocks_() const{
            cytnx_error_msg(true,"[ERROR] fatal internal, cannot call on a un-initialize CyTensor_base%s","\n");
            return std::vector<Tensor>();
        }
        void CyTensor_base::put_block(const Tensor &in, const cytnx_uint64 &idx){
            cytnx_error_msg(true,"[ERROR] fatal internal, cannot call on a un-initialize CyTensor_base%s","\n");
        }
        void CyTensor_base::put_block_(const Tensor &in, const cytnx_uint64 &idx){
            cytnx_error_msg(true,"[ERROR] fatal internal, cannot call on a un-initialize CyTensor_base%s","\n");
        }
        void CyTensor_base::put_block(const Tensor &in, const std::vector<cytnx_int64> &qnum){
            cytnx_error_msg(true,"[ERROR] fatal internal, cannot call on a un-initialize CyTensor_base%s","\n");
        }
        void CyTensor_base::put_block_(const Tensor &in, const std::vector<cytnx_int64> &qnum){
            cytnx_error_msg(true,"[ERROR] fatal internal, cannot call on a un-initialize CyTensor_base%s","\n");
        }
        void CyTensor_base::set(const std::vector<Accessor> &accesssors, const Tensor &rhs){
            cytnx_error_msg(true,"[ERROR] fatal internal, cannot call on a un-initialize CyTensor_base%s","\n");
        }
        boost::intrusive_ptr<CyTensor_base> CyTensor_base::get(const std::vector<Accessor> &accesssors){
            cytnx_error_msg(true,"[ERROR] fatal internal, cannot call on a un-initialize CyTensor_base%s","\n");
            return nullptr;
        }
        
       
        void CyTensor_base::reshape_(const std::vector<cytnx_int64> &new_shape, const cytnx_uint64 &Rowrank){
            cytnx_error_msg(true,"[ERROR] fatal internal, cannot call on a un-initialize CyTensor_base%s","\n");

        }
        boost::intrusive_ptr<CyTensor_base> CyTensor_base::reshape(const std::vector<cytnx_int64> &new_shape, const cytnx_uint64 &Rowrank){
            cytnx_error_msg(true,"[ERROR] fatal internal, cannot call on a un-initialize CyTensor_base%s","\n");
            return nullptr;
        }
        void CyTensor_base::set_labels(const std::vector<cytnx_int64> &new_labels){
                cytnx_error_msg(new_labels.size()!=this->_labels.size(),"[ERROR][set_labels][CyTensor] %s\n","the len(new_labels) does not match the rank of the UniTenosr");
                //check duplicate:
                std::vector<cytnx_int64> tmp = vec_unique<cytnx_int64>(new_labels);
                cytnx_error_msg(tmp.size() != new_labels.size(), "[ERROR][set_labels][CyTensor] %s\n","the input labels cannot contain duplicated element(s).");
                this->_labels = new_labels;
        }
        boost::intrusive_ptr<CyTensor_base> CyTensor_base::to_dense(){
            cytnx_error_msg(true,"[ERROR] fatal internal, cannot call on a un-initialize CyTensor_base%s","\n");
            return nullptr;
        }
        void CyTensor_base::to_dense_(){
            cytnx_error_msg(true,"[ERROR] fatal internal, cannot call on a un-initialize CyTensor_base%s","\n");
        }
        void CyTensor_base::combineBonds(const std::vector<cytnx_int64> &indicators, const bool &permute_back, const bool &by_labels){
            cytnx_error_msg(true,"[ERROR] fatal internal, cannot call on a un-initialize CyTensor_base%s","\n");
        }
        boost::intrusive_ptr<CyTensor_base> CyTensor_base::contract(const boost::intrusive_ptr<CyTensor_base>&rhs){
            cytnx_error_msg(true,"[ERROR] fatal internal, cannot call on a un-initialize CyTensor_base%s","\n");
            return nullptr;
        }
        std::vector<Bond> CyTensor_base::getTotalQnums(const bool &physical){

            cytnx_error_msg(true,"[ERROR] fatal internal, cannot call on a un-initialize CyTensor_base%s","\n");
            return std::vector<Bond>();
        }

        //-----------------------------------------
        std::ostream& operator<<(std::ostream& os, const CyTensor &in){
            char* buffer = (char*)malloc(sizeof(char)*256);
            sprintf(buffer,"Tensor name: %s\n",in.name().c_str()); os << std::string(buffer);
            if(!in.is_tag()) 
                sprintf(buffer,"braket_form : %s\n", in.is_braket_form()?"True":"False"); os << std::string(buffer);
            
            if(in.is_blockform()){
                std::vector<Tensor> tmp = in.get_blocks();
                for(cytnx_uint64 i=0;i<tmp.size();i++)
                    os << tmp[i] << std::endl;
                
            }else{
                sprintf(buffer,"is_diag    : %s\n",in.is_diag()?"True":"False"); os << std::string(buffer);
                Tensor tmp = in.get_block();
                os << tmp << std::endl;     
            }         
            free(buffer);
            return os;
        
        }

        CyTensor Contract(const CyTensor &inL, const CyTensor &inR){
            return inL.contract(inR);
        }
        
}

