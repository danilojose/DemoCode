#pragma once
#include <System\StdLibraries.h>
namespace GameSystem
{
class HashedString
{
public:
	/// <summary>
	/// Initializes a new instance of the <see cref="HashedString"/> class.
	/// </summary>
	/// <param name="pIdentString">The p ident string.</param>
	explicit HashedString(char const * const pIdentString)
		: m_ident(hash_name(pIdentString))
		, m_identStr(pIdentString)
	{
	}

	/// <summary>
	/// Gets the hash value.
	/// </summary>
	/// <returns></returns>
	unsigned long getHashValue(void) const
	{

		return reinterpret_cast<unsigned long>(m_ident);
	}

	/// <summary>
	/// Gets the string.
	/// </summary>
	/// <returns></returns>
	const std::string & getStr() const
	{
		return m_identStr;
	}

	static
		/// <summary>
		/// Hash_names the specified p ident string.
		/// </summary>
		/// <param name="pIdentStr">The p ident string.</param>
		/// <returns></returns>
		void * hash_name(char const *  pIdentStr);

	/// <summary>
	/// Operators the specified o.
	/// </summary>
	/// <param name="o">The o.</param>
	/// <returns></returns>
	bool operator< (HashedString const & o) const
	{
		bool r = (getHashValue() < o.getHashValue());
		return r;
	}

	/// <summary>
	/// Operator==s the specified o.
	/// </summary>
	/// <param name="o">The o.</param>
	/// <returns></returns>
	bool operator== (HashedString const & o) const
	{
		bool r = (getHashValue() == o.getHashValue());
		return r;
	}

private:

	// note: m_ident is stored as a void* not an int, so that in
	// the debugger it will show up as hex-values instead of
	// integer values. This is a bit more representative of what
	// we're doing here and makes it easy to allow external code
	// to assign event types as desired.

	void *             m_ident;
	std::string		   m_identStr;
};

}