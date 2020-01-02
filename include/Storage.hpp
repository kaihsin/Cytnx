#ifndef _H_Storage_
#define _H_Storage_

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <initializer_list>
#include <typeinfo>
#include <vector>
#include <complex>

#ifdef UNI_OMP
    #include <omp.h>
#endif

#include "Type.hpp"
#include "Device.hpp"
#include "intrusive_ptr_base.hpp"
#include "cytnx_error.hpp"



namespace cytnx{

    ///@cond    
    class Storage_base : public intrusive_ptr_base<Storage_base> {
        public:
            void* Mem;
            //std::vector<unsigned int> shape;

            unsigned long long len; // default 0
            unsigned long long cap; // default 0  
            unsigned int dtype; // default 0, Void
            int device; // default -1, on cpu

            Storage_base(): cap(0),len(0), Mem(NULL),dtype(0), device(-1){};
            //Storage_base(const std::initializer_list<unsigned int> &init_shape);
            //Storage_base(const std::vector<unsigned int> &init_shape);    
            Storage_base(const unsigned long long &len_in,const int &device);

            Storage_base(Storage_base &Rhs);
            Storage_base& operator=(Storage_base &Rhs);
            boost::intrusive_ptr<Storage_base> astype(const unsigned int &dtype);

            //void Init(const std::initializer_list<unsigned int> &init_shape);
            std::string dtype_str() const ;
            std::string device_str() const;         
            const unsigned long long &capacity() const{
                return this->cap;
            }
            const unsigned long long &size() const{
                return this->len;
            }
            ~Storage_base();


            template<class T>
            T& at(const unsigned int &idx) const;
            
            template<class T>   
            T* data() const;
            void print();
            void print_info();
            /*
                This function is design to check the type mismatch. 
                Handy for developer to exclude the assign of double 
                C pointer into a non-DoubleStorage.
                
                For example:
                float *cptr = (float*)calloc(4,sizeof(float));

                intrusive_ptr<Storage> array(new DoubleStorage());
                array->_Init_byptr((void*)cptr,4); // This is fatal, since we alloc cptr as float, 
                                                   // but apon free, DoubleStorage will free 2x 
                                                   // of memory!!!!

                array->_Init_byptr_safe(cptr,4);   // This is design to avoid the above problem 
                                                   // by checking the type of input pointer with 
                                                   // the type of Storage before call _Init_byptr.
                                                   // [Note] this will intorduce overhead!!.

            */
            template<class T>
            void _Init_byptr_safe(T *rawptr, const unsigned long long &len_in){
                //check:
                if(this->dtype==Type.Float){
                        cytnx_error_msg(typeid(T) != typeid(cytnx_float),"%s","[ERROR _Init_byptr_safe type not match]");
                }else if(this->dtype==Type.Double){
                        cytnx_error_msg(typeid(T) != typeid(cytnx_double),"%s","[ERROR _Init_byptr_safe type not match]");
                }else if(this->dtype==Type.Uint64){
                        cytnx_error_msg(typeid(T) != typeid(cytnx_uint64),"%s","[ERROR _Init_byptr_safe type not match]");
                }else if(this->dtype==Type.Uint32){
                        cytnx_error_msg(typeid(T) != typeid(cytnx_uint32),"%s","[ERROR _Init_byptr_safe type not match]");
                }else if(this->dtype==Type.Int64){
                        cytnx_error_msg(typeid(T) != typeid(cytnx_int64),"%s","[ERROR _Init_byptr_safe type not match]");
                }else if(this->dtype==Type.Int32){
                        cytnx_error_msg(typeid(T) != typeid(cytnx_int32),"%s","[ERROR _Init_byptr_safe type not match]");
                }else if(this->dtype==Type.ComplexDouble){
                        cytnx_error_msg(typeid(T) != typeid(cytnx_complex128),"%s","[ERROR _Init_byptr_safe type not match]");
                }else if(this->dtype==Type.ComplexFloat){
                        cytnx_error_msg(typeid(T) != typeid(cytnx_complex64),"%s","[ERROR _Init_byptr_safe type not match]");
                }else if(this->dtype==Type.Int16){
                        cytnx_error_msg(typeid(T) != typeid(cytnx_int16),"%s","[ERROR _Init_byptr_safe type not match]");
                }else if(this->dtype==Type.Uint16){
                        cytnx_error_msg(typeid(T) != typeid(cytnx_uint16),"%s","[ERROR _Init_byptr_safe type not match]");
                }else if(this->dtype==Type.Bool){
                        cytnx_error_msg(typeid(T) != typeid(cytnx_bool),"%s","[ERROR _Init_byptr_safe type not match]");
                }else{
                    cytnx_error_msg(1,"[FATAL] ERROR%s","\n");
                }

                this->_Init_byptr((void*)rawptr,len_in);
            }
            
            
            

            void GetElem_byShape(boost::intrusive_ptr<Storage_base> &out, const std::vector<cytnx_uint64> &shape, const std::vector<cytnx_uint64> &mapper, const std::vector<cytnx_uint64> &len, const std::vector<std::vector<cytnx_uint64> > &locators);
            void SetElem_byShape(boost::intrusive_ptr<Storage_base> &in, const std::vector<cytnx_uint64> &shape, const std::vector<cytnx_uint64> &mapper, const std::vector<cytnx_uint64> &len, const std::vector<std::vector<cytnx_uint64> > &locators, const bool &is_scalar);

            // these is the one that do the work, and customize with Storage_base
            //virtual void Init(const std::vector<unsigned int> &init_shape);
            virtual void Init(const unsigned long long &len_in, const int &device=-1);
            virtual void _Init_byptr(void *rawptr, const unsigned long long &len_in, const int &device=-1, const bool &iscap=false, const unsigned long long &cap_in=0);

            // this function will return a new storage with the same type as the one 
            // that initiate this function. 
            virtual boost::intrusive_ptr<Storage_base> _create_new_sametype();

            // [future] this will move the memory to device / cpu
            virtual void to_(const int &device);
            virtual boost::intrusive_ptr<Storage_base> to(const int &device);

            virtual boost::intrusive_ptr<Storage_base> clone();

            // this will perform permute on the underlying memory. 
            virtual boost::intrusive_ptr<Storage_base> Move_memory(const std::vector<cytnx_uint64> &old_shape, const std::vector<cytnx_uint64> &mapper, const std::vector<cytnx_uint64> &invmapper);
            virtual void Move_memory_(const std::vector<cytnx_uint64> &old_shape, const std::vector<cytnx_uint64> &mapper, const std::vector<cytnx_uint64> &invmapper); 
            virtual void PrintElem_byShape(std::ostream& os, const std::vector<cytnx_uint64> &shape, const std::vector<cytnx_uint64> &mapper={});        
            virtual void print_elems();

            //generators:
            virtual void fill(const cytnx_complex128 &val);
            virtual void fill(const cytnx_complex64  &val);
            virtual void fill(const cytnx_double     &val);
            virtual void fill(const cytnx_float      &val);
            virtual void fill(const cytnx_int64      &val);
            virtual void fill(const cytnx_uint64     &val);
            virtual void fill(const cytnx_int32      &val);
            virtual void fill(const cytnx_uint32     &val);
            virtual void fill(const cytnx_int16      &val);
            virtual void fill(const cytnx_uint16     &val);
            virtual void fill(const cytnx_bool       &val);
            virtual void set_zeros();
            virtual void resize(const cytnx_uint64 &newsize);
            
            virtual void append(const cytnx_complex128 &val);
            virtual void append(const cytnx_complex64  &val);
            virtual void append(const cytnx_double     &val);
            virtual void append(const cytnx_float      &val);
            virtual void append(const cytnx_int64      &val);
            virtual void append(const cytnx_uint64     &val);
            virtual void append(const cytnx_int32      &val);
            virtual void append(const cytnx_uint32     &val);
            virtual void append(const cytnx_int16      &val);
            virtual void append(const cytnx_uint16     &val);
            virtual void append(const cytnx_bool       &val);

    };        
    ///@endcond

    ///@cond
    class FloatStorage : public Storage_base{
        public:
            FloatStorage(){this->dtype=Type.Float;};
            void Init(const unsigned long long &len_in, const int &device=-1);
            void _Init_byptr(void *rawptr, const unsigned long long &len_in, const int &device=-1, const bool &iscap=false, const unsigned long long &cap_in=0);
            boost::intrusive_ptr<Storage_base> _create_new_sametype();
            boost::intrusive_ptr<Storage_base> clone();
            boost::intrusive_ptr<Storage_base> Move_memory(const std::vector<cytnx_uint64> &old_shape, const std::vector<cytnx_uint64> &mapper, const std::vector<cytnx_uint64> &invmapper);
            void Move_memory_(const std::vector<cytnx_uint64> &old_shape, const std::vector<cytnx_uint64> &mapper, const std::vector<cytnx_uint64> &invmapper); 
            void to_(const int &device);
            boost::intrusive_ptr<Storage_base> to(const int &device);
            void PrintElem_byShape(std::ostream& os, const std::vector<cytnx_uint64> &shape, const std::vector<cytnx_uint64> &mapper={});        
            void print_elems();

            //generators:
            void fill(const cytnx_complex128 &val);
            void fill(const cytnx_complex64  &val);
            void fill(const cytnx_double     &val);
            void fill(const cytnx_float      &val);
            void fill(const cytnx_int64      &val);
            void fill(const cytnx_uint64     &val);
            void fill(const cytnx_int32      &val);
            void fill(const cytnx_uint32     &val);
            void fill(const cytnx_int16      &val);
            void fill(const cytnx_uint16     &val);
            void fill(const cytnx_bool       &val);
            void set_zeros();
            void resize(const cytnx_uint64 &newsize);

            void append(const cytnx_complex128 &val);
            void append(const cytnx_complex64  &val);
            void append(const cytnx_double     &val);
            void append(const cytnx_float      &val);
            void append(const cytnx_int64      &val);
            void append(const cytnx_uint64     &val);
            void append(const cytnx_int32      &val);
            void append(const cytnx_uint32     &val);
            void append(const cytnx_int16      &val);
            void append(const cytnx_uint16     &val);
            void append(const cytnx_bool       &val);

    };          
    ///@endcond

    ///@cond
    class DoubleStorage: public Storage_base{
        public:
            DoubleStorage(){this->dtype=Type.Double;};
            void Init(const unsigned long long &len_in,const int &device=-1);
            void _Init_byptr(void *rawptr, const unsigned long long &len_in, const int &device=-1, const bool &iscap=false, const unsigned long long &cap_in=0);
            boost::intrusive_ptr<Storage_base> _create_new_sametype();
            boost::intrusive_ptr<Storage_base> clone();
            boost::intrusive_ptr<Storage_base> Move_memory(const std::vector<cytnx_uint64> &old_shape, const std::vector<cytnx_uint64> &mapper, const std::vector<cytnx_uint64> &invmapper);
            void Move_memory_(const std::vector<cytnx_uint64> &old_shape, const std::vector<cytnx_uint64> &mapper, const std::vector<cytnx_uint64> &invmapper); 
            void to_(const int &device);
            boost::intrusive_ptr<Storage_base> to(const int &device);
            void PrintElem_byShape(std::ostream& os, const std::vector<cytnx_uint64> &shape, const std::vector<cytnx_uint64> &mapper={});        
            void print_elems();

            //generators:
            void fill(const cytnx_complex128 &val);
            void fill(const cytnx_complex64  &val);
            void fill(const cytnx_double     &val);
            void fill(const cytnx_float      &val);
            void fill(const cytnx_int64      &val);
            void fill(const cytnx_uint64     &val);
            void fill(const cytnx_int32      &val);
            void fill(const cytnx_uint32     &val);
            void fill(const cytnx_int16      &val);
            void fill(const cytnx_uint16     &val);
            void fill(const cytnx_bool       &val);
            void set_zeros();
            void resize(const cytnx_uint64 &newsize);

            void append(const cytnx_complex128 &val);
            void append(const cytnx_complex64  &val);
            void append(const cytnx_double     &val);
            void append(const cytnx_float      &val);
            void append(const cytnx_int64      &val);
            void append(const cytnx_uint64     &val);
            void append(const cytnx_int32      &val);
            void append(const cytnx_uint32     &val);
            void append(const cytnx_int16      &val);
            void append(const cytnx_uint16     &val);
            void append(const cytnx_bool       &val);

    };
    ///@endcond

    ///@cond
    class ComplexDoubleStorage: public Storage_base{
        public:
            ComplexDoubleStorage(){this->dtype=Type.ComplexDouble;};
            void Init(const unsigned long long &len_in, const int &device=-1);
            void _Init_byptr(void *rawptr, const unsigned long long &len_in, const int &device=-1, const bool &iscap=false, const unsigned long long &cap_in=0);
            boost::intrusive_ptr<Storage_base> _create_new_sametype();
            boost::intrusive_ptr<Storage_base> clone();
            boost::intrusive_ptr<Storage_base> Move_memory(const std::vector<cytnx_uint64> &old_shape, const std::vector<cytnx_uint64> &mapper, const std::vector<cytnx_uint64> &invmapper);
            void Move_memory_(const std::vector<cytnx_uint64> &old_shape, const std::vector<cytnx_uint64> &mapper, const std::vector<cytnx_uint64> &invmapper); 
            void to_(const int &device);
            boost::intrusive_ptr<Storage_base> to(const int &device);
            void PrintElem_byShape(std::ostream& os, const std::vector<cytnx_uint64> &shape, const std::vector<cytnx_uint64> &mapper={});        
            void print_elems();

            //generators:
            void fill(const cytnx_complex128 &val);
            void fill(const cytnx_complex64  &val);
            void fill(const cytnx_double     &val);
            void fill(const cytnx_float      &val);
            void fill(const cytnx_int64      &val);
            void fill(const cytnx_uint64     &val);
            void fill(const cytnx_int32      &val);
            void fill(const cytnx_uint32     &val);
            void fill(const cytnx_int16      &val);
            void fill(const cytnx_uint16     &val);
            void fill(const cytnx_bool       &val);
            void set_zeros();
            void resize(const cytnx_uint64 &newsize);


            void append(const cytnx_complex128 &val);
            void append(const cytnx_complex64  &val);
            void append(const cytnx_double     &val);
            void append(const cytnx_float      &val);
            void append(const cytnx_int64      &val);
            void append(const cytnx_uint64     &val);
            void append(const cytnx_int32      &val);
            void append(const cytnx_uint32     &val);
            void append(const cytnx_int16      &val);
            void append(const cytnx_uint16     &val);
            void append(const cytnx_bool       &val);

    };
    ///@endcond

    ///@cond
    class ComplexFloatStorage: public Storage_base{
        public:
            ComplexFloatStorage(){this->dtype=Type.ComplexFloat;};
            void Init(const unsigned long long &len_in,const int &device=-1);
            void _Init_byptr(void *rawptr, const unsigned long long &len_in, const int &device=-1, const bool &iscap=false, const unsigned long long &cap_in=0);
            boost::intrusive_ptr<Storage_base> _create_new_sametype();
            boost::intrusive_ptr<Storage_base> clone();
            boost::intrusive_ptr<Storage_base> Move_memory(const std::vector<cytnx_uint64> &old_shape, const std::vector<cytnx_uint64> &mapper, const std::vector<cytnx_uint64> &invmapper);
            void Move_memory_(const std::vector<cytnx_uint64> &old_shape, const std::vector<cytnx_uint64> &mapper, const std::vector<cytnx_uint64> &invmapper); 
            void to_(const int &device);
            boost::intrusive_ptr<Storage_base> to(const int &device);
            void PrintElem_byShape(std::ostream& os, const std::vector<cytnx_uint64> &shape, const std::vector<cytnx_uint64> &mapper={});        
            void print_elems();

            //generators:
            void fill(const cytnx_complex128 &val);
            void fill(const cytnx_complex64  &val);
            void fill(const cytnx_double     &val);
            void fill(const cytnx_float      &val);
            void fill(const cytnx_int64      &val);
            void fill(const cytnx_uint64     &val);
            void fill(const cytnx_int32      &val);
            void fill(const cytnx_uint32     &val);
            void fill(const cytnx_int16      &val);
            void fill(const cytnx_uint16     &val);
            void fill(const cytnx_bool       &val);
            void set_zeros();
            void resize(const cytnx_uint64 &newsize);

            void append(const cytnx_complex128 &val);
            void append(const cytnx_complex64  &val);
            void append(const cytnx_double     &val);
            void append(const cytnx_float      &val);
            void append(const cytnx_int64      &val);
            void append(const cytnx_uint64     &val);
            void append(const cytnx_int32      &val);
            void append(const cytnx_uint32     &val);
            void append(const cytnx_int16      &val);
            void append(const cytnx_uint16     &val);
            void append(const cytnx_bool       &val);

    };
    ///@endcond

    ///@cond
    class Int64Storage : public Storage_base{
        public:
            Int64Storage(){this->dtype=Type.Int64;};
            void Init(const unsigned long long &len_in, const int &device=-1);
            void _Init_byptr(void *rawptr, const unsigned long long &len_in, const int &device=-1, const bool &iscap=false, const unsigned long long &cap_in=0);
            boost::intrusive_ptr<Storage_base> _create_new_sametype();
            boost::intrusive_ptr<Storage_base> clone();
            boost::intrusive_ptr<Storage_base> Move_memory(const std::vector<cytnx_uint64> &old_shape, const std::vector<cytnx_uint64> &mapper, const std::vector<cytnx_uint64> &invmapper);
            void Move_memory_(const std::vector<cytnx_uint64> &old_shape, const std::vector<cytnx_uint64> &mapper, const std::vector<cytnx_uint64> &invmapper); 
            void to_(const int &device);
            boost::intrusive_ptr<Storage_base> to(const int &device);
            void PrintElem_byShape(std::ostream& os, const std::vector<cytnx_uint64> &shape, const std::vector<cytnx_uint64> &mapper={});        
            void print_elems();

            //generators:
            void fill(const cytnx_complex128 &val);
            void fill(const cytnx_complex64  &val);
            void fill(const cytnx_double     &val);
            void fill(const cytnx_float      &val);
            void fill(const cytnx_int64      &val);
            void fill(const cytnx_uint64     &val);
            void fill(const cytnx_int32      &val);
            void fill(const cytnx_uint32     &val);
            void fill(const cytnx_int16      &val);
            void fill(const cytnx_uint16     &val);
            void fill(const cytnx_bool       &val);
            void set_zeros();
            void resize(const cytnx_uint64 &newsize);

            void append(const cytnx_complex128 &val);
            void append(const cytnx_complex64  &val);
            void append(const cytnx_double     &val);
            void append(const cytnx_float      &val);
            void append(const cytnx_int64      &val);
            void append(const cytnx_uint64     &val);
            void append(const cytnx_int32      &val);
            void append(const cytnx_uint32     &val);
            void append(const cytnx_int16      &val);
            void append(const cytnx_uint16     &val);
            void append(const cytnx_bool       &val);

    };          
    ///@endcond

    ///@cond
    class Uint64Storage : public Storage_base{
        public:
            Uint64Storage(){this->dtype=Type.Uint64;};
            void Init(const unsigned long long &len_in, const int &device=-1);
            void _Init_byptr(void *rawptr, const unsigned long long &len_in, const int &device=-1, const bool &iscap=false, const unsigned long long &cap_in=0);
            boost::intrusive_ptr<Storage_base> _create_new_sametype();
            boost::intrusive_ptr<Storage_base> clone();
            boost::intrusive_ptr<Storage_base> Move_memory(const std::vector<cytnx_uint64> &old_shape, const std::vector<cytnx_uint64> &mapper, const std::vector<cytnx_uint64> &invmapper);
            void Move_memory_(const std::vector<cytnx_uint64> &old_shape, const std::vector<cytnx_uint64> &mapper, const std::vector<cytnx_uint64> &invmapper); 
            void to_(const int &device);
            boost::intrusive_ptr<Storage_base> to(const int &device);
            void PrintElem_byShape(std::ostream& os, const std::vector<cytnx_uint64> &shape, const std::vector<cytnx_uint64> &mapper={});        
            void print_elems();


            //generators:
            void fill(const cytnx_complex128 &val);
            void fill(const cytnx_complex64  &val);
            void fill(const cytnx_double     &val);
            void fill(const cytnx_float      &val);
            void fill(const cytnx_int64      &val);
            void fill(const cytnx_uint64     &val);
            void fill(const cytnx_int32      &val);
            void fill(const cytnx_uint32     &val);
            void fill(const cytnx_int16      &val);
            void fill(const cytnx_uint16     &val);
            void fill(const cytnx_bool       &val);
            void set_zeros();
            void resize(const cytnx_uint64 &newsize);

            void append(const cytnx_complex128 &val);
            void append(const cytnx_complex64  &val);
            void append(const cytnx_double     &val);
            void append(const cytnx_float      &val);
            void append(const cytnx_int64      &val);
            void append(const cytnx_uint64     &val);
            void append(const cytnx_int32      &val);
            void append(const cytnx_uint32     &val);
            void append(const cytnx_int16      &val);
            void append(const cytnx_uint16     &val);
            void append(const cytnx_bool       &val);
    };          
    ///@endcond
    ///@cond
    class Int32Storage : public Storage_base{
        public:
            Int32Storage(){this->dtype=Type.Int32;};
            void Init(const unsigned long long &len_in, const int &device=-1);
            void _Init_byptr(void *rawptr, const unsigned long long &len_in, const int &device=-1, const bool &iscap=false, const unsigned long long &cap_in=0);
            boost::intrusive_ptr<Storage_base> _create_new_sametype();
            boost::intrusive_ptr<Storage_base> clone();
            boost::intrusive_ptr<Storage_base> Move_memory(const std::vector<cytnx_uint64> &old_shape, const std::vector<cytnx_uint64> &mapper, const std::vector<cytnx_uint64> &invmapper);
            void Move_memory_(const std::vector<cytnx_uint64> &old_shape, const std::vector<cytnx_uint64> &mapper, const std::vector<cytnx_uint64> &invmapper); 
            void to_(const int &device);
            boost::intrusive_ptr<Storage_base> to(const int &device);
            void PrintElem_byShape(std::ostream& os, const std::vector<cytnx_uint64> &shape, const std::vector<cytnx_uint64> &mapper={});        
            void print_elems();

            //generators:
            void fill(const cytnx_complex128 &val);
            void fill(const cytnx_complex64  &val);
            void fill(const cytnx_double     &val);
            void fill(const cytnx_float      &val);
            void fill(const cytnx_int64      &val);
            void fill(const cytnx_uint64     &val);
            void fill(const cytnx_int32      &val);
            void fill(const cytnx_uint32     &val);
            void fill(const cytnx_int16      &val);
            void fill(const cytnx_uint16     &val);
            void fill(const cytnx_bool       &val);
            void set_zeros();
            void resize(const cytnx_uint64 &newsize);
            void append(const cytnx_complex128 &val);
            void append(const cytnx_complex64  &val);
            void append(const cytnx_double     &val);
            void append(const cytnx_float      &val);
            void append(const cytnx_int64      &val);
            void append(const cytnx_uint64     &val);
            void append(const cytnx_int32      &val);
            void append(const cytnx_uint32     &val);
            void append(const cytnx_int16      &val);
            void append(const cytnx_uint16     &val);
            void append(const cytnx_bool       &val);
    };          
    ///@endcond

    ///@cond
    class Uint32Storage : public Storage_base{
        public:
            Uint32Storage(){this->dtype=Type.Uint32;};
            void Init(const unsigned long long &len_in, const int &device=-1);
            void _Init_byptr(void *rawptr, const unsigned long long &len_in, const int &device=-1, const bool &iscap=false, const unsigned long long &cap_in=0);
            boost::intrusive_ptr<Storage_base> _create_new_sametype();
            boost::intrusive_ptr<Storage_base> clone();
            boost::intrusive_ptr<Storage_base> Move_memory(const std::vector<cytnx_uint64> &old_shape, const std::vector<cytnx_uint64> &mapper, const std::vector<cytnx_uint64> &invmapper);
            void Move_memory_(const std::vector<cytnx_uint64> &old_shape, const std::vector<cytnx_uint64> &mapper, const std::vector<cytnx_uint64> &invmapper); 
            void to_(const int &device);
            boost::intrusive_ptr<Storage_base> to(const int &device);
            void PrintElem_byShape(std::ostream& os, const std::vector<cytnx_uint64> &shape, const std::vector<cytnx_uint64> &mapper={});        
            void print_elems();

            //generators:
            void fill(const cytnx_complex128 &val);
            void fill(const cytnx_complex64  &val);
            void fill(const cytnx_double     &val);
            void fill(const cytnx_float      &val);
            void fill(const cytnx_int64      &val);
            void fill(const cytnx_uint64     &val);
            void fill(const cytnx_int32      &val);
            void fill(const cytnx_uint32     &val);
            void fill(const cytnx_int16      &val);
            void fill(const cytnx_uint16     &val);
            void fill(const cytnx_bool       &val);
            void set_zeros();
            void resize(const cytnx_uint64 &newsize);
            void append(const cytnx_complex128 &val);
            void append(const cytnx_complex64  &val);
            void append(const cytnx_double     &val);
            void append(const cytnx_float      &val);
            void append(const cytnx_int64      &val);
            void append(const cytnx_uint64     &val);
            void append(const cytnx_int32      &val);
            void append(const cytnx_uint32     &val);
            void append(const cytnx_int16      &val);
            void append(const cytnx_uint16     &val);
            void append(const cytnx_bool       &val);
    };          
    ///@endcond

    ///@cond
    class Uint16Storage : public Storage_base{
        public:
            Uint16Storage(){this->dtype=Type.Uint16;};
            void Init(const unsigned long long &len_in, const int &device=-1);
            void _Init_byptr(void *rawptr, const unsigned long long &len_in, const int &device=-1, const bool &iscap=false, const unsigned long long &cap_in=0);
            boost::intrusive_ptr<Storage_base> _create_new_sametype();
            boost::intrusive_ptr<Storage_base> clone();
            boost::intrusive_ptr<Storage_base> Move_memory(const std::vector<cytnx_uint64> &old_shape, const std::vector<cytnx_uint64> &mapper, const std::vector<cytnx_uint64> &invmapper);
            void Move_memory_(const std::vector<cytnx_uint64> &old_shape, const std::vector<cytnx_uint64> &mapper, const std::vector<cytnx_uint64> &invmapper); 
            void to_(const int &device);
            boost::intrusive_ptr<Storage_base> to(const int &device);
            void PrintElem_byShape(std::ostream& os, const std::vector<cytnx_uint64> &shape, const std::vector<cytnx_uint64> &mapper={});        
            void print_elems();

            //generators:
            void fill(const cytnx_complex128 &val);
            void fill(const cytnx_complex64  &val);
            void fill(const cytnx_double     &val);
            void fill(const cytnx_float      &val);
            void fill(const cytnx_int64      &val);
            void fill(const cytnx_uint64     &val);
            void fill(const cytnx_int32      &val);
            void fill(const cytnx_uint32     &val);
            void fill(const cytnx_int16      &val);
            void fill(const cytnx_uint16     &val);
            void fill(const cytnx_bool       &val);
            void set_zeros();
            void resize(const cytnx_uint64 &newsize);
            void append(const cytnx_complex128 &val);
            void append(const cytnx_complex64  &val);
            void append(const cytnx_double     &val);
            void append(const cytnx_float      &val);
            void append(const cytnx_int64      &val);
            void append(const cytnx_uint64     &val);
            void append(const cytnx_int32      &val);
            void append(const cytnx_uint32     &val);
            void append(const cytnx_int16      &val);
            void append(const cytnx_uint16     &val);
            void append(const cytnx_bool       &val);
    };          
    ///@endcond

    ///@cond
    class Int16Storage : public Storage_base{
        public:
            Int16Storage(){this->dtype=Type.Int16;};
            void Init(const unsigned long long &len_in, const int &device=-1);
            void _Init_byptr(void *rawptr, const unsigned long long &len_in, const int &device=-1, const bool &iscap=false, const unsigned long long &cap_in=0);
            boost::intrusive_ptr<Storage_base> _create_new_sametype();
            boost::intrusive_ptr<Storage_base> clone();
            boost::intrusive_ptr<Storage_base> Move_memory(const std::vector<cytnx_uint64> &old_shape, const std::vector<cytnx_uint64> &mapper, const std::vector<cytnx_uint64> &invmapper);
            void Move_memory_(const std::vector<cytnx_uint64> &old_shape, const std::vector<cytnx_uint64> &mapper, const std::vector<cytnx_uint64> &invmapper); 
            void to_(const int &device);
            boost::intrusive_ptr<Storage_base> to(const int &device);
            void PrintElem_byShape(std::ostream& os, const std::vector<cytnx_uint64> &shape, const std::vector<cytnx_uint64> &mapper={});        
            void print_elems();

            //generators:
            void fill(const cytnx_complex128 &val);
            void fill(const cytnx_complex64  &val);
            void fill(const cytnx_double     &val);
            void fill(const cytnx_float      &val);
            void fill(const cytnx_int64      &val);
            void fill(const cytnx_uint64     &val);
            void fill(const cytnx_int32      &val);
            void fill(const cytnx_uint32     &val);
            void fill(const cytnx_int16      &val);
            void fill(const cytnx_uint16     &val);
            void fill(const cytnx_bool       &val);
            void set_zeros();
            void resize(const cytnx_uint64 &newsize);
            void append(const cytnx_complex128 &val);
            void append(const cytnx_complex64  &val);
            void append(const cytnx_double     &val);
            void append(const cytnx_float      &val);
            void append(const cytnx_int64      &val);
            void append(const cytnx_uint64     &val);
            void append(const cytnx_int32      &val);
            void append(const cytnx_uint32     &val);
            void append(const cytnx_int16      &val);
            void append(const cytnx_uint16     &val);
            void append(const cytnx_bool       &val);
    };          
    ///@endcond

    ///@cond
    class BoolStorage : public Storage_base{
        public:
            BoolStorage(){this->dtype=Type.Bool;};
            void Init(const unsigned long long &len_in, const int &device=-1);
            void _Init_byptr(void *rawptr, const unsigned long long &len_in, const int &device=-1, const bool &iscap=false, const unsigned long long &cap_in=0);
            boost::intrusive_ptr<Storage_base> _create_new_sametype();
            boost::intrusive_ptr<Storage_base> clone();
            boost::intrusive_ptr<Storage_base> Move_memory(const std::vector<cytnx_uint64> &old_shape, const std::vector<cytnx_uint64> &mapper, const std::vector<cytnx_uint64> &invmapper);
            void Move_memory_(const std::vector<cytnx_uint64> &old_shape, const std::vector<cytnx_uint64> &mapper, const std::vector<cytnx_uint64> &invmapper); 
            void to_(const int &device);
            boost::intrusive_ptr<Storage_base> to(const int &device);
            void PrintElem_byShape(std::ostream& os, const std::vector<cytnx_uint64> &shape, const std::vector<cytnx_uint64> &mapper={});        
            void print_elems();

            //generators:
            void fill(const cytnx_complex128 &val);
            void fill(const cytnx_complex64  &val);
            void fill(const cytnx_double     &val);
            void fill(const cytnx_float      &val);
            void fill(const cytnx_int64      &val);
            void fill(const cytnx_uint64     &val);
            void fill(const cytnx_int32      &val);
            void fill(const cytnx_uint32     &val);
            void fill(const cytnx_int16      &val);
            void fill(const cytnx_uint16     &val);
            void fill(const cytnx_bool       &val);
            void set_zeros();
            void resize(const cytnx_uint64 &newsize);
            void append(const cytnx_complex128 &val);
            void append(const cytnx_complex64  &val);
            void append(const cytnx_double     &val);
            void append(const cytnx_float      &val);
            void append(const cytnx_int64      &val);
            void append(const cytnx_uint64     &val);
            void append(const cytnx_int32      &val);
            void append(const cytnx_uint32     &val);
            void append(const cytnx_int16      &val);
            void append(const cytnx_uint16     &val);
            void append(const cytnx_bool       &val);
    };          
    ///@endcond


    ///@cond
    typedef boost::intrusive_ptr<Storage_base> (*pStorage_init)();
    ///@endcond

    ///@cond
    class Storage_init_interface: public Type_class{
        public:
            std::vector<pStorage_init> USIInit;
            Storage_init_interface();
    };
    ///@endcond;

    /// an memeory storage with multi-type/multi-device support 
    class Storage{  
        private:
            //Interface:
            Storage_init_interface __SII;

        public:
            ///@cond
            boost::intrusive_ptr<Storage_base> _impl;
            ///@endcond

            /**
            @brief initialize a Storage
            @param size the number of elements for the Storage
            @param dtype the dtype of the Storage instance. This can be any of type defined in cytnx::Type  
            @param device the device of the Storage instance. This can be cytnx::Device.cpu or cytnx::Device.cuda+<gpuid>
            
            ## Example:
            ### c++ API:
            \include example/Storage/Init.cpp
            #### output>
            \verbinclude example/Storage/Init.cpp.out
            ### python API:
            \include example/Storage/Init.py               
            #### output>
            \verbinclude example/Storage/Init.py.out
            */            
            void Init(const unsigned long long &size,const unsigned int &dtype=Type.Double, int device=-1){
                cytnx_error_msg(dtype>=N_Type,"%s","[ERROR] invalid argument: dtype");
                this->_impl = __SII.USIInit[dtype]();
                this->_impl->Init(size,device);
            }
            Storage(const unsigned long long &size, const unsigned int &dtype=Type.Double, int device=-1): _impl(new Storage_base()){
                Init(size,dtype,device);
            }
            Storage(): _impl(new Storage_base()){};
            ///@cond
            Storage(boost::intrusive_ptr<Storage_base> in_impl){
                this->_impl = in_impl;
            }
            Storage(const Storage &rhs){
                this->_impl = rhs._impl;
            }
            Storage& operator=(const Storage &rhs){
                this->_impl = rhs._impl;
                return *this;
            }
            ///@endcond

            void _Save(std::fstream &f);
            void _Load(std::fstream &f);

            /**
            @brief Save current Storage to file
            @param fname file name

            description:
                save the Storage to file with file path specify with input param 'fname' with postfix ".cyst"

            */            
            void Save(const std::string &fname);
            void Save(const char* fname);
            /**
            @brief Load current Storage from file
            @param fname file name

            description:
                load the Storage from file with file path specify with input param 'fname'.

            */            
            void Load(const std::string &fname);
            void Load(const char* fname);

            /**
            @brief cast the type of current Storage
            @param new_type the new type of the Storage instance. This can be any of type defined in cytnx::Type  

            description:
                1. if the new_type is the same as the dtype of current Storage, then return self;
                   otherwise, return a new instance that has the same content as current Storage with dtype=new_type
                2. the return Stoarge will be on the same device as the current Storage.

            ## Example:
            ### c++ API:
            \include example/Storage/astype.cpp
            #### output>
            \verbinclude example/Storage/astype.cpp.out
            ### python API:
            \include example/Storage/astype.py               
            #### output>
            \verbinclude example/Storage/astype.py.out
            */            
            Storage astype(const unsigned int &new_type) const{
                return this->_impl->astype(new_type);
            }

            /**
            @brief the dtype-id of current Storage
            @return [cytnx_uint64] the dtype-id.

            */            
            const unsigned int &dtype() const{
                return this->_impl->dtype;
            }

            /**
            @brief the dtype (std::string) of current Storage
            @return [std::string] dtype name

            */            
            const std::string dtype_str() const{
                std::string out = this->_impl->dtype_str();
                return out;
            }
            /**
            @brief the device-id of current Storage
            @return [cytnx_int64] the device-id.

            */            
            const int &device() const{
                return this->_impl->device;
            }

            /**
            @brief the device (std::string) of current Storage
            @return [std::string] device name

            */            
            const std::string device_str() const{
                std::string out = this->_impl->device_str();
                return out;
            }            

            /**
            @brief append a value
            @param val the value to append. it can be any type defined in cytnx::Type
                
            [Note] 
                1. cannot append a complex value into a real Storage.     
            */            
            template<class T> 
            void append(const T &val){
                return this->_impl->append(val);
            }

            ///@cond
            template<class T> // this is c++ only
            T& at(const unsigned int &idx) const{
                cytnx_error_msg(idx>=this->size(),"[ERROR] trying to access elements out of bound.%s","\n");
                return this->_impl->at<T>(idx);
            }
            template<class T> // this is c++ only
            T* data() const{
                return this->_impl->data<T>();
            }
            ///@endcond

            /**
            @brief resize the current Storage. 
            @param newsize. 
                
            */
            void resize(const cytnx_uint64 &newsize){
                this->_impl->resize(newsize);
            }


            /**
            @brief move the current Storage to different deivce. 
            @param device the device-id. It can be any device defined in \link cytnx::Device cytnx::Device \endlink
                
            see also \link cytnx::Storage::to Storage.to() \endlink
            */
            void to_(const int &device){
                this->_impl->to_(device);
            }

            /**
            @brief move a new Storage with same content as current Storage on different deivce. 
            @param device the device-id. It can be any device defined in \link cytnx::Device cytnx::Device \endlink
                
            [Note]
                if the \param device is the same as the current Storage's device, return self. 
            
            see also \link cytnx::Storage::to_ Storage.to_() \endlink
            */
            Storage to(const int &device){
                return Storage(this->_impl->to(device));
            }
            /**
            @brief return a copy of current storage.
            @return 
                [Storage]
            
            ## Example:
            ### c++ API:
            \include example/Storage/clone.cpp
            #### output>
            \verbinclude example/Storage/clone.cpp.out
            ### python API:
            \include example/Storage/clone.py               
            #### output>
            \verbinclude example/Storage/clone.py.out
            */
            Storage clone() const{
                return Storage(this->_impl->clone());
            }

            /**
            @brief the size ( no. of elements ) in the Storage
            @return [cytnx_uint64]  
                
            */            
            const unsigned long long &size() const{
                return this->_impl->len;
            }
            
            /**
            @brief the capacity ( no. of real elements in memory) in the Storage
            @return [cytnx_uint64]  
                
            */            
            const unsigned long long &capacity() const{
                return this->_impl->cap;
            }

            /**
            @brief print the info of the Storage, including the device, dtype and size.
  
            */
            void print_info(){
                this->_impl->print_info();
            }
            /// @cond 
            // this is a redundant function
            void print(){
                this->_impl->print();
            }
            /// @endcond

            /**
            @brief set all the elements to zero. 
           
            [Note] although it is also possible to use Storage.fill(0) to set all the elements to zero, 
                   using set_zeros will have significant faster performance. 
                
            */            
            void set_zeros(){
                this->_impl->set_zeros();
            }

            /**
            @brief compare two Storage
            @param Storage another Storage to compare to

            [Note] the == operator will compare the content between two storages. use cytnx::is() for checking two variables share the same instance. 
            
            ## Example:
            ### c++ API:
            \include example/Storage/eq.cpp
            #### output>
            \verbinclude example/Storage/eq.cpp.out
            ### python API:
            \include example/Storage/eq.py               
            #### output>
            \verbinclude example/Storage/eq.py.out
            */
            bool operator==(const Storage &rhs);
            bool operator!=(const Storage &rhs);

            /**
            @brief set all the elements to the assigned value val
            @param val the value to set on all the elements. it can be any type defined in cytnx::Type
                
            [Note] 
                1. cannot assign a complex value into a real Storage.     
            */            
            template<class T>
            void fill(const T& val){
                this->_impl->fill(val);
            }


            /*
                @brief renew/create a Storage using c++ vector. 
                @param vin the C++ vector with supported types. 
                    
                [Note] 
                    This function is C++ only
            */
            template<class T>
            void from_vector(const std::vector<T> &vin,const int device=-1){
                //auto dispatch:
                //check:
                if(typeid(T) == typeid(cytnx_float)){
                   this->_impl = __SII.USIInit[Type.Float]();
                }else if(typeid(T) == typeid(cytnx_double)){
                   this->_impl = __SII.USIInit[Type.Double]();
                }else if(typeid(T) == typeid(cytnx_uint64)){
                   this->_impl = __SII.USIInit[Type.Uint64]();
                }else if(typeid(T) == typeid(cytnx_uint32)){
                   this->_impl = __SII.USIInit[Type.Uint32]();
                }else if(typeid(T) == typeid(cytnx_int64)){
                   this->_impl = __SII.USIInit[Type.Int64]();
                }else if(typeid(T) == typeid(cytnx_int32)){
                   this->_impl = __SII.USIInit[Type.Int32]();
                }else if(typeid(T) == typeid(cytnx_complex128)){
                   this->_impl = __SII.USIInit[Type.ComplexDouble]();
                }else if(typeid(T) == typeid(cytnx_complex64)){
                   this->_impl = __SII.USIInit[Type.ComplexFloat]();
                }else if(typeid(T) == typeid(cytnx_int16)){
                   this->_impl = __SII.USIInit[Type.Int16]();
                }else if(typeid(T) == typeid(cytnx_uint16)){
                   this->_impl = __SII.USIInit[Type.Uint16]();
                }else if(typeid(T) == typeid(cytnx_bool)){
                   this->_impl = __SII.USIInit[Type.Bool]();
                }else{
                    cytnx_error_msg(1,"[FATAL] ERROR unsupport type%s","\n");
                }
                this->_impl->Init(vin.size(),device);
                memcpy(this->_impl->Mem,&vin[0],sizeof(T)*vin.size());
            }


    };

    ///@cond
    std::ostream& operator<<(std::ostream& os, Storage &in);
    ///@endcond

}

#endif
