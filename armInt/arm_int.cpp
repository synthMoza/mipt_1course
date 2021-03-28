#include <iostream>

class arm_int
{
private:
    int value_;
public:
    arm_int() :
        value_ (0)
    {}
    arm_int(int var) :
        value_ (var)
        {};
    arm_int(const arm_int& that) :
        value_ (that.value_) {};
    arm_int(arm_int&& temp) :
        value_ (temp.value_)
    {
        temp.value_ = 0;
    };

    //operators
    friend arm_int operator +(const arm_int& a, const arm_int& b);
    //friend arm_int operator +(const arm_int& a, const int& b);
    //friend arm_int operator +(const int& a, const arm_int& b);
    friend arm_int operator -(const arm_int& a, const arm_int& b);
    //friend arm_int operator -(const arm_int& a, const int& b);
    //friend arm_int operator -(const int& a, const arm_int& b);

    //unary
    friend const arm_int& operator -(arm_int& a);
    //prefix
    friend const arm_int& operator ++(arm_int& a);
    friend const arm_int& operator --(arm_int& a);
    //postfix
    friend const arm_int operator ++(arm_int& a, int);
    friend const arm_int operator --(arm_int& a, int);

    //binary
    friend bool operator ==(const arm_int& left, const arm_int& right);
    friend bool operator !=(const arm_int& left, const arm_int& right);

    friend bool operator >(const arm_int& left, const arm_int& right);
    friend bool operator >=(const arm_int& left, const arm_int& right);
    friend bool operator <(const arm_int& left, const arm_int& right);
    friend bool operator <=(const arm_int& left, const arm_int& right);

    friend arm_int operator *(const arm_int& a, const arm_int& b);
    //friend arm_int operator *(const arm_int& a, const int& b);
    //friend arm_int operator *(const int& a, const arm_int& b);

    friend arm_int operator /(const arm_int& a, const arm_int& b);
    //friend arm_int operator /(const arm_int& a, const int& b);
    //friend arm_int operator /(const int& a, const arm_int& b);

    friend arm_int operator %(const arm_int& a, const arm_int& b);
    //friend arm_int operator %(const arm_int& a, const int& b);
    //friend arm_int operator %(const int& a, const arm_int& b);

    friend arm_int& operator +=(arm_int& left, const arm_int& right);
    //friend int& operator +=(int& left, const arm_int& right);
    //friend arm_int& operator +=(arm_int& left, const int& right);

    friend arm_int& operator -=(arm_int& left, const arm_int& right);
    //friend int& operator -=(int& left, const arm_int& right);
    //friend arm_int& operator -=(arm_int& left, const int& right);

    friend arm_int& operator *=(arm_int& left, const arm_int& right);
    //friend int& operator *=(int& left, const arm_int& right);
    //friend arm_int& operator *=(arm_int& left, const int& right);

    friend arm_int& operator /=(arm_int& left, const arm_int& right);
    //friend int& operator /=(int& left, const arm_int& right);
    //friend arm_int& operator /=(arm_int& left, const int& right);

    friend arm_int& operator %=(arm_int& left, const arm_int& right);
    //friend int& operator %=(int& left, const arm_int& right);
    //friend arm_int& operator %=(arm_int& left, const int& right);

    friend std::ostream& operator <<(std::ostream& out, const arm_int& a);
    friend std::istream& operator >>(std::istream& in, arm_int& a);

    arm_int& operator =(const arm_int& that)
    {
        if (this == &that)
            return *this;

        value_ = that.value_;
        return *this;
    }

    arm_int& operator =(const int& that_value)
    {
        value_ = that_value;
        return *this;
    }
};
//add
arm_int operator +(const arm_int& a, const arm_int& b)
{
    arm_int result = 0;
    result.value_ = a.value_ + b.value_;
    return result;
}
/*
arm_int operator +(const arm_int& a, const int& b)
{
    arm_int result = 0;
    result.value_ = a.value_ + b;
    return result;
}

arm_int operator +(const int& a, const arm_int& b)
{
    arm_int result = 0;
    result.value_ = a + b.value_;
    return result;
}
 */
//sub
arm_int operator -(const arm_int& a, const arm_int& b)
{
    arm_int result = 0;
    result.value_ = a.value_ - b.value_;
    return result;
}
/*
arm_int operator -(const arm_int& a, const int& b)
{
    arm_int result = 0;
    result.value_ = a.value_ - b;
    return result;
}

arm_int operator -(const int& a, const arm_int& b)
{
    arm_int result = 0;
    result.value_ = a - b.value_;
    return result;
}
*/
//mul
arm_int operator *(const arm_int& a, const arm_int& b)
{
    arm_int result = 0;
    result.value_ = a.value_ * b.value_;
    return result;
}
/*
arm_int operator *(const arm_int& a, const int& b)
{
    arm_int result = 0;
    result.value_ = a.value_ * b;
    return result;
}

arm_int operator *(const int& a, const arm_int& b)
{
    arm_int result = 0;
    result.value_ = a * b.value_;
    return result;
}
*/
//div
arm_int operator /(const arm_int& a, const arm_int& b)
{
    arm_int result = 0;
    result.value_ = a.value_ / b.value_;
    return result;
}
/*
arm_int operator /(const arm_int& a, const int& b)
{
    arm_int result = 0;
    result.value_ = a.value_ / b;
    return result;
}

arm_int operator /(const int& a, const arm_int& b)
{
    arm_int result = 0;
    result.value_ = a / b.value_;
    return result;
}
*/
//%
arm_int operator %(const arm_int& a, const arm_int& b)
{
    arm_int result = 0;
    result.value_ = a.value_ % b.value_;
    return result;
}
/*
arm_int operator %(const arm_int& a, const int& b)
{
    arm_int result = 0;
    result.value_ = a.value_ % b;
    return result;
}

arm_int operator %(const int& a, const arm_int& b)
{
    arm_int result = 0;
    result.value_ = a % b.value_;
    return result;
}
*/
//unary plus
const arm_int& operator +(const arm_int& a)
{
    return a;
}

//unary sub
const arm_int& operator -(arm_int& a)
{
    a.value_ = -1 * a.value_;
    return a;
}

//prefix ++
const arm_int& operator ++(arm_int& a)
{
    a.value_++;
    return a;
}
//prefix --
const arm_int& operator --(arm_int& a)
{
    a.value_--;
    return a;
}
//postfix ++
const arm_int operator ++(arm_int& a, int)
{
    arm_int old_value = a;
    a.value_++;
    return old_value;
}
//postfix --
const arm_int operator --(arm_int& a, int)
{
    arm_int old_value = a;
    a.value_--;
    return old_value;
}
//condition
bool operator ==(const arm_int& left, const arm_int& right)
{
    return left.value_ == right.value_;
}
bool operator !=(const arm_int& left, const arm_int& right)
{
    return left.value_ != right.value_;
}
bool operator >(const arm_int& left, const arm_int& right)
{
    return left.value_ > right.value_;
}
bool operator >=(const arm_int& left, const arm_int& right)
{
    return left.value_ >= right.value_;
}
bool operator <(const arm_int& left, const arm_int& right)
{
    return left.value_ < right.value_;
}
bool operator <=(const arm_int& left, const arm_int& right)
{
    return left.value_ <= right.value_;
}

arm_int& operator +=(arm_int& left, const arm_int& right)
{
    left.value_ += right.value_;
    return left;
}
/*
int& operator +=(int& left, const arm_int& right)
{
    left += right.value_;
    return left;
}
arm_int& operator +=(arm_int& left, const int& right)
{
    left.value_ += right;
    return left;
}
*/
arm_int& operator -=(arm_int& left, const arm_int& right)
{
    left.value_ -= right.value_;
    return left;
}
/*
int& operator -=(int& left, const arm_int& right)
{
    left -= right.value_;
    return left;
}
arm_int& operator -=(arm_int& left, const int& right)
{
    left.value_ -= right;
    return left;
}
*/
arm_int& operator *=(arm_int& left, const arm_int& right)
{
    left.value_ *= right.value_;
    return left;
}
/*
int& operator *=(int& left, const arm_int& right)
{
    left *= right.value_;
    return left;
}
arm_int& operator *=(arm_int& left, const int& right)
{
    left.value_ *= right;
    return left;
}
*/
arm_int& operator /=(arm_int& left, const arm_int& right)
{
    left.value_ /= right.value_;
    return left;
}
/*
int& operator /=(int& left, const arm_int& right)
{
    left /= right.value_;
    return left;
}
arm_int& operator /=(arm_int& left, const int& right)
{
    left.value_ /= right;
    return left;
}
*/
arm_int& operator %=(arm_int& left, const arm_int& right)
{
    left.value_ %= right.value_;
    return left;
}
/*
int& operator %=(int& left, const arm_int& right)
{
    left %= right.value_;
    return left;
}
arm_int& operator %=(arm_int& left, const int& right)
{
    left.value_ %= right;
    return left;
}
*/
std::ostream& operator <<(std::ostream& out, const arm_int& a)
{
    out << a.value_;

    return out;
}

std::istream& operator >>(std::istream& in, arm_int& a)
{
    in >> a.value_;

    return in;
}

int main() {
    arm_int gru = 0;
    arm_int var = 0;

    std::cout << "Enter your first value:" << std::endl;
    std::cin >> gru;

    std::cout << "Enter your second value:" << std::endl;
    std::cin >> var;

    if (gru == var)
        std::cout << "Grandly equal";

    std::cout << (gru + var) << std::endl;
    std::cout << (gru * var) << std::endl;
    std::cout << (gru / var) << std::endl;
    return 0;
}
