#pragma once
#include <cstdint>
#include <string>
#include <iostream>

//entity that represents a single product in the application
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
		std::uint32_t stockCount,
		float uintPrice);


	~ProductItem();
	ProductItem& operator=(const ProductItem& product);
	ProductItem& operator=(ProductItem& product);

	bool operator==(ProductItem& compare);
	bool operator==(const ProductItem& compare) const;

	bool operator<(ProductItem& compare);
	bool operator<(const ProductItem& compare) const;

	bool operator>(const ProductItem& compare) const;
	bool operator!=(const ProductItem& compare) const;


	ProductItem(const ProductItem& product);
	ProductItem(ProductItem& product);


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

	inline float GetUnitPrice() const { return mUnitPrice; }
	float& UnitPrice() { return mUnitPrice; }



	bool IsEmpty() const;

private:

	std::uint64_t mProductID;
	std::string mProductName;
	std::string mProductActiveIngredent;
	std::string mCategoryName;
	std::string mProductDesc;
	std::string mDirForUse;
	std::uint32_t mStockCount;
	float mUnitPrice;	

private:
	//settings and debug
};
