/*
#include <memory>
#include <iostream>
template <class node, class T>
class TIterator
{
public:
	TIterator(node * n) {
		node_ptr = n;
	}
	node * operator * () {
		return node_ptr;
	}
	std::shared_ptr<node> operator -> () {
		return node_ptr;
	}
	void operator ++ () {
		node_ptr = (node_ptr->GetNext());
	}
	TIterator operator ++ (int) {
		TIterator iter(*this);
		++(*this);
		return iter;
	}
	bool operator == (TIterator const& i) {
		return node_ptr == i.node_ptr;
	}
	bool operator != (TIterator const& i) {
		return !(*this == i);
	}
private:
	node * node_ptr;
};
*/

#include <memory>
#include <iostream>

template <class N, class T>
class TIterator
{
public:
	TIterator(std::shared_ptr<N> n) {
		cur = n;
	}

	std::shared_ptr<T> operator* () {
		return cur->get_Figure();
	}

	std::shared_ptr<T> operator-> () {
		return cur->get_Figure();
	}

	void operator++() {
		cur = cur->get_son();
		cur = cur->get_bro();
	}

	TIterator operator++ (int) {
		TIterator cur(*this);
		++(*this);
		return cur;
	}

	bool operator == (const TIterator &i) {
		return cur == i.cur;
	}

	bool operator!= (const TIterator &i) {
		return (cur != i.cur);
	}

private:
	std::shared_ptr<N> cur;
};
