#include <iostream>

class AdvancedType
{
private:
	SimpleType simple;
	
public:
	AdvancedType() {
		std::cout << "AdvancedType::ctor" << std::endl;
	}
	AdvancedType(int a) : simple(a, a) {
		std::cout << "AdvancedType::ctor(a)" << std::endl;
	}
	~AdvancedType() { std::cout << "AdvancedType::destructor" << std::endl; }
};

int main()
{
	AdvancedType advObj;
	AdvancedType advObj2(10);
}