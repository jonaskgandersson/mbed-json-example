#include "mbed.h"
#include "jWrite.h"

void jWriteTest();

int main()
{
    jWriteTest();
	return 0;
}

// Examples of jWrite
// - using global control structure
//
void jWriteTest()
{
	char buffer[1024];
	unsigned int buflen= 1024;
	int err;

	printf("A JSON object example:\n\n" );

	jwOpen( buffer, buflen, JW_OBJECT, JW_PRETTY );		// start root object

	jwObj_string( "key", "value" );				// add object key:value pairs
	jwObj_int( "int", 1 );
	jwObj_double( "double", 1.234 );
	jwObj_null( "nullThing" );
	jwObj_bool( "bool", 1 );
	jwObj_array( "EmptyArray" );
		// empty array
	jwEnd();
	jwObj_array( "anArray" );					// array with elements
		jwArr_string("array one" );
		jwArr_int( -2 );
		jwArr_double( 1234.567 );
		jwArr_null();
		jwArr_bool( 0 );
		jwArr_object();							// object in array
			jwObj_string( "obj3_one", "one");
			jwObj_string( "obj3_two", "two");
		jwEnd();
		jwArr_array();							// array in array
			jwArr_int( 0 );
			jwArr_int( 1 );
			jwArr_int( 2 );
		jwEnd();
	jwEnd();									// end of "anArray" , back to root object

	jwObj_object( "EmptyObject" );
	jwEnd();

	jwObj_object( "anObject" );					// object in the root object
		jwObj_string("msg","object in object");
		jwObj_string("msg2","object in object 2nd entry");
	jwEnd();
	jwObj_string( "ObjEntry", "This is the last one" );

	err= jwClose();								// close and get error code

	printf( buffer );
	if( err != JWRITE_OK )
		printf( "Error: %s at function call %d\n", jwErrorToString(err), jwErrorPos() );

	printf("\n\nA JSON array example:\n\n" );

	jwOpen( buffer, buflen, JW_ARRAY, JW_PRETTY );
	jwArr_string( "String value" );
	jwArr_int( 1234 );
	jwArr_double( 567.89012 );
	jwArr_bool( 1 );
	jwArr_null();
	jwArr_object();		
		// empty object
	jwEnd();
	jwArr_object();
		jwObj_string( "key", "value" );
		jwObj_string( "key2", "value2" );
	jwEnd();
	jwArr_array();		// array in array
		jwArr_string("Array in array");
		jwArr_string("the end");
	jwEnd();
	jwArr_string("Empty array next...");
	jwArr_array();
		// empty array
	jwEnd();
	err= jwClose();

	printf( buffer );
	if( err != JWRITE_OK )
		printf( "Error: %s at function call %d\n", jwErrorToString(err), jwErrorPos() );

	printf("\n\nExample error:\n\n" );
	// this is a copy of the above array example with an error introduced...
	//
	jwOpen( buffer, buflen, JW_ARRAY, JW_PRETTY );	// 1
	jwArr_string( "String value" );					// 2
	jwArr_int( 1234 );								// 3
	jwArr_double( 567.89012 );						// 4
	jwArr_bool( 1 );								// 5
	jwArr_null();									// 6
	jwArr_object();									// 7
		// empty object
	//jwEnd();
	jwArr_object();									// 8  <-- this is where the error is
		jwObj_string( "key", "value" );				// 9
		jwObj_string( "key2", "value2" );			// 10
	jwEnd();										// 11 
	jwArr_array();		// array in array			// 12
		jwArr_string("Array in array");				// 13
		jwArr_string("the end");					// 14
	jwEnd();										// 15
	jwArr_string("Empty array next...");			// 16
	jwArr_array();									// 17
		// empty array
	jwEnd();										// 18
	err= jwClose();									

	printf( buffer );
	if( err != JWRITE_OK )
		printf( "Error: %s at function call %d\n", jwErrorToString(err), jwErrorPos() );

	return;
}