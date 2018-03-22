#include "mbed.h"

#include "jWrite.h"
#include "jRead.h"

// Color formated text and escapecharacters
#define FRONT_COLOR_BLACK   "\x1b[38;5;16m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_WHITE   "\x1b[37;1m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define BACK_COLOR_BLUE   "\x1b[44m"
#define BACK_COLOR_PEACH  "\x1b[48;5;208m"

#define CLEAR_CONSOLE      "\033[2J\033[1;1H"

// jWrite functions
void jWriteTest();

// jRead functions
void testQuery(const char *pJson, const char *query);
void runExamples();
void articleExample();
void jReadExample();

int main()
{
	printf(CLEAR_CONSOLE);
	printf(" +------------------------------------------------------------------+ \r\n");
	printf(" |  %-64s| \r\n", "Mbed json example using jWrite and jRead.");
	printf(" |  Mbed-OS version: %d.%d.%-43d| \r\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
	printf(" +------------------------------------------------------------------+ \r\n");
	printf(" |  %-64s| \r\n", "Write some json using jWrite");
#ifdef JW_GLOBAL_CONTROL_STRUCT
	printf(" |  %-64s| \r\n", "Using global control struct");
	printf(" |  %-64s| \r\n", "See jWrite.h, #define JW_GLOBAL_CONTROL_STRUCT");
#else
	printf(" |  %-64s| \r\n", "Using supplied jWriteControl");
	printf(" |  %-64s| \r\n", "See jWrite.h, #define JW_GLOBAL_CONTROL_STRUCT");
#endif
	printf(" +------------------------------------------------------------------+ \r\n");
	printf("\r\n");

	jWriteTest();

	printf("\r\n");
	printf(" +------------------------------------------------------------------+ \r\n");
	printf(" |  %-64s| \r\n", "Read some json using jRead");
	printf(" |  %-64s| \r\n", "Basic example from readme on Github");
	printf(" +------------------------------------------------------------------+ \r\n");
	printf("\r\n");

	jReadExample();

	printf("\r\n");
	printf(" +------------------------------------------------------------------+ \r\n");
	printf(" |  %-64s| \r\n", "Read some json using jRead");
	printf(" |  %-64s| \r\n", "Some examples:");
	printf(" +------------------------------------------------------------------+ \r\n");
	printf("\r\n");

	runExamples();

	printf("\r\n");
	printf(" +------------------------------------------------------------------+ \r\n");
	printf(" |  %-64s| \r\n", "Read some json using jRead");
	printf(" |  %-64s| \r\n", "Advanced example from readme on Github");
	printf(" +------------------------------------------------------------------+ \r\n");
	printf("\r\n");

	articleExample();

	printf("\r\n");
	printf(" +------------------------------------------------------------------+ \r\n");
	printf(" |  %-64s| \r\n", "DONE!");
	printf(" +------------------------------------------------------------------+ \r\n");
	printf("\r\n");

	return 0;
}

	//------------------------------------------------------------

#ifdef JW_GLOBAL_CONTROL_STRUCT

// Examples of jWrite
// - using global control structure
//
void jWriteTest()
{
	char buffer[1024];
	unsigned int buflen = 1024;
	int err;

	printf("A JSON object example:\n\n");

	jwOpen(buffer, buflen, JW_OBJECT, JW_PRETTY); // start root object

	jwObj_string("key", "value"); // add object key:value pairs
	jwObj_int("int", 1);
	jwObj_double("double", 1.234);
	jwObj_null("nullThing");
	jwObj_bool("bool", 1);
	jwObj_array("EmptyArray");
	// empty array
	jwEnd();
	jwObj_array("anArray"); // array with elements
	jwArr_string("array one");
	jwArr_int(-2);
	jwArr_double(1234.567);
	jwArr_null();
	jwArr_bool(0);
	jwArr_object(); // object in array
	jwObj_string("obj3_one", "one");
	jwObj_string("obj3_two", "two");
	jwEnd();
	jwArr_array(); // array in array
	jwArr_int(0);
	jwArr_int(1);
	jwArr_int(2);
	jwEnd();
	jwEnd(); // end of "anArray" , back to root object

	jwObj_object("EmptyObject");
	jwEnd();

	jwObj_object("anObject"); // object in the root object
	jwObj_string("msg", "object in object");
	jwObj_string("msg2", "object in object 2nd entry");
	jwEnd();
	jwObj_string("ObjEntry", "This is the last one");

	err = jwClose(); // close and get error code

	printf(buffer);
	if (err != JWRITE_OK)
		printf("Error: %s at function call %d\n", jwErrorToString(err), jwErrorPos());

	printf("\n\nA JSON array example:\n\n");

	jwOpen(buffer, buflen, JW_ARRAY, JW_PRETTY);
	jwArr_string("String value");
	jwArr_int(1234);
	jwArr_double(567.89012);
	jwArr_bool(1);
	jwArr_null();
	jwArr_object();
	// empty object
	jwEnd();
	jwArr_object();
	jwObj_string("key", "value");
	jwObj_string("key2", "value2");
	jwEnd();
	jwArr_array(); // array in array
	jwArr_string("Array in array");
	jwArr_string("the end");
	jwEnd();
	jwArr_string("Empty array next...");
	jwArr_array();
	// empty array
	jwEnd();
	err = jwClose();

	printf(buffer);
	if (err != JWRITE_OK)
		printf("Error: %s at function call %d\n", jwErrorToString(err), jwErrorPos());

	printf("\n\nExample error:\n\n");
	// this is a copy of the above array example with an error introduced...
	//
	jwOpen(buffer, buflen, JW_ARRAY, JW_PRETTY); // 1
	jwArr_string("String value");				 // 2
	jwArr_int(1234);							 // 3
	jwArr_double(567.89012);					 // 4
	jwArr_bool(1);								 // 5
	jwArr_null();								 // 6
	jwArr_object();								 // 7
												 // empty object
	//jwEnd();
	jwArr_object();						 // 8  <-- this is where the error is
	jwObj_string("key", "value");		 // 9
	jwObj_string("key2", "value2");		 // 10
	jwEnd();							 // 11
	jwArr_array();						 // array in array			// 12
	jwArr_string("Array in array");		 // 13
	jwArr_string("the end");			 // 14
	jwEnd();							 // 15
	jwArr_string("Empty array next..."); // 16
	jwArr_array();						 // 17
										 // empty array
	jwEnd();							 // 18
	err = jwClose();

	printf(buffer);
	if (err != JWRITE_OK)
		printf("Error: %s at function call %d\n", jwErrorToString(err), jwErrorPos());

	return;
}

#else /* JW_GLOBAL_CONTROL_STRUCT */

// Same examples using user-defined control structure
// - to compile in this mode, comment out the JW_GLOBAL_CONTROL_STRUCT definition
//
void jWriteTest()
{
	char buffer[1024];
	unsigned int buflen = 1024;
	int err;
	struct jWriteControl jwc;

	printf("jWrite - a JSON object example:\n\n");
	//
	// Example JSON object
	//
	jwOpen(&jwc, buffer, buflen, JW_OBJECT, JW_PRETTY);

	jwObj_string(&jwc, "key", "value");
	jwObj_int(&jwc, "int", 1);
	jwObj_double(&jwc, "double", 1.234);
	jwObj_null(&jwc, "nullThing");
	jwObj_bool(&jwc, "bool", 1);
	jwObj_array(&jwc, "EmptyArray"); // empty array
	jwEnd(&jwc);
	jwObj_array(&jwc, "anArray");
	jwArr_string(&jwc, "array one");
	jwArr_int(&jwc, 2);
	jwArr_double(&jwc, 1234.567);
	jwArr_null(&jwc);
	jwArr_bool(&jwc, 0);
	jwArr_object(&jwc);
	jwObj_string(&jwc, "obj3_one", "one");
	jwObj_string(&jwc, "obj3_two", "two");
	jwEnd(&jwc);
	jwArr_array(&jwc);
	jwArr_int(&jwc, 0);
	jwArr_int(&jwc, 1);
	jwArr_int(&jwc, 2);
	jwEnd(&jwc);
	jwEnd(&jwc);

	jwObj_object(&jwc, "EmptyObject");
	jwEnd(&jwc);

	jwObj_object(&jwc, "anObject");
	jwObj_string(&jwc, "msg", "object in object");
	jwObj_string(&jwc, "msg2", "object in object 2nd entry");
	jwEnd(&jwc);
	jwObj_string(&jwc, "ObjEntry", "This is the last one");

	err = jwClose(&jwc);

	printf(buffer);
	if (err != JWRITE_OK)
		printf("Error: %s at function call %d\n", jwErrorToString(err), jwErrorPos(&jwc));

	printf("\n\nA JSON array example:\n\n");

	jwOpen(&jwc, buffer, buflen, JW_ARRAY, JW_PRETTY);
	jwArr_string(&jwc, "String value");
	jwArr_int(&jwc, 1234);
	jwArr_double(&jwc, 567.89012);
	jwArr_bool(&jwc, 1);
	jwArr_null(&jwc);
	jwArr_object(&jwc);
	// empty object
	jwEnd(&jwc);
	jwArr_object(&jwc);
	jwObj_string(&jwc, "key", "value");
	jwObj_string(&jwc, "key2", "value2");
	jwEnd(&jwc);
	jwArr_array(&jwc); // array in array
	jwArr_string(&jwc, "Array in array");
	jwArr_string(&jwc, "the end");
	jwEnd(&jwc);
	err = jwClose(&jwc);

	printf(buffer);
	if (err != JWRITE_OK)
		printf("Error: %s at function call %d\n", jwErrorToString(err), jwErrorPos(&jwc));

	printf("\n\nExample error:\n\n");
	jwOpen(&jwc, buffer, buflen, JW_ARRAY, JW_PRETTY);	// 1
	jwArr_string(&jwc, "String value");		   			// 2
	jwArr_int(&jwc, 1234);					  			// 3
	jwArr_double(&jwc, 567.89012);					  	// 4
	jwArr_bool(&jwc, 1);					 			// 5
	jwArr_null(&jwc);						   			// 6
	jwArr_object(&jwc);						   			// 7
											   			// empty object
	//jwEnd( &jwc );
	jwArr_object(&jwc);					  // 8  <-- this is where the error is
	jwObj_string(&jwc, "key", "value");   // 9
	jwObj_string(&jwc, "key2", "value2"); // 10
	jwEnd(&jwc);						  // 11
	jwArr_array(&jwc);					  // array in array		// 12
	jwArr_string(&jwc, "Array in array"); // 13
	jwArr_string(&jwc, "the end");		  // 14
	jwEnd(&jwc);						  // 15
	err = jwClose(&jwc);				  // 16

	printf(buffer);
	if (err != JWRITE_OK)
		printf("Error: %s at function call %d\n", jwErrorToString(err), jwErrorPos(&jwc));

	return;
}

#endif /* JW_GLOBAL_CONTROL_STRUCT */

//******************* jRead ***********************
//-------------------------------------------------
// Do a query and print the results
//
void testQuery(const char *pJson, const char *query)
{
	struct jReadElement jElement;
	jRead(pJson, query, &jElement);
	printf("Query: \"%s\"\n", query);
	printf("return: %d= %s\n", jElement.error, jReadErrorToString(jElement.error));
	printf(" dataType = %s\n", jReadTypeToString(jElement.dataType));
	printf(" elements = %d\n", jElement.elements);
	printf(" bytelen  = %d\n", jElement.bytelen);
	printf(" value    = %*.*s\n\n", jElement.bytelen, jElement.bytelen, (char *)jElement.pValue);
}

//=================================================================
// Examples
// - whole bunch of jRead examples
//
void runExamples()
{
	long l;
	int i;
	double d;
	char str[128];
	struct jReadElement arrayElement;

	const char *exampleJson =
		"{\n"
		"  \"astring\": \"This is a string\",\n"
		"  \"number1\": 42,\n"
		"  \"number2\":  -123.45,\n"
		"  \"anObject\":{\"one\":1,\"two\":{\"obj2.1\":21,\"obj2.2\":22},\"three\":333},\n"
		"  \"anArray\":[0, \"one\", {\"two.0\":20,\"two.1\":21}, 3, [4,44,444]],\n"
		"  \"isnull\":null,\n"
		"  \"emptyArray\":[],\n"
		"  \"emptyObject\":{  },\n"
		"  \"yes\": true,\n"
		"  \"no\":  false\n"
		"}\n";

	printf( "Example json:\r\n %s\r\n", exampleJson);

	testQuery(exampleJson, "");
	testQuery(exampleJson, "[1");
	testQuery(exampleJson, "{'astring'");
	testQuery(exampleJson, "{'number1'");
	testQuery(exampleJson, "{'number2'");
	testQuery(exampleJson, "{'anObject'");
	testQuery(exampleJson, "{'anArray'");
	testQuery(exampleJson, "{'isnull'");
	testQuery(exampleJson, "{'yes'");
	testQuery(exampleJson, "{'no'");
	testQuery(exampleJson, "{'missing'");
	testQuery(exampleJson, "{'anObject'{'two'");
	testQuery(exampleJson, "{'anObject' {'two' {'obj2.2'");
	testQuery(exampleJson, "{'anObject'{'three'");
	testQuery(exampleJson, "{'anArray' [1");
	testQuery(exampleJson, "{'anArray' [2 {'two.1'");
	testQuery(exampleJson, "{'anArray' [4 [2");
	testQuery(exampleJson, "{'anArray' [999");

	printf("Empty array or object...\n");
	testQuery(exampleJson, "{'emptyArray'");
	testQuery(exampleJson, "{'emptyObject'");

	printf("Return the key at a given index in an object...\n");
	testQuery(exampleJson, "{3");
	testQuery(exampleJson, "{'anObject' {1");
	testQuery(exampleJson, "{999");

	// examples of helper functions
	l = jRead_long(exampleJson, "{'number1'", NULL);		// 42
	i = jRead_int(exampleJson, "{'yes'", NULL);				// 1	(BOOL example)
	d = jRead_double(exampleJson, "{'number2'", NULL);		// -123.45
	jRead_string(exampleJson, "{'astring'", str, 16, NULL); // "This is a strin\0" (buffer too short example)

	printf("Helper Functions...\n");
	printf("  \"number1\"= %ld\n", l);
	printf("  \"yes\"    = %d\n", i);
	printf("  \"number2\"= %lf\n", d);
	printf("  \"astring\"= \"%s\"\n", str);

	// Example of cascading queries
	printf("\nQueries on sub-elements and use of query parameters...\n");

	// locate "anArray"...
	jRead(exampleJson, "{'anArray'", &arrayElement);
	printf("  \"anArray\": = %*.*s\n\n", arrayElement.bytelen, arrayElement.bytelen, (char *)arrayElement.pValue);

	// do queries within "anArray"...
	for (i = 0; i < arrayElement.elements; i++)
	{
		// index the array using queryParam
		jRead_string((char *)arrayElement.pValue, "[*", str, 128, &i);
		printf("  anArray[%d] = %s\n", i, str);
	}

	// example using a parameter array
	{
		int params[2] = {2, 1};
		jRead_string((char *)arrayElement.pValue, "[*{*", str, 128, params);
		printf("\n  anArray[%d] objectKey[%d] = \"%s\"\n", params[0], params[1], str);
	}
}

//=================================================================
//
// Example used in article
//
#define NAMELEN 32
struct NamesAndNumbers
{
	char Name[NAMELEN];
	long Number;
};

void articleExample()
{
	struct NamesAndNumbers people[42];
	struct jReadElement element;
	int i;
	const char *pJson =
		"{\n"
		"  \"Company\": \"The Most Excellent Example Company\",\n"
		"  \"Address\": \"Planet Earth\",\n"
		"  \"Numbers\":[\n"
		"    { \"Name\":\"Fred\",   \"Ident\":12345 },\n"
		"    { \"Name\":\"Jim\",    \"Ident\":\"87654\" },\n"
		"    { \"Name\":\"Zaphod\", \"Ident\":\"0777621\" }\n"
		"  ]\n"
		"}\n";

	printf("Example Json to parse: \r\n%s\r\n", pJson);
	
	printf("\r\n");
	printf("Read Numbers array\r\n");
	jRead(pJson, "{'Numbers'", &element); // we expect "Numbers" to be an array
	

	if (element.dataType == JREAD_ARRAY)
	{
		printf("Numbers is of dataType: %d\r\n", element.dataType);
		for (i = 0; i < element.elements; i++) // loop for no. of elements in JSON
		{
			jRead_string(pJson, "{'Numbers'[*{'Name'", people[i].Name, NAMELEN, &i);
			people[i].Number = jRead_long(pJson, "{'Numbers'[*{'Ident'", &i);
		}
		//Print result
		for (i = 0; i < element.elements; i++) // loop for no. of elements in JSON
		{
			printf("Name: %s, Ident: %ld\r\n", people[i].Name, people[i].Number);
		}
	}
	i = 0;
}

void jReadExample()
{

	const char *pJson = 
	"{\n"
	"  \"astring\":\"This is a string\",\n"
	"  \"anumber\":42,\n"
	"  \"myarray\":[ \"zero\", 1, {\"description\":\"element 2\"}, null ],\n"
	"  \"yesno\":true,\n"
	"  \"PI\":\"3.1415926\",\n"
	"  \"foo\":null\n"
	"}\n";
	
	printf( "Example JSON to parse:\r\n%s\r\n", pJson);

	testQuery( pJson, "{'astring'" );

	testQuery( pJson, "{'myarray'" );

	testQuery( pJson, "{'myarray' [2 {'description'" );

	// Helper functions
	char destString[100];
	jRead_string( pJson, "{'astring'", destString, 100, NULL );

	printf( "jRead_string, query: %s, result: %s\r\n", "{'astring'", destString );

	int my_int = 0;
	my_int = jRead_int( pJson, "{'myarray'[1", NULL );

	printf( "jRead_int, query: %s, result: %d\r\n", "{'myarray'[1", my_int );

	long my_long = 0;
	my_long = jRead_long( pJson, "{'anumber'", NULL );

	printf( "jRead_long, query: %s, result: %ld\r\n", "{'anumber'", my_long );

	double my_double = 0.0;
	my_double = jRead_double( pJson, "{'PI'", NULL );

	printf( "jRead_double, query: %s, result: %.7f\r\n", "{'PI'", my_double );

}