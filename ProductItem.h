#pragma once
#include <cstdint>
#include <string>
#include <iostream>
#include <list>
#include <algorithm>

//entity that represents a single product in the application

//TODO: redo files to that they have meta-data that describes the file
//e.g version number, number of products in the file ect
class ProductItem
{
public:

	class IdGen
	{
	public:
		IdGen(std::uint64_t startFrom = 0);
		static std::uint64_t idSeed;
		static std::uint64_t GetID();
	};


	ProductItem();
	ProductItem(std::uint64_t id, const std::string& ProductName,
		const std::string& ProductActIng,
		const std::string& CategoryName,
		const std::string& ProductDesc,
		const std::string& DirForUse,
		const std::string& ProductClass,
		std::uint32_t stockCount,
		std::uint32_t packageSize,
		float uintPrice);


	~ProductItem();
	ProductItem& operator=(const ProductItem& product);
	ProductItem& operator=(const ProductItem&& product) noexcept;

	ProductItem(const ProductItem& product);
	ProductItem(ProductItem&& product) noexcept;

	bool operator==(ProductItem& compare);
	bool operator==(const ProductItem& compare) const;

	bool operator<(ProductItem& compare);
	bool operator<(const ProductItem& compare) const;

	bool operator>(const ProductItem& compare) const;
	bool operator!=(const ProductItem& compare) const;




	friend std::ostream& operator<<(std::ostream& os, const ProductItem& item);
	friend std::istream& operator>>(std::istream& os, ProductItem& item);

	std::size_t GetMemSpace();

	inline const std::string& GetProductName() const { return mProductName; }
	inline std::string& ProductName() { return mProductName; }

	inline const std::string& GetProductActIng() const { return mProductActiveIngredent; }
	inline std::string& ProductActiveIng() { return mProductActiveIngredent; }

	inline const std::string& GetCategoryName() const { return mCategoryName; }
	inline std::string& CategoryName() { return mCategoryName; }

	inline const std::string& GetProdcutDesc() const { return mProductDesc; }
	inline std::string& ProductDesc() { return mProductDesc; }

	inline const std::string& GetDirForUse() const { return mDirForUse; }
	inline std::string& DirForUse() { return mDirForUse; }

	inline std::uint32_t GetStockCount() const { return mStockCount; }
	std::uint32_t& StockCount() { return mStockCount; }

	inline std::uint32_t GetPackageSize() const { return mPackageSize; }
	std::uint32_t& PackageSize() { return mPackageSize; }

	inline float GetUnitPrice() const { return mUnitPrice; }
	float& UnitPrice() { return mUnitPrice; }

	std::uint64_t GetID() const { return mProductID; }
	inline std::uint64_t& Id() { return mProductID; }

	inline const std::string& GetProductClass() const { return mProductClass; }
	inline std::string& ProductClass() { return mProductClass; }

	const std::list<std::string>& GetHealthTag() const { return mHealthTag; }
	std::list<std::string>& HealthTag() { return mHealthTag; }


	bool IsEmpty() const;

public:
	//Health tag management
	void AddTag(const std::string& tag);
	void RemoveTag(const std::string& tag);
	void WriteTag(std::ostream& os) const;
	void ReadTag(std::istream& os);

//for read write for internal use 
	static void AllocateReadBuffer();
	static void DeallocateReadBuffer();
	//read buffer
	static char* buffer;

private:

	std::uint64_t mProductID;
	std::string mProductName;
	std::string mProductActiveIngredent;
	std::string mCategoryName;
	std::string mProductDesc;
	std::string mDirForUse;
	std::string mProductClass;
	std::uint32_t mStockCount;
	std::uint32_t mPackageSize;
	float mUnitPrice;	

private:
	//Health tags: possible conditions that this product is used to threat
	std::list<std::string> mHealthTag;



private:
	//settings and debug
};
