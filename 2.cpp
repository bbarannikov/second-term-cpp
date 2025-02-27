#include <iostream>
#include <numeric>
void extgcd(int a, int b, int& k_a, int& l_b, int& gcd)
{
    k_a = 1;
    int k_b = 0;
    int l_a = 0;
    l_b = 1;
    int temp = 0;
    if(a > b)
    {
        while(b != 0)
        {
            temp = k_b; k_b = k_a - k_b * (a / b); k_a = temp;
            if(a % b != 0)
            {
                temp = l_b; l_b = l_a - l_b * (a / b); l_a = temp;
            }
            temp = a; a = b; gcd = b; b = temp % b;
        }
    }
    else
    {
        extgcd(b, a, l_b, k_a, gcd);
    }
}

void solve(int a, int b, int c, int& x, int& y)
{
    int gcd, k_a, k_b;
    extgcd(a, b, x, y, gcd);
    if (c % gcd == 0)
    {
        x *= (c / gcd);
        y *= (c / gcd);
    }
    else
    {
        x = 0; y = 0;
    }
}

int main()
{
    int a, b, c, x, y, gcd;
    std::cin >> a >> b >> c;
    extgcd(a, b, x, y, gcd);
    std::cout << a << "*" << x << "+" << b << "*" << y << "=" << gcd << std::endl;
    solve(a, b, c, x, y);
    std::cout << a << "*" << x << "+" << b << "*" << y << "=" << c << std::endl;
}