#include "ProductItem.h"
std::uint64_t ProductItem::IdGen::idSeed = 0;

ProductItem::ProductItem()
:mProductID{0},
mStockCount{ 0 }, mUnitPrice{0.0}{
}

ProductItem::ProductItem(std::uint64_t id, const std::string& ProductName, const std::string& productActIng, const std::string& CategoryName, const std::string& ProductDesc, const std::string& DirForUse, const std::string& ProductClass, std::uint32_t stockCount, float unitPrice)
	: mProductID{id}, mProductName(ProductName), mProductActiveIngredent(productActIng), mCategoryName(CategoryName), mProductDesc(ProductDesc), mDirForUse(DirForUse), mProductClass(ProductClass), mStockCount(stockCount), mUnitPrice(unitPrice) {
}

ProductItem::~ProductItem()
{
	//can safly destory for now

}


ProductItem& ProductItem::operator=(const ProductItem& product)
{
	// TODO: insert return statement here
	if (this == &product)
	{
		return (*this);
	}
	mProductID = product.mProductID;
	mProductName = product.mProductName;
	mProductActiveIngredent = product.mProductActiveIngredent;
	mCategoryName = product.mCategoryName;
	mProductDesc = product.mProductDesc;
	mDirForUse = product.mDirForUse;
	mProductClass = product.mProductClass;
	mStockCount = product.mStockCount;
	mUnitPrice = product.mUnitPrice;
	mHealthTag = product.mHealthTag;
	return(*this);
}

ProductItem& ProductItem::operator=(const ProductItem&& product) noexcept
{
	if (this == &product)
	{
		return *this;
	}

	mProductID = product.mProductID;
	mProductName = std::move(product.mProductName);
	mProductActiveIngredent = std::move(product.mProductActiveIngredent);
	mCategoryName = std::move(product.mCategoryName);
	mProductDesc = std::move(product.mProductDesc);
	mDirForUse = std::move(product.mDirForUse);
	mProductClass = std::move(product.mProductClass);
	mStockCount = product.mStockCount;
	mUnitPrice = product.mUnitPrice;
	mHealthTag = product.mHealthTag;
	return (*this);
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
	mProductClass = product.mProductClass;
	mStockCount = product.mStockCount;
	mUnitPrice = product.mUnitPrice;
	mHealthTag = product.mHealthTag;
}

ProductItem::ProductItem(ProductItem&& product) noexcept
{
	mProductID = product.mProductID;
	mProductName = std::move(product.mProductName);
	mProductActiveIngredent = std::move(product.mProductActiveIngredent);
	mProductDesc = std::move(product.mProductDesc);
	mDirForUse = std::move(product.mDirForUse);
	mProductClass = std::move(product.mProductClass);
	mStockCount = product.mStockCount;
	mUnitPrice = product.mUnitPrice;
	mHealthTag = product.mHealthTag;

}

std::size_t ProductItem::GetMemSpace()
{
	std::size_t bytes = 0;
	bytes += sizeof(std::uint64_t);
	bytes += sizeof(std::string::value_type) * mProductName.size();
	bytes += sizeof(std::string::value_type) * mCategoryName.size();
	bytes += sizeof(std::string::value_type) * mProductDesc.size();
	bytes += sizeof(std::string::value_type) * mDirForUse.size();
	bytes += sizeof(std::string::value_type) * mProductClass.size();
	bytes += sizeof(std::uint32_t);
	bytes += sizeof(float);
	return bytes;
}

bool ProductItem::IsEmpty() const 
{
	return (mProductID == 0);
}

void ProductItem::AddTag(const std::string& tag)
{
	if (!tag.empty()) mHealthTag.push_back(tag);
}

void ProductItem::RemoveTag(const std::string& tag)
{
	if (!tag.empty())
	{
		//linear search complexity, the list is a small list removing that expensive relax
		auto iterWhere = std::find(mHealthTag.begin(), mHealthTag.end(), tag);
		mHealthTag.erase(iterWhere);
	}
}

void ProductItem::WriteTag(std::ostream& os) const
{
	if (!mHealthTag.empty())
	{
		//write the size
		std::size_t size = mHealthTag.size();
		os.write((const char*)& size, sizeof(std::size_t));
		for (auto& i : mHealthTag)
		{
			const size_t bytes = sizeof(std::string::value_type) * i.size();
			os.write((const char*)& bytes, sizeof(std::size_t));
			os.write(i.data(), bytes);
		}
	}
}

void ProductItem::ReadTag(std::istream& os)
{
	std::size_t listSize = 0, strSize = 0;
	char* buffer = new char[256];
	os.read((char*)& listSize, sizeof(std::size_t));
	for (int i = 0; i < listSize; i++)
	{
		os.read((char*)& strSize, sizeof(std::size_t));
		if (strSize >= 256) strSize = 255;
		memset(buffer, '\0', 256);
		os.read(buffer, strSize);
		mHealthTag.push_back(std::string(buffer));
	}

	delete[] buffer;
	return;
}


std::ostream& operator<<(std::ostream& os, const ProductItem& item)
{
	std::size_t bytes = 0;
	bytes = sizeof(std::uint64_t);
	os.write((const char*)&item.mProductID, bytes);

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


	bytes = sizeof(std::string::value_type) * item.mProductClass.size();
	os << bytes;
	os.write(item.mProductClass.data(), bytes);

	bytes = sizeof(std::uint32_t);
	os.write((const char*)& item.mStockCount, bytes);

	bytes = sizeof(float);
	os.write((const char*)& item.mUnitPrice, bytes);

	item.WriteTag(os);

	return os;
}

std::istream& operator>>(std::istream& os, ProductItem& item)
{
	//buffer overlow attach here, big security issue

	char* buffer = new char[256];
	std::size_t bytes = sizeof(std::uint64_t);
	os.read((char*)&item.mProductID, bytes);

	os >> bytes;
	if(bytes >= 256) bytes = 255;
	memset(buffer, '\0', 256);
	os.read(buffer, bytes);
	item.mProductName = std::string(buffer);

	os >> bytes;
	if(bytes >= 256) bytes = 255;
	memset(buffer, '\0', 256);
	os.read(buffer, bytes);
	item.mProductActiveIngredent = std::string(buffer);

	os >> bytes;
	if(bytes >= 256) bytes = 255;
	memset(buffer, '\0', 256);
	os.read(buffer, bytes);
	item.mCategoryName = std::string(buffer);

	os >> bytes;
	if(bytes >= 256) bytes = 255;
	memset(buffer, '\0', 256);
	os.read(buffer, bytes);
	item.mProductDesc = std::string(buffer);

	os >> bytes;
	if(bytes >= 256) bytes = 255;
	memset(buffer, '\0', 256);
	os.read(buffer, bytes);
	item.mDirForUse = std::string(buffer);

	os >> bytes;
	if (bytes >= 256) bytes = 255;
	memset(buffer, '\0', 256);
	os.read(buffer, bytes);
	item.mProductClass = std::string(buffer);

	bytes = sizeof(std::uint32_t);
	os.read((char*)& item.mStockCount, bytes);

	bytes = sizeof(float);
	os.read((char*)& item.mUnitPrice, bytes);

	item.ReadTag(os);


	delete[] buffer;
	return os;

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
