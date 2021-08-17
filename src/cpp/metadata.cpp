/*
 * BSD 2-Clause License
 *
 * Copyright (c) 2021, Hewlett Packard Enterprise
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "metadata.h"

using namespace SmartRedis;

// MetaData copy constructor
MetaData::MetaData(const MetaData& metadata)
{
    _clone_from(metadata);
}

// MetaData copy assignment operator
MetaData& MetaData::operator=(const MetaData& metadata)
{
    this->_clone_from(metadata);
    return *this;
}

// MetaData move assignment operator
MetaData& MetaData::operator=(MetaData&& metadata)
{
    // Check for self-move
    if (this == &metadata)
        return *this;

    // Clear out fields
        this->_delete_fields();

    // Migrate data
    this->_field_map = std::move(metadata._field_map);
    this->_char_array_mem_mgr = std::move(metadata._char_array_mem_mgr);
    this->_char_mem_mgr = std::move(metadata._char_mem_mgr);
    this->_double_mem_mgr = std::move(metadata._double_mem_mgr);
    this->_float_mem_mgr = std::move(metadata._float_mem_mgr);
    this->_int64_mem_mgr = std::move(metadata._int64_mem_mgr);
    this->_uint64_mem_mgr = std::move(metadata._uint64_mem_mgr);
    this->_int32_mem_mgr = std::move(metadata._int32_mem_mgr);
    this->_uint32_mem_mgr = std::move(metadata._uint32_mem_mgr);
    this->_str_len_mem_mgr = std::move(metadata._str_len_mem_mgr);

    // Done
    return *this;
}

// Metadata destructor
MetaData::~MetaData()
{
    this->_delete_fields();
}

// Clone data from another Metadata instance
void MetaData::_clone_from(const MetaData& other)
{
    // Protect against a self-copy
    if (this == &other)
        return;

    // Clean out the old data
    this->_delete_fields();

    // Clone the fields
    std::unordered_map<std::string, MetadataField*>::const_iterator it =
        other._field_map.cbegin();
    for ( ; it != other._field_map.cend(); it++) {
        this->_create_field(it->first, it->second->type());
        this->_deep_copy_field(this->_field_map[it->first], it->second);
    }

    // Clone the memory managers
    this->_char_array_mem_mgr = other._char_array_mem_mgr;
    this->_char_mem_mgr = other._char_mem_mgr;
    this->_double_mem_mgr = other._double_mem_mgr;
    this->_float_mem_mgr = other._float_mem_mgr;
    this->_int64_mem_mgr = other._int64_mem_mgr;
    this->_uint64_mem_mgr = other._uint64_mem_mgr;
    this->_int32_mem_mgr = other._int32_mem_mgr;
    this->_uint32_mem_mgr = other._uint32_mem_mgr;
    this->_str_len_mem_mgr = other._str_len_mem_mgr;
}

// Add metadata scalar field (non-string) with value. If the field does not
// exist, it will be created. If the field exists, the value will be appended
// to existing field.
void MetaData::add_scalar(const std::string& field_name,
                          const void* value,
                          MetaDataType type)

{
    // Create a field for the scalar if needed
    if (!this->has_field(field_name)) {
         this->_create_field(field_name, type);
    }

    // Get the field
    MetadataField* mdf = this->_field_map[field_name];
    if (mdf == NULL) {
        throw std::runtime_error("Metadata field was not found");
    }

    // Get its type
    MetaDataType existing_type = mdf->type();
    if (existing_type != type) {
        throw std::runtime_error("The existing metadata field "\
                                 "has a different type. ");
    }

    // Add the value
    switch (type) {
        case MetaDataType::dbl :
            ((ScalarField<double>*)(mdf))->append(value);
            break;
        case MetaDataType::flt :
            ((ScalarField<float>*)(mdf))->append(value);
            break;
        case MetaDataType::int64 :
            ((ScalarField<int64_t>*)(mdf))->append(value);
            break;
        case MetaDataType::uint64 :
            ((ScalarField<uint64_t>*)(mdf))->append(value);
            break;
        case MetaDataType::int32 :
            ((ScalarField<int32_t>*)(mdf))->append(value);
            break;
        case MetaDataType::uint32 :
            ((ScalarField<uint32_t>*)(mdf))->append(value);
            break;
        case MetaDataType::string :
            throw std::runtime_error("Invalid MetaDataType used in "\
                                     "MetaData.add_scalar().");
    }
}

// Add string to a metadata field. If the field does not exist, it will be created.
// If the field exists, the value will be appended to existing field.
void MetaData::add_string(const std::string& field_name, const std::string& value)
{
    // Create the field if this will be the first string for it
    if (!this->has_field(field_name))
         this->_create_field(field_name, MetaDataType::string);

    // Get the field
    MetadataField* mdf = this->_field_map[field_name];
    if (mdf == NULL) {
        throw std::runtime_error("Internal error: Metadata field was not found");
    }

    // Double-check its type
    if (mdf->type() != MetaDataType::string) {
        throw std::runtime_error("The metadata field isn't a string type.");
    }

    // Add the value
    ((StringField*)mdf)->append(value);
}

// Get metadata values from field that are scalars (non-string)
void MetaData::get_scalar_values(const std::string& name,
                                void*& data,
                                size_t& length,
                                MetaDataType& type)
{
    // Make sure the field exists
    if (this->_field_map[name] == NULL) {
        throw std::runtime_error("The metadata field " + name +
                                 " does not exist.");
    }

    // Get values for the field
    type = this->_field_map[name]->type();
    switch (type) {
        case MetaDataType::dbl :
            this->_get_numeric_field_values<double>
                (name, data, length, this->_double_mem_mgr);
            break;
        case MetaDataType::flt :
            this->_get_numeric_field_values<float>
                (name, data, length, this->_float_mem_mgr);
            break;
        case MetaDataType::int64 :
            this->_get_numeric_field_values<int64_t>
                (name, data, length, this->_int64_mem_mgr);
            break;
        case MetaDataType::uint64 :
            this->_get_numeric_field_values<uint64_t>
                (name, data, length, this->_uint64_mem_mgr);
            break;
        case MetaDataType::int32 :
            this->_get_numeric_field_values<int32_t>
                (name, data, length, this->_int32_mem_mgr);
            break;
        case MetaDataType::uint32 :
            this->_get_numeric_field_values<uint32_t>
                (name, data, length, this->_uint32_mem_mgr);
            break;
        case MetaDataType::string :
            throw std::runtime_error("MetaData.get_scalar_values() "\
                                     "requested invalid MetaDataType.");
            break;
        default:
            throw std::runtime_error("MetaData.get_scalar_values() "\
                                     "requested unknown MetaDataType.");
            break;
    }
}

// Get metadata string field using a c-style interface.
void MetaData::get_string_values(const std::string& name,
                                 char**& data,
                                 size_t& n_strings,
                                 size_t*& lengths)

{
    // Retrieve the strings
    std::vector<std::string> field_strings = this->get_string_values(name);

    // Allocate space to copy the strings
    n_strings = 0; // Set to zero until all data copied
    data = this->_char_array_mem_mgr.allocate(field_strings.size());
    if (data == NULL)
        throw std::bad_alloc();
    lengths = this->_str_len_mem_mgr.allocate(field_strings.size());
    if (lengths == NULL)
        throw std::bad_alloc();

    // Copy each metadata string into the string buffer
    for (int i = 0; i < field_strings.size(); i++) {
        size_t size = field_strings[i].size();
        char* cstr = this->_char_mem_mgr.allocate(size + 1);
        if (cstr == NULL)
            throw std::bad_alloc();
        field_strings[i].copy(cstr, size, 0);
        cstr[size] = '\0';
        data[i] = cstr;
        lengths[i] = size;
    }

    // Write down the number of strings copied
    n_strings = field_strings.size();
}

// Get metadata values string field
std::vector<std::string>
MetaData::get_string_values(const std::string& name)
{
    // Get the field
    MetadataField* mdf = this->_field_map[name];
    if (mdf == NULL) {
        throw std::runtime_error("The metadata field " + name +
                                 " does not exist.");
    }

    // Double-check its type
    if (mdf->type() != MetaDataType::string) {
        throw std::runtime_error("The metadata field " + name +
                                 " is not a string field.");
    }

    // Return the values
    return ((StringField*)mdf)->values();
}

// This function checks if the DataSet has a field
bool MetaData::has_field(const std::string& field_name)
{
    return (this->_field_map.count(field_name) > 0);
}

// Clear all entries in a DataSet field.
void MetaData::clear_field(const std::string& field_name)
{
    if (this->has_field(field_name)) {
        this->_field_map[field_name]->clear();
        delete this->_field_map[field_name]; // ***WS*** FINDME: is this the appropriate cleanup for the allocator used?
        this->_field_map.erase(field_name);
    }
}

// Create a new metadata field with the given name and type.
void MetaData::_create_field(const std::string& field_name,
                             const MetaDataType type)
{
    switch(type) {
        case MetaDataType::string :
            this->_create_string_field(field_name);
            break;
        case MetaDataType::dbl :
            this->_create_scalar_field<double>(field_name,type);
            break;
        case MetaDataType::flt :
            this->_create_scalar_field<float>(field_name,type);
            break;
        case MetaDataType::int64 :
            this->_create_scalar_field<int64_t>(field_name,type);
            break;
        case MetaDataType::uint64 :
            this->_create_scalar_field<uint64_t>(field_name,type);
            break;
        case MetaDataType::int32 :
            this->_create_scalar_field<int32_t>(field_name,type);
            break;
        case MetaDataType::uint32 :
            this->_create_scalar_field<uint32_t>(field_name,type);
            break;
        default:
            throw std::runtime_error("Unknown field type in _create_field");
    }
}

// Perform a deep copy assignment of a scalar or string field.
void MetaData::_deep_copy_field(MetadataField* dest_field,
                                MetadataField* src_field)
{
    MetaDataType type = src_field->type();
    switch (type) {
        case MetaDataType::string :
            *((StringField*)dest_field) =
                *((StringField*)src_field);
            break;
        case MetaDataType::dbl :
            *((ScalarField<double>*)dest_field) =
                *((ScalarField<double>*)src_field);
            break;
        case MetaDataType::flt :
            *((ScalarField<float>*)dest_field) =
                *((ScalarField<float>*)src_field);
            break;
        case MetaDataType::int64 :
            *((ScalarField<int64_t>*)dest_field) =
                *((ScalarField<int64_t>*)src_field);
            break;
        case MetaDataType::uint64 :
            *((ScalarField<uint64_t>*)dest_field) =
                *((ScalarField<uint64_t>*)src_field);
            break;
        case MetaDataType::int32 :
            *((ScalarField<int32_t>*)dest_field) =
                *((ScalarField<int32_t>*)src_field);
            break;
        case MetaDataType::uint32 :
            *((ScalarField<uint32_t>*)dest_field) =
                *((ScalarField<uint32_t>*)src_field);
            break;
        default:
            throw std::runtime_error("Unknown field type in _deep_copy_field");
    }
}

// Create a new scalar metadata field and add it to the field map.
template <typename T>
void MetaData::_create_scalar_field(const std::string& field_name,
                                    const MetaDataType type)
{
    MetadataField* mdf = new ScalarField<T>(field_name, type);
    if (mdf != NULL)
        this->_field_map[field_name] = mdf;
}

// Create a new string metadata field and add it to the field map
void MetaData::_create_string_field(const std::string& field_name)
{
    MetadataField* mdf = new StringField(field_name);
    if (mdf != NULL)
        this->_field_map[field_name] = mdf;
}

// Allocate new memory to hold metadata field values and return these values
// via the c-ptr reference being pointed to the newly allocated memory
template <typename T>
void MetaData::_get_numeric_field_values(const std::string& name,
                                         void*& data,
                                         size_t& n_values,
                                         SharedMemoryList<T>& mem_list)
{
    // Make sure the field exists
    MetadataField* mdf = this->_field_map[name];
    if (mdf == NULL) {
        throw std::runtime_error("Field " + name + " does not exist.");
    }

    // Perform type-specific allocation
    switch (mdf->type()) {
        case MetaDataType::dbl : {
            ScalarField<double>* sdf = (ScalarField<double>*)mdf;
            n_values = sdf->size();
            data = (void*)mem_list.allocate(n_values);
            if (data == NULL)
                throw std::bad_alloc();
            std::memcpy(data, sdf->data(), n_values * sizeof(T));
            }
            break;
        case MetaDataType::flt : {
            ScalarField<float>* sdf = (ScalarField<float>*)mdf;
            n_values = sdf->size();
            data = (void*)mem_list.allocate(n_values);
            if (data == NULL)
                throw std::bad_alloc();
            std::memcpy(data, sdf->data(), n_values*sizeof(T));
            }
            break;
        case MetaDataType::int64 : {
            ScalarField<int64_t>* sdf = (ScalarField<int64_t>*)mdf;
            n_values = sdf->size();
            data = (void*)mem_list.allocate(n_values);
            if (data == NULL)
                throw std::bad_alloc();
            std::memcpy(data, sdf->data(), n_values*sizeof(T));
            }
            break;
        case MetaDataType::uint64 : {
            ScalarField<uint64_t>* sdf = (ScalarField<uint64_t>*)mdf;
            n_values = sdf->size();
            data = (void*)mem_list.allocate(n_values);
            if (data == NULL)
                throw std::bad_alloc();
            std::memcpy(data, sdf->data(), n_values*sizeof(T));
            }
            break;
        case MetaDataType::int32 : {
            ScalarField<int32_t>* sdf = (ScalarField<int32_t>*)mdf;
            n_values = sdf->size();
            data = (void*)mem_list.allocate(n_values);
            if (data == NULL)
                throw std::bad_alloc();
            std::memcpy(data, sdf->data(), n_values*sizeof(T));
            }
            break;
        case MetaDataType::uint32 : {
            ScalarField<uint32_t>* sdf = (ScalarField<uint32_t>*)mdf;
            n_values = sdf->size();
            data = (void*)mem_list.allocate(n_values);
            if (data == NULL)
                throw std::bad_alloc();
            std::memcpy(data, sdf->data(), n_values*sizeof(T));
            }
            break;
        case MetaDataType::string :
            throw std::runtime_error("Invalid MetaDataType used in "\
                                     "MetaData.add_scalar().");
            break;
        default:
            throw std::runtime_error("Unknown MetaDataType found in "\
                                     "MetaData.add_scalar().");

    }
}

// Retrieve a vector of std::pair with the field name and the field serialization 
// for all fields in the MetaData set.
std::vector<std::pair<std::string, std::string>>
    MetaData::get_metadata_serialization_map()
{
    std::unordered_map<std::string, MetadataField*>::iterator
        mdf_it = this->_field_map.begin();
    std::vector<std::pair<std::string, std::string>> fields;
    for ( ; mdf_it != this->_field_map.end(); mdf_it++) {
        fields.push_back({mdf_it->first,mdf_it->second->serialize()});
    }

    return fields;
}

// Add a serialized field to the MetaData object
void MetaData::add_serialized_field(const std::string& name,
                                    char* buf,
                                    size_t buf_size)
{
    // Sanity check
    if (buf == NULL)
        throw std::runtime_error("invalid buffer supplied");

    // Determine the type of the serialized data
    std::string_view buf_sv(buf, buf_size);
    MetaDataType type = MetadataBuffer::get_type(buf_sv);

    // Make sure we don't already have a field with this name
    if (this->has_field(name))
        throw std::runtime_error("Cannot add serialized field if "\
                                 "already exists.");

    // Allocate memory for the field
    MetadataField* mdf = NULL;
    switch (type) {
        case MetaDataType::dbl :
            mdf = new ScalarField<double>(
                name, MetaDataType::dbl,
                MetadataBuffer::unpack_scalar_buf<double>(buf_sv));
            break;
        case MetaDataType::flt :
            mdf = new ScalarField<float>(
                name, MetaDataType::flt,
                MetadataBuffer::unpack_scalar_buf<float>(buf_sv));
            break;
        case MetaDataType::int64 :
            mdf = new ScalarField<int64_t>(
                name, MetaDataType::int64,
                MetadataBuffer::unpack_scalar_buf<int64_t>(buf_sv));
            break;
        case MetaDataType::uint64 :
            mdf = new ScalarField<uint64_t>(
                name, MetaDataType::uint64,
                MetadataBuffer::unpack_scalar_buf<uint64_t>(buf_sv));
            break;
        case MetaDataType::int32 :
            mdf = new ScalarField<int32_t>(
                name, MetaDataType::int32,
                MetadataBuffer::unpack_scalar_buf<int32_t>(buf_sv));
            break;
        case MetaDataType::uint32 :
            mdf = new ScalarField<uint32_t>(
                name, MetaDataType::uint32,
                MetadataBuffer::unpack_scalar_buf<uint32_t>(buf_sv));
            break;
        case MetaDataType::string :
            mdf = new StringField(
                name, MetadataBuffer::unpack_string_buf(buf_sv));
            break;
        default:
            throw std::runtime_error("unknown type in add_serialized_field");
    }

    // Add the field
    if (mdf == NULL)
        throw std::bad_alloc();
    this->_field_map[name] = mdf;
}

// Delete the memory associated with all fields and clear inventory
void MetaData::_delete_fields()
{
    std::unordered_map<std::string, MetadataField*>::iterator it =
        this->_field_map.begin();
    for ( ; it != this->_field_map.end(); it++) {
        delete it->second;
        it->second = NULL;
    }
    this->_field_map.clear();
}

// EOF
