#include "IRemoveCriteria.h"
#include "Figure.h"
template <class T> class IRemoveCriteriaByValue : public IRemoveCriteria<T> {
public:
	IRemoveCriteriaByValue(T&& value) : _value(&value) {};
	bool isIt(T* value) override {
		if (value == nullptr) {
			return false;
		}
		//return false;
		return  size >= value->Square();
	}
	int size;
private:
	T* _value;
};