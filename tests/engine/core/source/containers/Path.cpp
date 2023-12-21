
#include <gtest/gtest.h>

#include <TestsCommon.hpp>
#include <core/containers/Path.hpp>



TEST( CoreContainerPath, BasicInitialize )
{
	using A = qup::Path32;

	// absolute path
	#if QUP_PLATFORM_WINDOWS

	A a;
	EXPECT_EQ( a.Size(), 0 );

	a = U"C:/testing1/testing2";
	EXPECT_EQ( a.Size(), 3 );

	a = U"C:\\testing1\\testing2";
	EXPECT_EQ( a.Size(), 3 );

	A b( U"C:/testing1/testing2" );
	EXPECT_EQ( b.Size(), 3 );

	#elif QUP_PLATFORM_LINUX || QUP_PLATFORM_APPLE

	A a;
	EXPECT_EQ( a.Size(), 0 );

	a = U"/testing1/testing2";
	EXPECT_EQ( a.Size(), 3 );

	a = U"\\testing1\\testing2";
	EXPECT_EQ( a.Size(), 3 );

	A b( U"/testing1/testing2" );
	EXPECT_EQ( b.Size(), 3 );

	#endif

	// relative path
	A c;
	EXPECT_EQ( c.Size(), 0 );

	c = U"testing1/testing2";
	EXPECT_EQ( c.Size(), 2 );
};

TEST( CoreContainerPath, Parsing )
{
	using A = qup::Path32;
	{
		A a = U"";
		EXPECT_EQ( a.Size(), 0 );

		a = U"t1";
		EXPECT_EQ( a.Size(), 1 );
		EXPECT_EQ( a, U"t1" );

		a = U"t1/t2";
		EXPECT_EQ( a.Size(), 2 );
		EXPECT_EQ( a, U"t1/t2" );

		a = U"t1/t2/t3";
		EXPECT_EQ( a.Size(), 3 );
		EXPECT_EQ( a, U"t1/t2/t3" );

		a = U"t1/t2/t3/t4/..";
		EXPECT_EQ( a.Size(), 3 );
		EXPECT_EQ( a, U"t1/t2/t3" );

		a = U"t1/t2/t3/../t4";
		EXPECT_EQ( a.Size(), 3 );
		EXPECT_EQ( a, U"t1/t2/t4" );

		a = U"t1/t2/../t3/t4";
		EXPECT_EQ( a.Size(), 3 );
		EXPECT_EQ( a, U"t1/t3/t4" );

		a = U"t1/../t2/t3/t4";
		EXPECT_EQ( a.Size(), 3 );
		EXPECT_EQ( a, U"t2/t3/t4" );

		a = U"../t1/t2/t3/t4";
		EXPECT_EQ( a.Size(), 5 );
		EXPECT_EQ( a, U"../t1/t2/t3/t4" );

		a = U"../t1/t2/t3/t4/..";
		EXPECT_EQ( a.Size(), 4 );
		EXPECT_EQ( a, U"../t1/t2/t3" );

		a = U"../t1/t2/t3/../t4";
		EXPECT_EQ( a.Size(), 4 );
		EXPECT_EQ( a, U"../t1/t2/t4" );

		a = U"../t1/t2/../t3/t4";
		EXPECT_EQ( a.Size(), 4 );
		EXPECT_EQ( a, U"../t1/t3/t4" );

		a = U"../t1/../t2/t3/t4";
		EXPECT_EQ( a.Size(), 4 );
		EXPECT_EQ( a, U"../t2/t3/t4" );

		a = U"t1/t2/t3/../..";
		EXPECT_EQ( a.Size(), 1 );
		EXPECT_EQ( a, U"t1" );

		a = U"t1/t2/t3/../../..";
		EXPECT_EQ( a.Size(), 0 );
		EXPECT_EQ( a, U"" );

		a = U"t1/t2/t3/../../../..";
		EXPECT_EQ( a.Size(), 1 );
		EXPECT_EQ( a, U".." );

		a = U"t1/../t3";
		EXPECT_EQ( a.Size(), 1 );
		EXPECT_EQ( a, U"t3" );

		a = U"t1/../t3/..";
		EXPECT_EQ( a.Size(), 0 );
		EXPECT_EQ( a, U"" );

		a = U"t1/../t3/../..";
		EXPECT_EQ( a.Size(), 1 );
		EXPECT_EQ( a, U".." );
	}
}

TEST( CoreContainerPath, IsRelative )
{
	using A = qup::Path32;

	#if QUP_PLATFORM_WINDOWS

	// absolute path
	A a = U"C:/testing1/testing2";
	EXPECT_EQ( a.IsRelative(), false );

	// relative path
	A b = U"testing1/testing2";
	EXPECT_EQ( b.IsRelative(), true );

	#elif QUP_PLATFORM_LINUX || QUP_PLATFORM_APPLE

	// absolute path
	A a = U"/testing1/testing2";
	EXPECT_EQ( a.IsRelative(), false );

	// relative path
	A b = U"testing1/testing2";
	EXPECT_EQ( b.IsRelative(), true );

	#endif
};

TEST( CoreContainerPath, OperatorIndex )
{
	using A = qup::Path32;

	#if QUP_PLATFORM_WINDOWS

	{
		A a = U"C:/testing1/testing2";
		EXPECT_EQ( a[ 0 ], U"C:" );
		EXPECT_EQ( a[ 1 ], U"testing1" );
		EXPECT_EQ( a[ 2 ], U"testing2" );

		EXPECT_THROW( a[ 3 ], qup::exception::Exception );
	}

	#elif QUP_PLATFORM_LINUX || QUP_PLATFORM_APPLE

	{
		A a = U"/testing1/testing2";
		EXPECT_EQ( a[ 0 ], U"" );
		EXPECT_EQ( a[ 1 ], U"testing1" );
		EXPECT_EQ( a[ 2 ], U"testing2" );

		EXPECT_THROW( a[ 3 ], qup::exception::Exception );
	}

	#endif

	{
		A a = U"testing1/testing2";
		EXPECT_EQ( a[ 0 ], U"testing1" );
		EXPECT_EQ( a[ 1 ], U"testing2" );

		EXPECT_THROW( a[ 2 ], qup::exception::Exception );
	}
};

TEST( CoreContainerPath, ToText )
{
	using A = qup::Path32;

	{
		A a;
		a.PushBack( U"t1" );
		EXPECT_EQ( a, U"t1" );

		a.PushBack( U"t2" );
		EXPECT_EQ( a, U"t1/t2" );

		a.PushBack( U"t3" );
		EXPECT_EQ( a, U"t1/t2/t3" );

		a.Erase( 1 );
		EXPECT_EQ( a, U"t1/t3" );

		a.PushFront( U"t4" );
		EXPECT_EQ( a, U"t4/t1/t3" );

		a.PushFront( U"" );
		EXPECT_EQ( a, U"/t4/t1/t3" );
	}
};

TEST( CoreContainerPath, Replace )
{
	using A = qup::Path32;

	#if QUP_PLATFORM_WINDOWS

	{
		A a( U"C:\\testing1\\testing2" );
		a.Replace( 1, U"test" );
		EXPECT_EQ( a, U"C:/test/testing2" );

		a.Replace( 2, U"testing5" );
		EXPECT_EQ( a, U"C:/test/testing5" );

		a.Replace( 0, U"root" );
		EXPECT_EQ( a, U"root/test/testing5" );

		EXPECT_THROW( a.Replace( 3, U"non_existant" ), qup::exception::Exception );
	}

	#elif QUP_PLATFORM_LINUX || QUP_PLATFORM_APPLE

	{
		A a( U"/testing1/testing2" );
		a.Replace( 1, U"test" );
		EXPECT_EQ( a, U"/test/testing2" );

		a.Replace( 2, U"testing5" );
		EXPECT_EQ( a, U"/test/testing5" );

		a.Replace( 0, U"root" );
		EXPECT_EQ( a, U"root/test/testing5" );

		EXPECT_THROW( a.Replace( 3, U"non_existant" ), qup::exception::Exception );
	}

	#endif

	{
		A a( U"testing1/testing2" );
		a.Replace( 1, U"test" );
		EXPECT_EQ( a, U"testing1/test");

		a.Replace( 0, U"new_first" );
		EXPECT_EQ( a, U"new_first/test" );

		a.Replace( 0, U"same_size" );
		EXPECT_EQ( a, U"same_size/test" );

		EXPECT_THROW( a.Replace( 2, U"non_existant" ), qup::exception::Exception );
	}

	#if QUP_PLATFORM_WINDOWS

	{
		A a( U"C:\\testing1\\testing2" );
		a.Replace( U"testing1", U"test" );
		EXPECT_EQ( a, U"C:/test/testing2" );

		a.Replace( U"testing2", U"testing5" );
		EXPECT_EQ( a, U"C:/test/testing5" );

		a.Replace( U"", U"non_existent" );
		EXPECT_EQ( a, U"C:/test/testing5" );
	}

	#elif QUP_PLATFORM_LINUX || QUP_PLATFORM_APPLE

	{
		A a( U"/testing1/testing2" );
		a.Replace( U"testing1", U"test" );
		EXPECT_EQ( a, U"/test/testing2" );

		a.Replace( U"testing2", U"testing5" );
		EXPECT_EQ( a, U"/test/testing5" );

		a.Replace( U"", U"non_existent" );
		EXPECT_EQ( a, U"/test/testing5" );
	}

	#endif

	{
		A a( U"testing1/testing2" );
		a.Replace( U"testing1", U"test" );
		EXPECT_EQ( a, U"test/testing2" );

		a.Replace( U"test", U"new_first" );
		EXPECT_EQ( a, U"new_first/testing2" );

		a.Replace( U"new_first", U"same_size" );
		EXPECT_EQ( a, U"same_size/testing2" );

		a.Replace( U"", U"non_existant" );
		EXPECT_EQ( a, U"same_size/testing2" );
	}
	{
		A a( U"same_name/same_name" );
		a.Replace( U"same_name", U"different_name" );
		EXPECT_EQ( a, U"different_name/same_name" );

		a.Replace( U"same_name", U"different_name" );
		EXPECT_EQ( a, U"different_name/different_name" );

		a.Replace( U"same_name", U"different_name" );
		EXPECT_EQ( a, U"different_name/different_name" );
	}
};

TEST( CoreContainerPath, ExtraSeparators )
{
	using A = qup::Path32;
	{
		A a;
		a = U"double_slashes//here";
		EXPECT_EQ( a.Size(), 2 );
		EXPECT_EQ( a[ 0 ], U"double_slashes" );
		EXPECT_EQ( a[ 1 ], U"here" );
	}

	#if QUP_PLATFORM_WINDOWS

	{
		A a;
		a = U"C:/double_slashes//here";
		EXPECT_EQ( a.Size(), 3 );
		EXPECT_EQ( a[ 0 ], U"C:" );
		EXPECT_EQ( a[ 1 ], U"double_slashes" );
		EXPECT_EQ( a[ 2 ], U"here" );
	}
	{
		A a;
		a = U"C://double_slashes/here";
		EXPECT_EQ( a.Size(), 3 );
		EXPECT_EQ( a[ 0 ], U"C:" );
		EXPECT_EQ( a[ 1 ], U"double_slashes" );
		EXPECT_EQ( a[ 2 ], U"here" );
	}

	#elif QUP_PLATFORM_LINUX || QUP_PLATFORM_APPLE

	{
		A a;
		a = U"/double_slashes//here";
		EXPECT_EQ( a.Size(), 3 );
		EXPECT_EQ( a[ 0 ], U"" );
		EXPECT_EQ( a[ 1 ], U"double_slashes" );
		EXPECT_EQ( a[ 2 ], U"here" );
	}
	{
		A a;
		a = U"//double_slashes/here";
		EXPECT_EQ( a.Size(), 3 );
		EXPECT_EQ( a[ 0 ], U"" );
		EXPECT_EQ( a[ 1 ], U"double_slashes" );
		EXPECT_EQ( a[ 2 ], U"here" );
	}

	#endif

	{
		A a;
		a = U"trailing/separators/ignored/";
		EXPECT_EQ( a.Size(), 3 );
		EXPECT_EQ( a[ 0 ], U"trailing" );
		EXPECT_EQ( a[ 1 ], U"separators" );
		EXPECT_EQ( a[ 2 ], U"ignored" );
	}
};

TEST( CoreContainerPath, GetCommonParent )
{
	using A = qup::Path32;

	{
		A a = U"p1/p2/p3";
		A b = U"p1/p2/p3";
		EXPECT_EQ( a.GetCommonParent( b ), U"p1/p2/p3" );
	}
	{
		A a = U"p1/p2/p3";
		A b = U"p1/p2/p4";
		EXPECT_EQ( a.GetCommonParent( b ), U"p1/p2" );
	}
	{
		A a = U"p1/p2/p3";
		A b = U"p1/p5/p4";
		EXPECT_EQ( a.GetCommonParent( b ), U"p1" );
	}
	{
		A a = U"";
		A b = U"p1/p2/p4";
		EXPECT_EQ( a.GetCommonParent( b ), U"" );
	}
	{
		A a = U"p1/p2/p3";
		A b = U"";
		EXPECT_EQ( a.GetCommonParent( b ), U"" );
	}
	{
		A a = U"";
		A b = U"";
		EXPECT_EQ( a.GetCommonParent( b ), U"" );
	}
};

TEST( CoreContainerPath, RelativePath )
{
	using A = qup::Path32;

	{
		A a = U"p1/p2";
		A b = U"p1/p2/p10";
		A c = a.GetRelativePath( b );
		EXPECT_EQ( c.Size(), 1 );
		EXPECT_EQ( c, U"p10" );
	}
	{
		A a = U"p1/p2/p3";
		A b = U"p1/p2";
		A c = a.GetRelativePath( b );
		EXPECT_EQ( c.Size(), 1 );
		EXPECT_EQ( c, U".." );
	}
	{
		A a = U"p1/p2/p3";
		A b = U"p1/p2/p10";
		A c = a.GetRelativePath( b );
		EXPECT_EQ( c.Size(), 2 );
		EXPECT_EQ( c, U"../p10" );
	}
	{
		A a = U"";
		A b = U"p1/p2/p10";
		A c = a.GetRelativePath( b );
		EXPECT_EQ( c.Size(), 3 );
		EXPECT_EQ( c, U"p1/p2/p10" );
	}
	{
		A a = U"p1/p2/p10";
		A b = U"";
		A c = a.GetRelativePath( b );
		EXPECT_EQ( c.Size(), 3 );
		EXPECT_EQ( c, U"../../.." );
	}
	{
		A a = U"p1/p2/p10";
		A b = U"p1/p2/p10";
		A c = a.GetRelativePath( b );
		EXPECT_EQ( c.Size(), 0 );
		EXPECT_EQ( c, U"" );
	}
};

TEST( CoreContainerPath, GetStem )
{
	using A = qup::Path32;
	{
		A a = U"test_file.txt";
		EXPECT_EQ( a.GetStem(), U"test_file" );
	}
	{
		A a = U"test_file";
		EXPECT_EQ( a.GetStem(), U"test_file" );
	}
	{
		A a = U".test_file";
		EXPECT_EQ( a.GetStem(), U".test_file" );
	}
	{
		A a = U"t.test_file";
		EXPECT_EQ( a.GetStem(), U"t" );
	}
	{
		A a = U"";
		EXPECT_EQ( a.GetStem(), U"" );
	}

	{
		A a = U"folder/test_file.txt";
		EXPECT_EQ( a.GetStem(), U"test_file" );
	}
	{
		A a = U"folder/test_file";
		EXPECT_EQ( a.GetStem(), U"test_file" );
	}
	{
		A a = U"folder/.test_file";
		EXPECT_EQ( a.GetStem(), U".test_file" );
	}
	{
		A a = U"folder/t.test_file";
		EXPECT_EQ( a.GetStem(), U"t" );
	}
	{
		A a = U"folder/";
		EXPECT_EQ( a.GetStem(), U"folder" );
	}
};

TEST( CoreContainerPath, GetExtension )
{
	using A = qup::Path32;

	{
		A a = U"test_file.txt";
		EXPECT_EQ( a.GetExtension(), U".txt" );
	}
	{
		A a = U"test_file";
		EXPECT_EQ( a.GetExtension(), U"" );
	}
	{
		A a = U"test_file.";
		EXPECT_EQ( a.GetExtension(), U"." );
	}
	{
		A a = U".test_file";
		EXPECT_EQ( a.GetExtension(), U"" );
	}
	{
		A a = U"..test_file";
		EXPECT_EQ( a.GetExtension(), U".test_file" );
	}
	{
		A a = U"t.test_file";
		EXPECT_EQ( a.GetExtension(), U".test_file" );
	}
	{
		A a = U"";
		EXPECT_EQ( a.GetExtension(), U"" );
	}

	{
		A a = U"folder/test_file.txt";
		EXPECT_EQ( a.GetExtension(), U".txt" );
	}
	{
		A a = U"folder/test_file";
		EXPECT_EQ( a.GetExtension(), U"" );
	}
	{
		A a = U"folder/test_file.";
		EXPECT_EQ( a.GetExtension(), U"." );
	}
	{
		A a = U"folder/.test_file";
		EXPECT_EQ( a.GetExtension(), U"" );
	}
	{
		A a = U"folder/..test_file";
		EXPECT_EQ( a.GetExtension(), U".test_file" );
	}
	{
		A a = U"folder/t.test_file";
		EXPECT_EQ( a.GetExtension(), U".test_file" );
	}
	{
		A a = U"folder/";
		EXPECT_EQ( a.GetExtension(), U"" );
	}

	{
		A a = U"folder/folder_with.extension/test_file.txt";
		EXPECT_EQ( a.GetExtension(), U".txt" );
	}
	{
		A a = U"folder/folder_with.extension/test_file";
		EXPECT_EQ( a.GetExtension(), U"" );
	}
	{
		A a = U"folder/folder_with.extension/test_file.";
		EXPECT_EQ( a.GetExtension(), U"." );
	}
	{
		A a = U"folder/folder_with.extension/.test_file";
		EXPECT_EQ( a.GetExtension(), U"" );
	}
	{
		A a = U"folder/folder_with.extension/..test_file";
		EXPECT_EQ( a.GetExtension(), U".test_file" );
	}
	{
		A a = U"folder/folder_with.extension/t.test_file";
		EXPECT_EQ( a.GetExtension(), U".test_file" );
	}
	{
		A a = U"folder/folder_with.extension/";
		EXPECT_EQ( a.GetExtension(), U".extension" );
	}
};



/*
	#if QUP_PLATFORM_WINDOWS

	#elif QUP_PLATFORM_LINUX || QUP_PLATFORM_APPLE

	#endif
*/
