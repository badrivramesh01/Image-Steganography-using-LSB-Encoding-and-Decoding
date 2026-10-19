#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

//function definition
OperationType check_operation_type(char *argv[])
{
    //checks the conmdition argument 1 is -e is equal to 0
    if( strcmp( argv[1], "-e") == 0 )
    {
	//returns e encode
	return e_encode;
    }
    //checks the condition
    else if( strcmp(argv[1], "-d") == 0)
    {
	//returns the e decode
	return e_decode;
    }
    //condition false then it will executes
    else
	return e_unsupported;
}
//main function
int main(int argc,char * argv[])
{
    //encoding structure name
    EncodeInfo encInfo;
    //decoding structure name
    DecodeInfo decInfo;
    //checking the operation type
    if(check_operation_type(argv) == e_encode)
    {
	//printing the statement
	printf("Selected Encoding\n");
	//EncodeInfo encInfo;
	//checking read and validate the encode arguments for encoding
	if ( read_and_validate_encode_args( argv, &encInfo ) == e_success )
	{
	    //printing the statement
	    printf("Read and validate args is successful\n");
	    //start the encoding
	    if ( do_encoding( &encInfo ) == e_success )
	    {
		//printing the statement
		printf("Encoding is successful\n");
	    }
	    else
	    {
		//printing the error statement
		printf("Encoding is failure\n");
		return -1;
	    }
	}
	else
	{
	    //printing the error statement
	    printf("Read and validate encode args was failure\n");
	    return -1;
	}
    }
    //checking operation type for decoding
    else if(check_operation_type(argv) == e_decode)
    {
	//printing the statement
	printf("Selected Decoding\n");
	//checking the read and validate arguments foe decoding
	if(read_and_validate_decode_args(argv,&decInfo) == e_success)
       	{
	    //printing the statement
	    printf("Read and validate decoding arguments are successfull\n");
	    //start the decoding
	    if(do_decoding(&decInfo) == e_success)
	    {
		//printing the statement
	       	printf("Decoding is successful\n");
	    }
	    else
	    {
		//printing the error statement
	       	printf("Decoding failure\n");
	       	return -1;
	    }
       	}
       	else
       	{
	    //printing the error statement
	    printf("Read and validate decode arguments was failure\n");
	    return -1;
       	}
    }
    else
    {
	//printing the error statement
	printf("Invalid Option\n");
    }
}

