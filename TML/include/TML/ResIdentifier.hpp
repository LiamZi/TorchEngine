#ifndef __TORHC_TML_RES_IDENTIFIER_HPP__
#define __TORHC_TML_RES_IDENTIFIER_HPP__

#pragma once

#include <TML/Util.hpp>
#include <istream>
#include <string>
#include <string_view>
#include <vector>


namespace Torch
{
	class ResIdentifier final
	{
	private:
		std::string _res_name;
		uint64_t _time_stamp;
		std::shared_ptr<std::istream> _istream;
		std::shared_ptr<std::streambuf> _streambuf;
		std::string _uid;

	public:
		ResIdentifier(std::string_view name, uint64_t timestamp, std::shared_ptr<std::istream> const& is);
		ResIdentifier(std::string_view name, uint64_t timestamp, std::shared_ptr<std::istream> const &is, std::shared_ptr<std::streambuf> const &streambuf);
	
		void Read(void *p, size_t size); 
		void Seek(int64_t offset, std::ios_base::seekdir way);
		int64_t Tell();
		void Clear();

		operator bool() const;
		bool operator!() const;
		std::istream &InputStream();

	public:
		Property<ResIdentifier, std::string> Name;
		Property<ResIdentifier, std::string> Guid;
		Property<ResIdentifier, uint64_t> Timestamp;
		ReadOnlyProperty<ResIdentifier, int64_t> Count;


	private:
		void _setName(const std::string name);
		std::string _getName() const; 
		void _setGuid(const std::string guid);
		std::string _getGuid() const;
		void _setTimestamp(uint64_t ts);
		uint64_t _getTimestamp() const; 
		int64_t _getCount() const;
	};
};

#endif // !__TORHC_TML_RES_IDENTIFIER_HPP__
