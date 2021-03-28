#include "arm_vector.h"

template <typename T>
class arm_Vector
{
private:
    //! The pointer of the buffer for this vector
    T* buffer_;
    //! The actual size of the vector
    size_t capacity_;
    //! The index of the next free element (pseudo-last element)
    size_t size_;
public:
    //! The default constructor: all elements are zero (using calloc), default size is 10
    arm_Vector() :
        size_ (0),
        capacity_ (START_SIZE),
        buffer_ (static_cast<T*>(calloc(START_SIZE, sizeof(T))))
    {
        std::cout << "Vector of \"" << typeid(T).name() << "\" initialized with " << START_SIZE << " capacity." << std::endl;
    }
    //! The constructor with the stated capacity
    explicit arm_Vector(size_t Capacity) :
        size_ (0),
        capacity_ (Capacity),
        buffer_ (static_cast<T*>(calloc(Capacity, sizeof(T))))
    {}
    //! The constructor with the stated capacity and the element for the vector to be filled with
    arm_Vector(size_t Capacity, const T& var) :
        size_ (Capacity),
        capacity_ (Capacity),
        buffer_ (static_cast<T*>(calloc(Capacity, sizeof(T))))
    {
        int counter = 0;

        for (counter = 0; counter < capacity_; counter++)
        {
            buffer_[counter] = var;
        }
    }
    //! The copy constructor - creates a new buffer with the same capacity, copies all elements till "size_"
    arm_Vector(const arm_Vector& that) :
        size_ (that.size_),
        capacity_ (that.capacity_),
        buffer_ (static_cast<T*>(calloc(that.capacity_, sizeof(T))))
    {
        int counter = 0;
         for (counter = 0; counter < size_; counter++)
         {
             that.buffer_[counter] = this->buffer_[counter];
         }
    }
    //! The steal constructor, doesn't create the buffer - just steals it and sets the buffer to zero (of the "robbed" buffer)
    arm_Vector(arm_Vector&& that) :
        size_ (that.size_),
        capacity_ (that.capacity_),
        buffer_ (that.buffer_)
    {
        that.buffer_ = nullptr;
        that.size_ = POISON_SIZE;
        that.capacity_ = POISON_SIZE;
        std::cout << "ROBBERY OS PROCEEDING, KEEP SAFE" << std::endl;
    }
    //! The destructor - sets all variables to zero, the buffer is deleted
    ~arm_Vector()
    {
        size_ = 0;
        capacity_ = 0;
        delete[](buffer_);
        std::cout << "Vector deleted." << std::endl;
    }
    T& operator [](const int index)
    {
        try
        {
            if (index < 0 || index >= capacity_)
                throw(ENUM_INVALID_INDEX);
        }
        ENUM_CATCH_ERROR
        if (size_ < index)
            size_ = index + 1;
        return buffer_[index];
    }
    arm_Vector& operator =(const arm_Vector& that)
    {
        int counter = 0;

        if (this == &that)
            return *this;

        for (counter = 0; counter < MIN(that.size_, this->capacity_); counter++)
        {
            this->buffer_[counter] = that.buffer_[counter];
        }

        return *this;
    }
    //! Show the first element from the vector without deleting it
    T& pull_front()
    {
        try
        {
            if (size_ == 0)
                throw(ENUM_EMPTY_VECTOR);
        }
        ENUM_CATCH_ERROR
        return buffer_[0];
    }
    //! Show the last element from the vector without deleting it
    T& pull_back()
    {
        try
        {
            if (size_ == 0)
                throw(ENUM_EMPTY_VECTOR);
        }
        ENUM_CATCH_ERROR
        return buffer_[size_ - 1];
    }
    //! Puts the element at the end of the vector
    void push_back(const T& element)
    {
        if (size_ >= capacity_)
        {
            buffer_ = static_cast<T*> (realloc(buffer_, capacity_ * 2));
            capacity_ *= 2;
        }

        buffer_[size_++] = element;
    }
    //!
    void clear()
    {
        delete[](buffer_);
        buffer_ = static_cast<T*> (calloc(START_SIZE, sizeof(T)));
        size_ = 0;
        capacity_ = START_SIZE;
    }
    //! Returns the current capacity of the vector
    size_t get_capacity()
    {
        return capacity_;
    }
    //! Returns the current size of the vector
    size_t get_size()
    {
        return size_;
    }
    //! Returns true if the vector is empty, otherwise returns false
    bool is_empty()
    {
        return size_ <= 0;
    }
};
