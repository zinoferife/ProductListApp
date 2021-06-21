#include "ProductItem.h"
std::uint64_t ProductItem::IdGen::idSeed = 0;

ProductItem::ProductItem()
:mProductID{0},
mStockCount{ 0 }, mUnitPrice{0.0}{
}

ProductItem::ProductItem(std::uint64_t id, const std::string& ProductName, const std::string& productActIng, const std::string& CategoryName, const std::string& ProductDesc, const std::string& DirForUse, std::uint32_t stockCount, float unitPrice)
	: mProductID{id}, mProductName(ProductName), mProductActiveIngredent(productActIng), mCategoryName(CategoryName), mProductDesc(ProductDesc), mDirForUse(DirForUse), mStockCount(stockCount), mUnitPrice(unitPrice) {
}

ProductItem::~ProductItem()
{
	//can safly destory for now

}

ProductItem& ProductItem::operator=(const ProductItem& product) 
{
	mProductID = product.mProductID;
	mProductName = product.mProductName;
	mProductActiveIngredent = product.mProductActiveIngredent;
	mCategoryName = product.mCategoryName;
	mProductDesc = product.mProductDesc;
	mDirForUse = product.mDirForUse;
	mStockCount = product.mStockCount;
	mUnitPrice = product.mUnitPrice;
	return (*this);
}

ProductItem& ProductItem::operator=(ProductItem& product)
{
	// TODO: insert return statement here
	mProductID = product.mProductID;
	mProductName = product.mProductName;
	mProductActiveIngredent = product.mProductActiveIngredent;
	mCategoryName = product.mCategoryName;
	mProductDesc = product.mProductDesc;
	mDirForUse = product.mDirForUse;
	mStockCount = product.mStockCount;
	mUnitPrice = product.mUnitPrice;
	return(*this);
}

bool ProductItem::operator==(ProductItem& compare)
{
	return (mProductName == compare.mProductName);
}

bool ProductItem::operator==(const ProductItem& compare) const
{
	return (mProductName == compare.mProductName);
}

bool ProductItem::operator<(ProductItem& compare)
{
	return (mProductName < compare.mProductName);
}

bool ProductItem::operator<(const ProductItem& compare) const
{
	return (mProductName < compare.mProductName);
}

bool ProductItem::operator>(const ProductItem& compare) const
{
	return !(*this < compare);
}

bool ProductItem::operator!=(const ProductItem& compare) const
{
	return !(*this == compare);
}

ProductItem::ProductItem(const ProductItem& product)
{
	mProductID = product.mProductID;
	mProductName = product.mProductName;
	mProductActiveIngredent = product.mProductActiveIngredent;
	mCategoryName = product.mCategoryName;
	mProductDesc = product.mProductDesc;
	mDirForUse = product.mDirForUse;
	mStockCount = product.mStockCount;
	mUnitPrice = product.mUnitPrice;
}

std::size_t ProductItem::GetMemSpace()
{
	std::size_t bytes = 0;
	bytes += sizeof(std::uint64_t);
	bytes += sizeof(std::string::value_type) * mProductName.size();
	bytes += sizeof(std::string::value_type) * mCategoryName.size();
	bytes += sizeof(std::string::value_type) * mProductDesc.size();
	bytes += sizeof(std::string::value_type) * mDirForUse.size();
	bytes += sizeof(std::uint32_t);
	bytes += sizeof(float);
	return bytes;
}

bool ProductItem::IsEmpty() const 
{
	return (mProductID == 0);
}


std::ostream& operator<<(std::ostream& os, const ProductItem& item)
{
	std::size_t bytes = 0;
	bytes = sizeof(std::uint64_t);
	os.write((const char*)item.mProductID, bytes);

	bytes = sizeof(std::string::value_type) * item.mProductName.size();
	os << bytes;
	os.write(item.mProductName.data(), bytes);

	bytes = sizeof(std::string::value_type) * item.mProductActiveIngredent.size();
	os << bytes;
	os.write(item.mProductActiveIngredent.data(), bytes);


	bytes = sizeof(std::string::value_type) * item.mCategoryName.size();
	os << bytes;
	os.write(item.mCategoryName.data(), bytes);

	bytes = sizeof(std::string::value_type) * item.mProductDesc.size();
	os << bytes;
	os.write(item.mProductDesc.data(), bytes);


	bytes = sizeof(std::string::value_type) * item.mDirForUse.size();
	os << bytes;
	os.write(item.mDirForUse.data(), bytes);

	bytes = sizeof(std::uint32_t);
	os.write((const char*)& item.mStockCount, bytes);

	bytes = sizeof(float);
	os.write((const char*)& item.mUnitPrice, bytes);
}

std::istream& operator>>(std::istream& os, ProductItem& item)
{
	char* buffer = new char[256];
	std::size_t bytes = sizeof(std::uint64_t);
	os.read((char*)&item.mProductID, bytes);

	os >> bytes;
	os.read(buffer, bytes);
	buffer[bytes + 1] = '\0';
	item.mProductName = std::string(buffer);

	os >> bytes;
	os.read(buffer, bytes);
	buffer[bytes + 1] = '\0';
	item.mProductActiveIngredent = std::string(buffer);

	os >> bytes;
	os.read(buffer, bytes);
	buffer[bytes + 1] = '\0';
	item.mCategoryName = std::string(buffer);

	os >> bytes;
	os.read(buffer, bytes);
	buffer[bytes + 1] = '\0';
	item.mProductDesc = std::string(buffer);

	os >> bytes;
	os.read(buffer, bytes);
	buffer[bytes + 1] = '\0';
	item.mDirForUse = std::string(buffer);

	bytes = sizeof(std::uint32_t);
	os.read((char*)& item.mStockCount, bytes);

	bytes = sizeof(float);
	os.read((char*)& item.mUnitPrice, bytes);

}

ProductItem::IdGen::IdGen(std::uint64_t startFrom)
{
	if (startFrom != 0)
	{
		idSeed = startFrom;
	}
}

std::uint64_t ProductItem::IdGen::GetID()
{
	return ++idSeed;
}
