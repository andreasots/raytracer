#ifndef RAYTRACER_ALLOCATOR_H
#define RAYTRACER_ALLOCATOR_H
#include <memory>

namespace Raytracer
{
    template <class T, std::size_t align> class allocator: public std::allocator<T>
    {
    	public:
    	typedef T value_type;
    	typedef T* pointer;
    	typedef T& reference;
    	typedef const T* const_pointer;
    	typedef const T& const_reference;
    	typedef std::size_t size_type;
    	typedef std::ptrdiff_t difference_type;
        template<typename U, std::size_t U_align = align> struct rebind
        {
            typedef allocator<U, U_align> other;
        };

        allocator() throw()
        {
        }

        allocator(const allocator&) throw()
        {
        }

        template<class U, std::size_t U_align> allocator(const allocator<U, U_align>&) throw()
        {
        }

        ~allocator() throw()
        {
        }

        pointer address(reference x) const
        {
            return &x;
        }

        const_pointer address(const_reference x) const
        {
            return &x;
        }

        pointer allocate(size_type n, const void *hint = 0)
        {
            pointer ret;
            if(posix_memalign(reinterpret_cast<void**>(&ret), align, n*sizeof(value_type)))
                std::__throw_bad_alloc();
            return ret;
        }
    
        void deallocate(pointer p, size_type n)
        {
            free(p);
        }
        
        size_type max_size() const throw() 
        {
            return size_type(-1)/sizeof(T);
        }
        
        void construct(pointer p, const_reference val) 
        {
            ::new((void *)p) value_type(val);
        }
        
        template<typename... Args> void construct(pointer p, Args&&... args)
	{
	    ::new((void *)p) value_type(std::forward<Args>(args)...);
	}

        void destroy(pointer p)
        {
            p->~value_type();
        }
    };
}
#endif
