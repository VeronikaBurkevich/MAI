template <class T> class IRemoveCriteria {
public:
	virtual bool isIt(T* value) = 0;
private:
};