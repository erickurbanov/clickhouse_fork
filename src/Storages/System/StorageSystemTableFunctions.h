#pragma once

#include <boost/noncopyable.hpp>
#include <DataTypes/DataTypeString.h>
#include <Storages/System/IStorageSystemOneBlock.h>

namespace DB
{

class StorageSystemTableFunctions final : public IStorageSystemOneBlock<StorageSystemTableFunctions>, boost::noncopyable
{
protected:
    using IStorageSystemOneBlock::IStorageSystemOneBlock;

    void fillData(MutableColumns & res_columns, ContextPtr context, const SelectQueryInfo & query_info) const override;

public:
    std::string getName() const override
    {
        return "SystemTableFunctions";
    }

    static NamesAndTypesList getNamesAndTypes();

};

}
