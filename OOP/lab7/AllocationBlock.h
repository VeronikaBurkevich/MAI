#ifndef ALLOCATIONBLOCK_H
#define ALLOCATIONBLOCK_H
#include <cstdlib>
class TAllocationBlock {
public:
	TAllocationBlock(size_t size, size_t count);
	void *allocate();
	void deallocate(void *pointer);
	bool has_free_blocks();
	virtual ~TAllocationBlock();
private:
	size_t _size;
	size_t _count;
	char *_used_blocks;
	void **_free_blocks;//node * root;
	size_t _free_count;
};
#endif /* ALLOCATIONBLOCK_H */