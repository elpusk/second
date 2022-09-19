#pragma once

namespace ns_std
{
	struct arrayDelete
	{
		template< typename T >
		void operator()( T *p )
		{
			delete [] p;
		}
	};

	struct emptyDelete
	{
		template< typename T >
		void operator()( T *p )
		{
		}
	};

}

