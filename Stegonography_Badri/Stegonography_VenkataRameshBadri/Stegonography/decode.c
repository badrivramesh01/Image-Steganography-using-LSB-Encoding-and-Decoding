#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "decode.h"
#include "types.h"
#include "common.h"

//declaring the unsigned integer magic string size variable
uint magic_string_size;
//declaring the unsigned integer secret file extension size
uint secret_file_extn_size;
//declaring the unsigned integer secret file size
uint secret_file_size;
//declaring the character array
char res[50];


/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    //checking the condition
    if(argv[2] != NULL && (strcmp((strstr(argv[2],".")),".bmp") == 0))
    {
	decInfo -> stego_image_fname = argv[2];
    }
    else
    {
	//condition false then it will return failure
	return e_failure;
    }
    //checks the condition argument 3 is null or not
    if(argv[3] == NULL)
    {
	//decode assigning to output fname
	decInfo -> output_fname ="decode";
    }
    //condition false it will works
    else
    {
	if(strchr(argv[3],'.'))
	{
	    decInfo -> output_fname = argv[3];
	}
	else
	{
	    //prints as output
	    printf("It is not text file\n");
	    //returning the failure
	    return e_failure;
	}
    }

    return e_success;
}

/* Open files for decoding */
Status open_decode_files(DecodeInfo *decInfo)
{
    // Stego Image file
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    // Do Error handling
    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);

        return e_failure;
    }

    return e_success;
}

/* Decode Magic String */
Status decode_magic_string(int magic_string_size,DecodeInfo *decInfo)
{
     //declaring character buf
    char buf[magic_string_size *8];
    //memory allocation magic string size and +1 to magic data
    decInfo -> magic_data = malloc(magic_string_size + 1);
    //loop runs until the magic string size
    for(int i=0;i<magic_string_size;i++)
    {
	//reads the stego image
	fread(buf,1,8,decInfo -> fptr_stego_image);
	//function calling
	decode_from_lsb_byte(&decInfo -> magic_data[i],buf,decInfo);
    }
    //magic data assign to null
    decInfo -> magic_data[magic_string_size] = '\0';
    return e_success;
}

Status decode_from_lsb_byte(char *data,char *buffer,DecodeInfo *decInfo)
{
    //declaring the ret and initialsing the 7 to ret
    int ret = 7;
    //declaring unsigned character c
    unsigned char ch = 0x00;
    //loop runs until the 8
    for(int i=0;i<8;i++)
    {
	//storing in ch
	ch = ((buffer[i] & 1) << ret--) | ch;
    }
    *data = ch;
    return e_success;
}

/* Decode the size of the secret file extension */
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    //declaring the character array
    char arr[32];
    //declaring the integer ret and initilialising the 31 to ret
    int ret=31;
    //reads the stego image
    fread(arr,1,32,decInfo -> fptr_stego_image);
    //loop runs until the 32
    for(int i=0;i<32;i++)
    {
	//assigning the secret file extension size
	secret_file_extn_size = secret_file_extn_size | ((arr[i] & 1) << ret--);
    }
    return e_success;
}

/* Decode secret file extension */
Status decode_secret_file_extn(char secret_file_size,DecodeInfo *decInfo)
{
     //declaring the character buf
    char buf[secret_file_size * 8];
    //memory allocation of secret file size and +1 to the file extension
    decInfo -> file_extn = malloc(secret_file_size + 1);
    //loop runs until the secret file size
    for(int i=0;i<secret_file_size;i++)
    {
	//reading the stego image
	fread(buf,1,8,decInfo -> fptr_stego_image);
	//function calling
	decode_from_lsb_byte(&decInfo -> file_extn[i],buf,decInfo);
    }
    //file extension assigning to null
    decInfo -> file_extn[secret_file_size] = '\0';
    //copyimng the output fname to res
    strcpy(res,decInfo -> output_fname);
    //checks the condition
    if(decInfo -> output_fname == "decode")
    {
	strcat(res,decInfo -> file_extn);
    }
    //fopen res has write
    decInfo -> fptr_output_data = fopen(res, "w");
    //checking the condition output data to null
    if(decInfo -> fptr_output_data == NULL)
    {
	perror("fopen");
	//prints the output fname
	fprintf(stderr,"ERROR : Unable to open file %s\n",decInfo -> output_fname);
	return e_failure;
    }
    //prints has file extension
    printf("file_extn:%s\n",res);
    return e_success;
}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    //declaring the character array
    char arr[32];
    //declaring the integer ret and initialising the 31 to ret
    int ret=31;
    //read the stego image
    fread(arr,1,32,decInfo -> fptr_stego_image);
    //loop runs until the 32
    for(int i=0;i<32;i++)
    {
	//storing the secret file size
	secret_file_size = secret_file_size | ((arr[i] & 1) << ret--);
    }
    return e_success;
}

Status decode_secret_file_data(int secret_file_size,DecodeInfo *decInfo)
{
    //declaring character array
    char buffer[secret_file_size * 8];
    //memory allocation to file data from secret file size
    decInfo -> secret_file_data = malloc(secret_file_size + 1);
    //for loop runs until the secret file size
    for(int i=0;i<secret_file_size;i++)
    {
	//read the stego i mage
	fread(buffer,1,8,decInfo -> fptr_stego_image);
	//function calling
	decode_from_lsb_byte(&decInfo -> secret_file_data[i],buffer,decInfo);
	//prints the output data
	fputc(decInfo -> secret_file_data[i],decInfo -> fptr_output_data);
    }
    decInfo -> secret_file_data[secret_file_size] ='\0';
    //returning the success
    return e_success;
}
//function definition
Status decode_magic_string_size(DecodeInfo *decInfo)
{
     //moving the stego image to 54
    fseek(decInfo -> fptr_stego_image,54,SEEK_SET);
    //declaring the character array
    char arr[32];
    //declaring the integer bit and initialising the 31 to bit
    int bit=31;
    //reads the stego image
    fread(arr,1,32,decInfo -> fptr_stego_image);
    //loop runs until the 32
    for(int i=0;i<32;i++)
    {
	//storing magic string size
	magic_string_size=magic_string_size | ((arr[i] & 1) << (bit--));
    }
    return e_success;
}

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo)
{
    //prints as output
    printf("Start decoding\n");
    //checks the condition has success or not
    if(open_decode_files(decInfo) == e_success)
    {
	//prints as output
	printf("Open files was successful\n");
    }
    //condition false it will executes
    else
    {
	//prints as output
	printf("Open files was failure\n");
        return e_failure;
    }
    //checks condition has equal to success or not
    if(decode_magic_string_size(decInfo) == e_success)
    {
	//prints as output
	printf("Magic string size is successful\n");
    }
    //condition false it will works
    else
    {
	//prints as output
	printf("Magic string was failure\n");
	return e_failure;
    }

    //checks the condition is equalk to success or not
    if(decode_magic_string(magic_string_size,decInfo) == e_success)
    {
	//prints as output
	printf("Magic string is successful\n");
    }
    //condition false then it will works
    else
    {
	//prints as output
	printf("Magic string was failure\n");
	return e_failure;
    }
    //checking the condition is equal to success or not
    if(decode_secret_file_extn_size(decInfo) == e_success)
    {
	//prints as output
	printf("Secret file extension size is successful\n");
    }
    //condition false then it will works
    else
    {
	//prints as output
	printf("Secret file extension size was failure\n");
	return e_failure;
    }
    //checking the condition is equal to success or not
    if(decode_secret_file_extn(secret_file_extn_size,decInfo) == e_success)
    {
	//prints as output
	printf("Secret file extension was successful\n");
    }
    //condition false then it will works
    else
    {
	//prints as output
	printf("Secret file extension was failure\n");
	return e_failure;
    }
    //checking the condition is equal to success or not
    if(decode_secret_file_size(decInfo) == e_success)
    {
	//prints as output
	printf("Secret file size is successful\n");
    }
    //condition false then it will works
    else
    {
	//prints as output
	printf("Secret file size was failure\n");
	return e_failure;
    }
    //checking the condition is equal to success or not
    if(decode_secret_file_data(secret_file_size,decInfo) == e_success)
    {
	//prints as output
	printf("Secret file data is successful\n");
    }
    //condition false then it will works
    else
    {
	//prints as output
	printf("Secret file data was failure\n");
	return e_failure;
    }    //retuens success
    return e_success;
}


