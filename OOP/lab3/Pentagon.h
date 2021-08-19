#include "Figure.h"
class pentagon :
	public Figure
{
public:
	pentagon();
	pentagon(std::istream &is);
	pentagon(size_t a);
	pentagon(const pentagon& orig);

	double square() override;
	void print() override;
	friend std::ostream& operator<<(std::ostream& os, const pentagon& right);
	friend std::istream& operator>>(std::istream& is, pentagon& right);
	friend bool operator==(const pentagon& left, const pentagon& right);
	pentagon& operator=(const pentagon& right);
	~pentagon();
private:
	size_t side_a;
};

