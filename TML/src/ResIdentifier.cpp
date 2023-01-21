#include <TML/ResIdentifier.hpp>

namespace Torch
{

    ResIdentifier::ResIdentifier(std::string_view name, uint64_t timestamp, std::shared_ptr<std::istream> const &is)
        : ResIdentifier{std::move(name), timestamp, is, std::shared_ptr<std::streambuf>()}
    {
    }

    ResIdentifier::ResIdentifier(std::string_view name, uint64_t timestamp, std::shared_ptr<std::istream> const &is, std::shared_ptr<std::streambuf> const &streambuf)
        : _res_name{std::move(name)}
        , _time_stamp{timestamp}
        , _istream{is}
        , _streambuf{streambuf}
        , _uid{""}
        , Name{this, &ResIdentifier::_getName, &ResIdentifier::_setName}
        , Guid{this, &ResIdentifier::_getGuid, &ResIdentifier::_setGuid}
        , Timestamp{this, &ResIdentifier::_getTimestamp, &ResIdentifier::_setTimestamp}
        , Count{this, &ResIdentifier::_getCount}
    {
    }

    void ResIdentifier::Read(void *p, size_t size)
    {
        _istream->read(static_cast<char*>(p), static_cast<std::streamsize>(size));
    }

    void ResIdentifier::Seek(int64_t offset, std::ios_base::seekdir way)
    {
        _istream->seekg(static_cast<std::istream::off_type>(offset), way);
    }

    int64_t ResIdentifier::Tell()
    {
        return static_cast<int64_t>(_istream->tellg());
    }
    void ResIdentifier::Clear()
    {
        _istream->clear();
    }

    ResIdentifier::operator bool() const
    {
        return !_istream->fail();
    }

    bool ResIdentifier::operator!() const
    {
        return _istream->operator!();
    }

    std::istream &ResIdentifier::InputStream()
    {
        return *_istream;
    }

    void ResIdentifier::_setName(const std::string name)
    {
        _res_name = name;
    }

    std::string ResIdentifier::_getName() const
    {
        return _res_name;
    }

    void ResIdentifier::_setGuid(const std::string guid)
    {
        _uid = guid;
    }

    std::string ResIdentifier::_getGuid() const
    {
        return _uid;
    }

    void ResIdentifier::_setTimestamp(uint64_t ts)
    {
        _time_stamp = ts;
    }

    uint64_t ResIdentifier::_getTimestamp() const
    {
        return _time_stamp;
    }

    int64_t ResIdentifier::_getCount() const
    {
        return static_cast<int64_t>(_istream->gcount());
    }
};